// This uses a BK10 Potentiometer to Vary the PWM Signal Input to our ESCs, this is an example used for calibrating the motors and controlling their speeds.
const int potPin = 34;  // potentiometer connected to ESP32's ADC
const int escPin = 14; // ESC pin

void setup() {
  Serial.begin(115200);
  pinMode(escPin, OUTPUT);
}

void loop() {
  int potValue = analogRead(potPin); // Read potentiometer value (0 to 4095)

  int pulseUs = map(potValue, 0, 4095, 1000, 2000); // Mapping potentiometer value range to the pulse width in microseconds (1000us to 2000us).
  
  // Manual PWM Signal Generation
  digitalWrite(escPin, HIGH);
  delayMicroseconds(pulseUs);
  digitalWrite(escPin, LOW);
  delayMicroseconds(20000 - pulseUs);
  // Setting pin to high for the desired pulsewidth, then low for the rest of the 20ms period according to the desired duty cycle.

  // Printing output for debugging/direction info
  Serial.print("Potentiometer: "); Serial.print(potValue);
  Serial.print("Pulse Width: "); Serial.print(pulseUs);
  Serial.print("Duty Cycle: "); Serial.println(pulseUs);
}
