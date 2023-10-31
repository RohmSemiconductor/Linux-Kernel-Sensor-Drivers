# ROHM BU27008 RGB + C/IR

The ROHM BU27008 is a sensor with 5 photodiodes (red, green, blue, clear
and IR) with four configurable channels. Red and green being always
available and two out of the rest three (blue, clear, IR) can be
selected to be simultaneously measured. Typical application is adjusting
LCD backlight of TVs, mobile phones and tablet PCs.

## Linux


The ROHM Finland SWDC has created an IIO driver in collaboration with the
Linux kernel community. Driver is merged to the [upstream Linux](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git) v6.5.
Support for computing lux values in the driver is being integrated and is currently in
IIO git tree [testing branch](https://git.kernel.org/pub/scm/linux/kernel/git/jic23/iio.git/log/?h=testing).
Feature is expected to be merged in upstream Linux v6.7.
