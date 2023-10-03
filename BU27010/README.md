# ROHM BU27010 RGB + C/IR + Flicker sensor

The ROHM BU27010 is a sensor with 6 photodiodes (red, green, blue, clear,
IR and flickering detection) with five configurable channels. Red, green
and flickering detection being always available and two out of the rest
three (blue, clear, IR) can be selected to be simultaneously measured.
Typical application is adjusting LCD/OLED backlight of TVs, mobile phones
and tablet PCs.

## Linux

The ROHM Finland SWDC is creating an IIO driver in collaboration with the
upstream Linux kernel community. Driver is merged for testing in
[upstream Linux tree](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/light/rohm-bu27008.c). First tag containing the driver is v6.6-rc1.
Driver is expected to be officially released in Linux v6.6
Computing lux values in the driver is being worked on.[The patch v1](https://lore.kernel.org/lkml/ZRq4pdDn6N73n7BO@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/) has been sent for review. It is possible this feature will be merged in upstream Linux v6.7.
