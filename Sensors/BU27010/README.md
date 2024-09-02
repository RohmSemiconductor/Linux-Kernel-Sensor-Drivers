# ROHM BU27010 RGB + C/IR + Flicker sensor

The ROHM BU27010 is a sensor with 6 photodiodes (red, green, blue, clear,
IR and flickering detection) with five configurable channels. Red, green
and flickering detection being always available and two out of the rest
three (blue, clear, IR) can be selected to be simultaneously measured.
Typical application is adjusting LCD/OLED backlight of TVs, mobile phones
and tablet PCs.

## Linux

The ROHM Finland SWDC has created an IIO driver in collaboration with the Linux kernel community. Driver is merged to the [upstream Linux](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git) v6.6. Support for computing lux values in the driver is being integrated and is currently in IIO git tree [testing branch](https://git.kernel.org/pub/scm/linux/kernel/git/jic23/iio.git/log/?h=testing). Feature is expected to be merged in upstream Linux v6.7.
