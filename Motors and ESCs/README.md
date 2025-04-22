# Aerecon Drone Motors

## BLDC Motors
- **Motor Model**: EMAX XA2212 820KV
- **Motor Type**: Brushless DC (BLDC)
- **Voltage Range**: 8-12V DC
- **Max Current**: 7.3-12A

![image](https://github.com/user-attachments/assets/25bafa40-c859-4ea5-b2b2-d236ace84131)

---

## Batteries Used
- **Battery Type**: LiPo 2S, 2x
- **Voltage Rating**: 7.4V
- **Capacity**: 8400mAh
- **C-Rating** : 100C (Discharge Rate)
- **Connector Type**: Deans / T Connector

---

## ESC (Electronic Speed Controller)
- **ESC Model**: EMAX Lightning BLHeli ESC
- **Amperage Rating**: 20A
- **Supported Input Voltage**: 4S, up to 16.8V
- **Firmware** : [BLHeli Github](https://github.com/bitdump/BLHeli)
- **Datasheet**: [PDF](https://github.com/user-attachments/files/19827448/BLHeli.Instructions.pdf)

> Notes: Also supports OneShot, Multishot. Look into these protocols as a potential option after first prototype.

---

## ESC Calibration Steps 
1. Remove Propellers for safety.
2. Ensure Power Switch is Off.
3. Connect ESC to One side of the power switch and LiPo battery to the other.
4. Input a 100% Throttle Signal to the ESC.
5. Turn the Power Switch On.
6. 3 Beeps, followed by a Jingle that is "ramping up" will be heared.
7. After the Jingle, quickly pull throttle to 0%.
8. A new Jingle that is ramping down should be heard.
9. Calibration is complete.

---

## PWM Signal Generation

- **PWM Frequency**: 50 Hz 
- **Pulse Width Period**: 20ms
  - 1 ms: 0% Throttle
  - 2 ms: 100% Throttle
- **Duty Cycle Calculation**: Duty Cycle (%) = (Pulse Width in µs / Period in µs) × 100

| Pulse Width (µs) | Duty Cycle (%) | Description       |
|------------------|----------------|-------------------|
| 1000             | 5%             | 0% throttle       |
| 1500             | 7.5%           | Mid throttle      |
| 2000             | 10%            | Full throttle     |


![image](https://github.com/user-attachments/assets/8c90948b-a5c0-487c-a4e1-51acb9a9da7c)

