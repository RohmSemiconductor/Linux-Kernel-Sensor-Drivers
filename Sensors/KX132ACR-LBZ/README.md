---
permalink: /Sensors/KX132ACR-LBZ/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the and not the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: KX132ACR-LBZ
compatible: rohm,kx132acr-lbz
upstreamed: v6.7
---

# ROHM 3-Axis Accelerometer IC KX132ACR-LBZ

KX132ACR-LBZ is an industrial grade 3-axis accelerometer from ROHM. The sensor features include variable ODRs, I2C and SPI control, FIFO with watermark IRQ, wake-up & back-to-sleep events, four acceleration ranges (-2..+2, -4..+4, -8..+8 and -16..+16 g), etc.

{% include kx022a_info.md %}

{% include kx022a_dt_example.md %}

{% include upstream_support.md %}

## Resources
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx132acr-lbz-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx132acr-lbz-product)

### Known issues
We strongly recommend using the latest upstream version of the driver as plenty of fixes are done to the drivers by the Linux kernel community and there is no database for all known issues. Issues spotted by us during development (like adding support for new hardware or improving feature coverage) may have been reported to the [issue tracker](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle).

### Please note:
The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. The fix is integrated to mainline in v6.7-rc7. If you use a kernel older than this, the fix can be found from [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
