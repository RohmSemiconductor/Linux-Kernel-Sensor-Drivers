# The BU27034ANUC

The original BU27034NUC sensor was redesigned. The new sensor is BU27034**A**NUC.

The ROHM BU27034ANUC is a digital Ambient Light Sensor IC with I²C bus interface. This IC can be used for obtaining ambient light data for example for adjusting LCD and backlight power of TV and mobile phone. It is capable of detecting a very wide range of illuminance.

[Data sheet](https://fscdn.rohm.com/en/products/databook/datasheet/ic/sensor/light/bu27034anuc-e.pdf) is available.

## Linux Driver

The old BU27034NUC upstream driver code was re-written to support the new BU27034ANUC variant.
Please note that the old upstream device driver is **not** working with the new sensor. The patches converting the old driver to work with the new sensor variant are currenrly merged in the IIO tree. The new driver will not work with the old sensor. The device-tree compatible was also changed to reflect this. Changes are expected to be relased at Linux v6.12. At the moment the changes can be found from the [Linux IIO subsystem testing tree](https://git.kernel.org/pub/scm/linux/kernel/git/jic23/iio.git/log/?h=testing). Commits:

- 3c9edf76791e ("dt-bindings: iio: BU27034 => BU27034ANUC")
- dd77c0ff9936 ("bu27034: ROHM BU27034NUC to BU27034ANUC")
- acfc80c702fc ("bu27034: ROHM BU27034NUC to BU27034ANUC drop data2")
- bb1059815f56 ("bu27034: ROHM BU27034ANUC correct gains and times")
- eb13959e2dea ("bu27034: ROHM BU27034ANUC correct lux calculation")

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
