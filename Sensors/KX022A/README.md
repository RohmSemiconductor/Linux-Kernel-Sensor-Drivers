---
permalink: /Sensors/KX022A/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the and not the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: KX022ACR-Z
compatible: kionix,kx022a
upstreamed: v6.2
---

# ROHM/Kionix 3-Axis Accelerometer IC KX022ACR-Z

KX022ACR-Z is a 3-axis accelerometer from ROHM/Kionix. The sensor features include variable ODRs, I2C and SPI control, FIFO/LIFO with watermark IRQ, tap/motion detection, wake-up & back-to-sleep events, four acceleration ranges (-2..+2, -4..+4, -8..+8 and -16..+16 g), etc.

{% include kx022a_info.md %}

{% include kx022a_dt_example.md %}

{% include quickstart_accel.md %}

## Resources
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx022acr-z-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx022acr-z-product#productDetail)

{% include upstream_support.md %}

### Known issues
We strongly recommend using the latest upstream version of the driver as plenty of fixes are done to the drivers by the Linux kernel community and there is no database for all known issues. Issues spotted by us during development (like adding support for new hardware or improving feature coverage) may have been reported to the [issue tracker](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle).

### Please note:

The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. The fix is integrated to mainline in v6.7-rc7. If you use a kernel older than this, the fix can be found from [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
