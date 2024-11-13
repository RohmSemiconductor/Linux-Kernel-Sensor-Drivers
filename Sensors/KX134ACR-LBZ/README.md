---
permalink: /Sensors/KX134ACR-LBZ/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: KX134ACR-LBZ
compatible: rohm,kx134acr-lbz
expectupstreamed: v6.14
patchlink: https://lore.kernel.org/lkml/cover.1731495937.git.mazziesaccount@gmail.com/
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle
---

# ROHM 3-Axis Accelerometer IC KX134ACR-LBZ

KX134ACR-LBZ is an industrial grade 3-axis accelerometer from ROHM. The sensor features include variable ODRs, I2C and SPI control, FIFO with watermark IRQ, wake-up & back-to-sleep events, four acceleration ranges (-8..+8, -16..+16, -32..+32 and -64..+64 g), etc.

## Resources
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx134acr-lbz-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx134acr-lbz-product)

{% include kx022a_info.md %}

{% include kx022a_dt_example.md %}

{% include quickstart_accel.md %}

