# ROHM/Kionix 3-Axis Accelerometer IC KX022A

KX022A is a 3-axis accelerometer from ROHM/Kionix. The sensor features include variable ODRs, I2C and SPI control, FIFO/LIFO with watermark IRQ, tap/motion detection, wake-up & back-to-sleep events, four acceleration ranges (2, 4, 8 and 16g), etc.

## Linux:

The ROHM Finland SWDC has created an IIO driver in collaboration with the Linux kernel community. Driver is included in Linux kernel v6.2.

### Please note:

The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. Fix is currently being integrated upstream but if you have a version with the bug please find a fix [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
