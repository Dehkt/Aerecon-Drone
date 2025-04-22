// This is a simple closed loop, we are using interrupt driven reading from MPU6050, reading acceleration data, calculating Pitch, 
// and using that to adjust the motor's rotation in real time to counteract the Pitch.
#include <Wire.h>

#define MPU_ADDR       0x68
#define ACCEL_XOUT_H   0x3B
#define PWR_MGMT_1     0x6B
#define INT_ENABLE     0x38
#define INT_PIN_CFG    0x37
#define SMPLRT_DIV     0x19
#define CONFIG         0x1A
#define GYRO_CONFIG    0x1B
#define ACCEL_CONFIG   0x1C

// Adjustable pins
#define MPU_INT_PIN    4
#define ESC_PIN        14

// ISR Flag for interrupt pin
volatile bool mpu_data_ready = false;

// MPU interrupt service routine
void IRAM_ATTR mpuISR() {
  mpu_data_ready = true;
}

// I2C write helper
void I2C_write(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void MPUInit() {
  I2C_write(PWR_MGMT_1, 0x00);
  I2C_write(SMPLRT_DIV, 0x07);
  I2C_write(CONFIG, 0x03);
  I2C_write(GYRO_CONFIG, 0x08);
  I2C_write(ACCEL_CONFIG, 0x10);
  I2C_write(INT_PIN_CFG, 0x10); // Interrupt active high
  I2C_write(INT_ENABLE, 0x01);  // Enable data ready interrupt
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA and SCL
  pinMode(ESC_PIN, OUTPUT);
  pinMode(MPU_INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(MPU_INT_PIN), mpuISR, RISING);
  MPUInit();
}

void loop() {
  if (mpu_data_ready) {
    // Reset Flag
    mpu_data_ready = false; 

    // Reading MPU6050 Data over I2C
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6);

    // Acceleration Data Read for Pitch Calculation
    int16_t ax = (Wire.read() << 8) | Wire.read();
    int16_t ay = (Wire.read() << 8) | Wire.read();
    int16_t az = (Wire.read() << 8) | Wire.read();

    // Convert to g (assuming 8g config → 4096 LSB/g)
    float ax_g = ax / 4096.0;
    float ay_g = ay / 4096.0;
    float az_g = az / 4096.0;

    // Calculate pitch angle (in degrees), we can use a lookup table for atan2 to reduce CPU cycles and other calculations
    float pitch = atan2(ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180.0 / PI;

    // Map pitch to PWM centered at 1500us, add 200us max correction
    int pulseUs = 1500 - (int)(pitch * 5);  // Tunable: 5us per deg

    // Constrain pulse to ESC safe range
    pulseUs = constrain(pulseUs, 1000, 2000);

    // Write Calculated PWM Signal to ESC Pin
    digitalWrite(ESC_PIN, HIGH);
    delayMicroseconds(pulseUs);
    digitalWrite(ESC_PIN, LOW);
    delayMicroseconds(20000 - pulseUs);

    // Print for Debugging
    Serial.print("Pitch: "); Serial.print(pitch);
    Serial.print(" | PWM: "); Serial.println(pulseUs);
  }
}
