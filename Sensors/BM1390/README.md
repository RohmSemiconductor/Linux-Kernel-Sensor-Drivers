---
permalink: /Sensors/BM1390/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
bindinglink: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/devicetree/bindings/iio/pressure/rohm,bm1390.yaml
sensorname: BM1390GLV-Z
devicetype: pressure-sensor
compatible: rohm,bm1390glv-z
dtsupply: vdd-supply = <&vdd>
dtintparent: <&gpio1>
dtints: <29 IRQ_TYPE_LEVEL_LOW>
upstreamed: v6.7
---

# ROHM BM1390 Pressure + temperature sensor

ROHM BM1390 is a pressure (and temperature) sensor.
The BM1390GLV-Z can measure pressures ranging from 300 hPa to 1300 hPa with
configurable measurement averaging and internal FIFO. The sensor does also
provide temperature measurements but the temperature compensation to the
pressure values is done by hardware.

Sensor also contains an IIR filter implemented in HW. IIR filter can be
configured to be "weak", "middle" or "strong" filtering.

{% include generic_driver_info.md %}

{% include simple_i2c_dt_example.md %}

{% include upstream_support.md %}
