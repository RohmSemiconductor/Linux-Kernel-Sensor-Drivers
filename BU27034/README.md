# ROHM BU27034 Ambient Light Sensor

ROHM BU27034 is an ambient light sesnor with 3 channels and 3 photo diodes
capable of detecting a very wide range of illuminance. Typical application
is adjusting LCD and backlight power of TVs and mobile phones.

BU27034 supports very large scale of gain settings from 1x up-to 4096x.
Additionally the measurement time can be adjusted.

[Data sheet](https://fscdn.rohm.com/en/products/databook/datasheet/ic/sensor/light/bu27034nuc-e.pdf) is available.

## Linux

The ROHM Finland SWDC is creating an IIO driver in collaboration with the Linux kernel community. Early driver version is merged to the [upstream](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=e52afbd61039e2c5a4e611e23b4aa963d34a4aef) Linux v6.4-rc1. Please note that some fixes are added to the driver during Linux v6.4 development cycle and usable driver should be included in Linux release v6.4.
