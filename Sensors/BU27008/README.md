---
permalink: /Sensors/BU27008/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
bindinglink: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/devicetree/bindings/iio/light/rohm,bu27008.yaml
sensorname: BU27008
devicetype: light-sensor
compatible: rohm,bu27008
upstreamed: v6.5
---

# ROHM BU27008 RGB + C/IR

The ROHM BU27008 is a sensor with 5 photodiodes (red, green, blue, clear
and IR) with four configurable channels. Red and green being always
available and two out of the rest three (blue, clear, IR) can be
selected to be simultaneously measured. Typical application is adjusting
LCD backlight of TVs, mobile phones and tablet PCs.

{% include generic_driver_info.md %}
Support for computing lux values was added in the Linux v6.8

{% include simple_i2c_dt_example.md %}

{% include upstream_support.md %}
