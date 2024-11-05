---
permalink: /Sensors/BU27034/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
bindinglink: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/devicetree/bindings/iio/light/rohm,bu27034anuc.yaml
sensorname: BU27034ANUC
compatible: rohm,bu27034anuc
dtsupply: vdd-supply = <&vdd>
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+bu27034anuc%3A+in%3Atitle+
---

# The BU27034ANUC Ambient Light Sensor

The original BU27034NUC sensor was redesigned. The new sensor is BU27034**A**NUC.

The ROHM BU27034ANUC is a digital Ambient Light Sensor IC with I²C bus interface. This IC can be used for obtaining ambient light data for example for adjusting LCD and backlight power of TV and mobile phone. It is capable of detecting a very wide range of illuminance.

## Resources
 - [Data sheet](https://fscdn.rohm.com/en/products/databook/datasheet/ic/sensor/light/bu27034anuc-e.pdf)

## Linux Driver

The old BU27034NUC upstream driver code was re-written to support the new BU27034ANUC variant.
The patches converting the old driver to work with the new sensor variant were first merged in the Linux v6.12-rc1 in [the Linux tree](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git). Please note that the old upstream device driver is **not** working with the new sensor. Also, the new driver will not work with the old sensor. The device-tree compatible was also changed to reflect this. Changes are expected to be officially relased at Linux v6.12.

### Commits converting support for BU27034NUC to BU27034ANUC
- 3c9edf76791e ("dt-bindings: iio: BU27034 => BU27034ANUC")
- dd77c0ff9936 ("bu27034: ROHM BU27034NUC to BU27034ANUC")
- acfc80c702fc ("bu27034: ROHM BU27034NUC to BU27034ANUC drop data2")
- bb1059815f56 ("bu27034: ROHM BU27034ANUC correct gains and times")
- eb13959e2dea ("bu27034: ROHM BU27034ANUC correct lux calculation")

{% include simple_i2c_dt_example.md %}

{% include upstream_support.md %}

{% include known_issues.md %}

---




Information below concerns the old BU27034NUC and is obsolete. It is here only to show the differencies for those who might have obtained the old device driver.

> ## ROHM BU27034 Ambient Light Sensor
> 
> ROHM BU27034 is an ambient light sesnor with 3 channels and 3 photo diodes
> capable of detecting a very wide range of illuminance. Typical application
> is adjusting LCD and backlight power of TVs and mobile phones.
> 
> BU27034 supports very large scale of gain settings from 1x up-to 4096x.
> Additionally the measurement time can be adjusted.
> 
> [Data sheet](https://fscdn.rohm.com/en/products/databook/datasheet/ic/sensor/light/bu27034nuc-e.pdf) is available.
> 
> ### Linux
> 
> The ROHM Finland SWDC has created an IIO driver in collaboration with the
> Linux kernel community. Driver is merged to the [upstream](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git) Linux v6.4.
