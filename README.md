# BBPower PCB Firmware

Firmware for the BurnerBoard power PCB. Communicates over a VESC-style CAN bus
and exposes a terminal command interface for controlling the board's subsystems.

## CAN commands

There are two layers of CAN commands: user-typed **terminal commands** and the
lower-level **VESC CAN protocol packets**.

### Terminal commands (`src/terminal.cpp`)

User-typed commands are processed by `terminal_process_string()` and arrive over
the CAN terminal channel (`COMM_TERMINAL_CMD`). Each toggles a subsystem and
echoes its new state. Any unrecognized command prints the usage list.

| Command       | Action                                                                                                   |
| ------------- | -------------------------------------------------------------------------------------------------------- |
| `stats`       | Print full status: VESC active/rpm, INA229 vbus/vshunt/current, and ON/OFF for auto, brain, amp, headlight, led_master, led_reg |
| `b1` / `b0`   | Brain regulator ON / OFF                                                                                  |
| `h1` / `h0`   | Headlight ON / OFF                                                                                        |
| `lm1` / `lm0` | LED master ON / OFF                                                                                       |
| `l1` / `l0`   | LEDs (led_reg) ON / OFF                                                                                   |
| `lx`          | LEDs hold — LED lease extension (`powerpcb_set_leds_hold(true)`)                                          |
| `amp1` / `amp0` | Amp ON / OFF                                                                                            |
| `ampx`        | Amp hold — amp lease extension (`powerpcb_set_amp_hold(true)`)                                            |
| `a1` / `a0`   | Auto mode ON / OFF                                                                                        |

### VESC CAN protocol packets (`src/bms_can.cpp`)

Lower-level frame handlers on the CAN bus (VESC-style protocol):

- **Buffer transfer:** `CAN_PACKET_FILL_RX_BUFFER`, `_FILL_RX_BUFFER_LONG`,
  `_PROCESS_RX_BUFFER`, `_PROCESS_SHORT_BUFFER` — reassemble multi-frame command
  payloads.
- **Liveness:** `CAN_PACKET_PING` / `_PONG`, `_SHUTDOWN`.
- **VESC status:** `CAN_PACKET_STATUS` through `_STATUS_5`.
- **COMM commands over CAN:** `COMM_FW_VERSION`, `COMM_BMS_GET_VALUES`,
  `COMM_BMS_GET_BATT_TYPE`, `COMM_TERMINAL_CMD` (routes strings to the terminal
  handler above), `COMM_PING_CAN`, plus bootloader/app-update commands
  (`COMM_JUMP_TO_BOOTLOADER`, `COMM_ERASE_NEW_APP`, `COMM_WRITE_NEW_APP_DATA[_LZO]`,
  `COMM_ERASE_BOOTLOADER`).
- **BMS telemetry the board transmits:** `CAN_PACKET_BURNERBOARD_POWER1/2`,
  `CAN_PACKET_BMS_V_TOT`, `_BMS_I`, `_BMS_AH_WH`, `_BMS_V_CELL`, `_BMS_BAL`,
  `_BMS_TEMPS`, `_BMS_HUM`, `_BMS_SOC_SOH_TEMP_STAT`.
