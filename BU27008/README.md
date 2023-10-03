# ROHM BU27008 RGB + C/IR

The ROHM BU27008 is a sensor with 5 photodiodes (red, green, blue, clear
and IR) with four configurable channels. Red and green being always
available and two out of the rest three (blue, clear, IR) can be
selected to be simultaneously measured. Typical application is adjusting
LCD backlight of TVs, mobile phones and tablet PCs.

## Linux

The ROHM Finland SWDC has created an IIO driver in collaboration with the
Linux kernel community. Driver is merged to the [upstream](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git) Linux v6.5.
Computing lux values in the driver is being worked on.[The patch v1](https://lore.kernel.org/lkml/ZRq4pdDn6N73n7BO@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/) has been sent for review. It is possible this feature will be merged in upstream Linux v6.7.
