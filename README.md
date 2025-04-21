# Aerecon-Drone
Information regarding implementation specifics like code, explanations, and updated system planning can be found here.

# Prototype System Architecture:

![image](https://github.com/user-attachments/assets/3b2be2d2-a921-4a21-8a07-7afefe9c7adf)

### **High Priority Tasks**:
1. **PID Control Loop**
   - **Gyro Polling (I2C)**: Use interrupt driven reading for gyro data instead of continuous polling. 
   - **Control Loop**: This is the core task and should run at the highest priority.
   - **Use Hardware PWM**: Instead of using software-based PWM like what we're doing for testing, use hardware based PWM to offload timing control to dedicated hardware.
   - **Avoid Software Delays**: Use vTaskDelayUntil() to ensure tasks run at fixed time intervals and avoid unnecessary delays caused by software-level waits.

2. **PWM Updates**
   - **Hardware PWM**: Use hardware PWM here as well.
   - **Separation of PWM Update Logic**: Keep the PWM update task separate from the control loop, this gives better responsiveness.

  <img src="https://github.com/user-attachments/assets/536bf8fe-e4d0-499a-9e5d-1e5c4587b699" width="800">

   ### **Low Priority Tasks/Telemetry**:
1. **Barometer (BMP390)**
   - **Altitude Hold and Sensing** allows us to stay at specified altitudes without user input.
 
3. **Magnetometer (MMC5983MA)**
   - **Read Data in Bursts**: Avoid polling the magnetometer in tight loops. Instead, read data in bursts every 100ms or so.

4. **GPS (NEOM9N00B)**
   - **Non-blocking Parsing**: Avoid blocking the GPS parsing in the main control loop. Instead, run the GPS parsing asynchronously and update global variables with the parsed data.
   - **Efficient Handling**: Use interrupts or a separate task for handling GPS data reception and parsing.

Future Work:
**Adaptive Tuning Task**
 - Run as a **lower-priority** task that runs every 10 seconds or so.
 - **Collect Response Data**: Record data from the IMU during a step response test.
 - **Tune PID Gains**: Based on the recorded data (e.g., overshoot, rise time, settling time), compute new PID gains using a cost function or heuristics.
   
Notes: For the next IMU (ICM-42688-P), use FIFO Buffer and SPI.
