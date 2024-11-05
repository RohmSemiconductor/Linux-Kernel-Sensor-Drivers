---
permalink: /Sensors/BU27010/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
bindinglink: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/devicetree/bindings/iio/light/rohm,bu27010.yaml
sensorname: BU27010
devicetype: light-sensor
compatible: rohm,bu27010
upstreamed: v6.6
---

# ROHM BU27010 RGB + C/IR + Flicker sensor

The ROHM BU27010 is a sensor with 6 photodiodes (red, green, blue, clear,
IR and flickering detection) with five configurable channels. Red, green
and flickering detection being always available and two out of the rest
three (blue, clear, IR) can be selected to be simultaneously measured.
Typical application is adjusting LCD/OLED backlight of TVs, mobile phones
and tablet PCs.

{% include generic_driver_info.md %}
Support for computing lux values was added in the Linux v6.8.

{% include simple_i2c_dt_example.md %}

{% include upstream_support.md %}
