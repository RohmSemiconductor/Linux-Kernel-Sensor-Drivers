# ROHM/Kionix 3-Axis Accelerometer IC KX112

KX022A is a 3-axis accelerometer from ROHM/Kionix. The sensor features include variable ODRs, I2C and SPI control, FIFO/LIFO with watermark IRQ, wake-up & back-to-sleep events, four acceleration ranges (2, 4, 8 and 16g), etc. The KX022A variants intended for consumer applications also include tap and motion detection.

## Linux:

The ROHM Finland SWDC has created an IIO driver in collaboration with the Linux kernel community. Driver is included in Linux kernel v6.2.

Also, a new compatible string "kx022acr-lbz" was added for the industrial grade accelerometer KX022ACR-LBZ. This IC does not contain the TAP / TILT engines. Work for supporting this variant in upstream driver will be done but in order to avoid unnecessary conflicts with KX132 upstream work sending the support to upstream is postponed. Meanwhile you can find the support for KX022ACR-LBZ added on top of Linux v6.5 [here](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/commits/kx022acr-lbz).
