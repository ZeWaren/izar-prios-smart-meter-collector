# IZAR I R4 PRIOS meter collector

![izar_rc_i_r4](doc/IZAR_RC_I_R4.jpg "A IZAR counter") ![STEVAL-FKI868V2](doc/steval-fki868v2.jpg "STEVAL-FKI868V2")

## Introduction

This project contains the implementation of a Wireless M-Bus (WMBus) collector for DIEHL IZAR I R4 smart meters, running
on a ST-Micro [STEVAL-FKI868V2](https://www.st.com/en/evaluation-tools/steval-fki868v2.html) kit (a [NUCLEO-L053R8](https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-mpu-eval-tools/stm32-mcu-mpu-eval-tools/stm32-nucleo-boards/nucleo-l053r8.html) board with a [S2-LP](https://www.st.com/content/st_com/en/products/wireless-transceivers-mcus-and-modules/sub-1ghz-rf/s2-lp.html) 868MHz transceiver).

### Supported smart meters

- DIEHL Metering (ex SAPPEL) IZAR 868 I R4 LIGHT

Many more devices should be supported (probably most if not all DIEHL PRIOS smart water meters).

Please report yours if you tested them.

## Usage

### General

Build the application using your favorite IDE (an IAR/EWARM workspace is provided).

Flash the firmware into the Cortex M0 microcontroller.

Plug the board into a computer using a USB connector.

Open the USB virtual COM port provided by the board, with speed 115200.

The device outputs every smart meter readings in real time:
    
    # socat open:/dev/cuaU0,raw,echo=0,ispeed=115200,ospeed=115200 -
    20d01c15,70.686996,69.581001,m3,2020,04,01,9.0,32,1,0,0,0,0,0,0,0,0,0,0,0,0
    20d02a72,65.434998,64.680000,m3,2020,04,01,9.0,32,3,0,0,0,0,0,0,0,0,0,0,0,0
    20d03c19,174.847000,172.501999,m3,2020,04,01,9.0,32,3,0,0,0,0,0,0,0,0,0,0,0,0
    20d04a74,59.242001,58.627998,m3,2020,04,01,9.0,32,2,0,0,0,0,0,0,0,0,0,0,0,0
    20d05c1d,92.295998,90.585999,m3,2020,04,01,9.0,32,2,0,0,0,0,0,0,0,0,0,0,0,0
    20d06a73,100.885002,99.934998,m3,2020,04,01,9.0,32,2,0,0,0,0,0,0,0,0,0,0,0,0
    20d07c1c,301.843994,300.838989,m3,2020,04,01,9.0,32,2,0,0,0,0,0,0,0,0,0,0,0,0
    20d08c1e,102.694000,101.646004,m3,2020,04,01,9.0,32,0,0,0,0,0,0,0,0,0,0,0,0,0
    20d09c16,92.638000,92.277000,m3,2020,04,01,9.0,32,2,0,0,0,0,0,0,0,0,0,0,0,0

The fields are:
- Meter id,
- Current value,
- H0 value,
- measurement unit,
- Year of H0,
- Month of H0,
- Day of H0,
- Remaining battery life (years),
- Radio interval (seconds),
- Random generator,
- Alarm: general_alarm,
- Alarm: leakage_currently,
- Alarm: leakage_previously,
- Alarm: meter_blocked,
- Alarm: back_flow,
- Alarm: underflow,
- Alarm: overflow,
- Alarm: submarine,
- Alarm: sensor_fraud_currently,
- Alarm: sensor_fraud_previously,
- Alarm: mechanical_fraud_currently,
- Alarm: mechanical_fraud_previously

In my case, the H0 value is the reading at the end of the last month.

### Shell

Logging the values for a specific meter in a file, prepending each line with the current timestamp can be done with some
shell scripting:

    socat open:/dev/cuaU0,raw,echo=0,ispeed=115200,ospeed=115200 - | grep 20d78c1e | awk '{cmd="date +%s"; (cmd | getline date); close(cmd); print date "," $1}' >> /var/log/izar_local.log

Here's how to convert a line to JSON using jq:

    $ echo "1587574231,20d78c16,92.638000,92.277000,m3,2020,04,01,9.0,32,0,0,0,0,0,0,0,0,0,0,0,0,0" | jq --slurp --raw-input --raw-output \
    'split(",") |
        {
        "timestamp": .[0] | tonumber,
        "meter_id": .[1],
        "current_reading": .[2] | tonumber,
        "h0_reading": .[3] | tonumber,
        "measurement_unit": .[4],
        "h0_year": .[5] | tonumber,
        "h0_month": .[6] | tonumber,
        "h0_day": .[7] | tonumber,
        "remaining_battery_life": .[8] | tonumber,
        "radio_interval": .[9] | tonumber,
        "random_generator": .[10] | tonumber,
        "alarms": {
            "general_alarm": .[11] | test("1"),
            "leakage_currently": .[12] | test("1"),
            "leakage_previously": .[13] | test("1"),
            "meter_blocked": .[14] | test("1"),
            "back_flow": .[15] | test("1"),
            "underflow": .[16] | test("1"),
            "overflow": .[17] | test("1"),
            "submarine": .[18] | test("1"),
            "sensor_fraud_currently": .[19] | test("1"),
            "sensor_fraud_previously": .[20] | test("1"),
            "mechanical_fraud_currently": .[21] | test("1"),
            "mechanical_fraud_previously": .[22] | test("1")
        }}'
        
    {
      "timestamp": 1587574231,
      "meter_id": "20d78c16",
      "current_reading": 92.638,
      "h0_reading": 92.277,
      "measurement_unit": "m3",
      "h0_year": 2020,
      "h0_month": 4,
      "h0_day": 1,
      "remaining_battery_life": 9,
      "radio_interval": 32,
      "random_generator": 0,
      "alarms": {
        "general_alarm": false,
        "leakage_currently": false,
        "leakage_previously": false,
        "meter_blocked": false,
        "back_flow": false,
        "underflow": false,
        "overflow": false,
        "submarine": false,
        "sensor_fraud_currently": false,
        "sensor_fraud_previously": false,
        "mechanical_fraud_currently": false,
        "mechanical_fraud_previously": false
      }
    }

## Authors

Erwan Martin <public@fzwte.net>

The logic to decrypt and decode the PRIOS protocol is originally from Jacek Tomasiak.

## Licence

The application code is protected by licence [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause).

The project uses C libraries provided by STMicroelectronics, which are protected by their own licences. 
