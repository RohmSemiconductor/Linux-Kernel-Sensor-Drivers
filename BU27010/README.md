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
[upstream IIO development tree](https://git.kernel.org/pub/scm/linux/kernel/git/jic23/iio.git/)
and is expected to be released in Linux v.6.6
