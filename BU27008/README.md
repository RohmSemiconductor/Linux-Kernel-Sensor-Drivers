# ROHM BU27008 RGB + C/IR

The ROHM BU27008 is a sensor with 5 photodiodes (red, green, blue, clear
and IR) with four configurable channels. Red and green being always
available and two out of the rest three (blue, clear, IR) can be
selected to be simultaneously measured. Typical application is adjusting
LCD backlight of TVs, mobile phones and tablet PCs.

## Linux

The ROHM Finland SWDC is creating an IIO driver in collaboration with the
upstream Linux kernel community. Driver is merged in
[upstream Linux development tree](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/)
at v6.5-rc1 development tag and is expected to be released in Linux v.6.5.
