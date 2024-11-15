---
permalink: /Sensors/KX132-1211/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: KX132-1211
compatible: kionix,kx132-1211
upstreamed: v6.7
extauthor: Mehdi Daijt
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle
---

# ROHM/Kionix 3-Axis Accelerometer IC KX132-1211

KX132-1211 is a 3-axis accelerometer from ROHM/Kionix. The KX132-1211 is in many aspects similar to the [KX022A](../KX022A) but has an improved FIFO and advanced data path engine.

## Resources
- [Reference manuual](https://fscdn.rohm.com/kionix/en/document/KX132-1211-Technical-Reference-Manual-Rev-5.0.pdf)
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx132-1211-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx132-1211-product)

## Linux Driver:

### Source code:
{% include source_upstream_status.md %}

{% include subsys_iio.md %}
 
{% include kx022a_info.md %}

{% include upstream_support.md %}

{% include known_issues.md %}
