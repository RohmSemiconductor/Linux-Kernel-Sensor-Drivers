# The BU27034 has changed. Please wait for update

The original BU27034NUC sensor was redesigned. The new sensor is BU27034**A**NUC. We are currently developing Linux device driver for the new BU27034ANUC variant. The upstream device driver mentioned below is **not** working with the new sensor. We are writing a new driver and try to get it upstream during the spring 2024. This page will be updated when the new driver is available. Meanwhile, you can see how the work proceeds by following the linux kerenel development mailing lists.

Information below concerns the old BU27034NUC and is obsolete. It is kept here until the upstream driver is updated to work with the new variant.

---

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
