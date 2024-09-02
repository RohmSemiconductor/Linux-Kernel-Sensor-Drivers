# ROHM BM1390 Pressure + temperature sensor

ROHM BM1390 is a pressure (and temperature) sensor.
The BM1390GLV-Z can measure pressures ranging from 300 hPa to 1300 hPa with
configurable measurement averaging and internal FIFO. The sensor does also
provide temperature measurements but the temperature compensation to the
pressure values is done by hardware.

Sensor also contains an IIR filter implemented in HW. IIR filter can be
configured to be "weak", "middle" or "strong" filtering.

## Linux

The ROHM Finland SWDC has created an IIO driver in collaboration with the Linux kernel community. Driver is included in Linux kernel v6.7.
