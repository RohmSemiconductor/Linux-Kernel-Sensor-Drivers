# ROHM/Kionix 3-Axis Accelerometer IC KX022ACR-Z

KX022ACR-Z is a 3-axis accelerometer from ROHM/Kionix. The sensor features include variable ODRs, I2C and SPI control, FIFO/LIFO with watermark IRQ, tap/motion detection, wake-up & back-to-sleep events, four acceleration ranges (2, 4, 8 and 16g), etc.

## Linux:

The ROHM Finland SWDC has created an IIO driver in collaboration with the Linux kernel community. Driver is included in [Linux kernel](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/accel/kionix-kx022a.c) v6.2 onwards.

## Configuration options

Please include following options to the kernel configuration:
- CONFIG_IIO_KX022A_I2C
- CONFIG_IIO_KX022A_SPI

selecting one of the above using the Linux configuration tools like 'menuconfig' will enable also the generic support for the KX022A:
- CONFIG_IIO_KX022A. - It should not be necessary to enable this manually.

## Support and known issues
We strongly recommend using the latest upstream version of the driver as plenty of fixes are done to the drivers by the Linux kernel community and there is no database for all known issues. Issues spotted during late development (like adding support for new hardware or improving feature coverage) may be reported to the [issue tracker](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle). Please use the regular upstream maintenance information for support. The maintenance information can be found from the linux [MAINTAINERS](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/MAINTAINERS) file. You can read the MAINTAINERS manually or use the [get_maintainer.pl](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/scripts/get_maintainer.pl)

### Please note:

The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. Fix is currently being integrated upstream but if you have a version with the bug please find a fix [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
