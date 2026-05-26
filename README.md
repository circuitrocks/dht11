# DHT11 — temperature & humidity sensor

Sample sketch for the **[DHT11 Temperature & Humidity Sensor Module](https://circuit.rocks/products/temperature-and-humidity-dht11-sensor)**, sold by [Circuitrocks](https://circuit.rocks/).

This example reads temperature and humidity from a DHT11 by speaking its single-wire protocol directly — **no external library required** — so you can see exactly how the chip is being talked to.

## What you need

- 1x DHT11 module (3-pin breakout)
- 1x Arduino Uno (or compatible 5V board)
- 3x jumper wires

## Wiring

| DHT11 pin | Arduino Uno pin |
|-----------|-----------------|
| VCC       | 5V              |
| GND       | GND             |
| DATA      | D2              |

The 3-pin breakout sold by Circuitrocks has the pull-up resistor already on the board. If you're using a bare 4-pin DHT11 instead, add a 10 kΩ resistor between DATA and VCC.

For 3.3 V boards (ESP32, Pi Pico, etc.), connect VCC to 3.3 V and use that board's GPIO for DATA — no other code change needed.

## How to use

1. Open `dht11.ino` in the Arduino IDE.
2. Select **Tools → Board → Arduino Uno** and the correct COM port.
3. Click **Upload**.
4. Open **Tools → Serial Monitor**, baud rate **9600**.
5. You'll see humidity (%) and temperature (°C) printed every 2 seconds. Breathe near the sensor — humidity should rise.

## What the values mean

- **Humidity** — relative humidity in percent (20–90 % usable range).
- **Temperature** — temperature in °C (0–50 °C usable range).

DHT11 reports integer values only. Accuracy is ±5 % RH and ±2 °C — fine for room monitoring, not for lab work.

## Notes

- DHT11 can be sampled at most **once per second**. The sketch waits 2 s between reads to be safe.
- The sketch uses the bit-banged protocol directly. For larger projects, the [Adafruit DHT sensor library](https://github.com/adafruit/DHT-sensor-library) is a more robust drop-in choice.
- DHT22 / AM2302 uses the same protocol with finer resolution and a wider range. This code can be adapted for it with small changes to the byte-decoding step.

## Datasheet

[DHT11 datasheet (Mouser, PDF)](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)

## Troubleshooting

- **"Read failed" every time** — check wiring (especially DATA), confirm VCC is 5 V, and that the small power LED on the breakout is lit.
- **Garbled values** — the protocol is timing-sensitive. Adding `Serial.print()` calls inside `readDHT11()` will break it.
- **All zeros** — the sensor needs ~1 s after power-up to stabilize. The sketch already waits in `setup()`.

## License

This sample code is released under the MIT License — see [LICENSE](LICENSE).

## Got stuck?

Drop by [circuit.rocks](https://circuit.rocks/) — happy to help our customers get their modules working.
