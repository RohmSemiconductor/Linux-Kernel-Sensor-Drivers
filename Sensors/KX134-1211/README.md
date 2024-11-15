---
permalink: /Sensors/KX134-1211/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: KX134-1211
compatible: rohm,kx134-1211
expectupstreamed: v6.14
downstreamlink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/kx134-1211-on-iio
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle
---

# ROHM/Kionix 3-Axis Accelerometer IC KX134-1211

KX134-1211 is a 3-axis accelerometer from ROHM/Kionix. The KX134-1211 is similar to the [KX132-1211](../KX132-1211) but has different g-ranges. KX134-1211 supports g-ranges +/-8g, +/-16g, +/-32g and +/-64g.

## Resources
- [Reference manuual](https://fscdn.rohm.com/kionix/en/document/KX134-1211-Technical-Reference-Manual-Rev-5.0.pdf)
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx134-1211-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx134-1211-product)

## Linux Driver:

### Source code:
{% include source_upstream_status.md %}

{% include subsys_iio.md %}

{% include kx022a_info.md %}

