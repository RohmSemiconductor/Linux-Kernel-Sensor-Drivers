---
permalink: /Sensors/KX022A/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: KX022ACR-Z
compatible: kionix,kx022a
upstreamed: v6.2
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle
---

# ROHM/Kionix 3-Axis Accelerometer IC KX022ACR-Z

KX022ACR-Z is a 3-axis accelerometer from ROHM/Kionix. The sensor features include variable ODRs, I2C and SPI control, FIFO/LIFO with watermark IRQ, tap/motion detection, wake-up & back-to-sleep events, four acceleration ranges (-2..+2, -4..+4, -8..+8 and -16..+16 g), etc.

## Resources
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx022acr-z-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx022acr-z-product#productDetail)

## Linux Driver:

### Source code:
{% include source_upstream_status.md %}

{% include subsys_iio.md %}

{% include kx022a_info.md %}

{% include upstream_support.md %}

{% include known_issues.md %}

### Please note:

The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. The fix is integrated to mainline in v6.7-rc7. If you use a kernel older than this, the fix can be found from [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
