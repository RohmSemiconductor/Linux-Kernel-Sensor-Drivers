# ROHM 3-Axis Accelerometer IC KX132ACR-LBZ

KX132ACR-LBZ is an industrial grade 3-axis accelerometer from ROHM. The sensor features include variable ODRs, I2C and SPI control, FIFO with watermark IRQ, wake-up & back-to-sleep events, four acceleration ranges (2, 4, 8 and 16g), etc.

## Linux:

The upstream KX022A Linux driver which ROHM has developed in collaboration with the Linux kernel community can with minor modifications be used to drive the KX132ACR-LBZ. Work for supporting this variant was added in upstream driver in the Linux kernel v6.7

Please include following options to the kernel configuration:
- CONFIG_IIO_KX022A_I2C
- CONFIG_IIO_KX022A_SPI

selecting one of the above using the Linux configuration tools like 'menuconfig' will enable also the generic support option:
- CONFIG_IIO_KX022A. - It is not necessary to enable this manually.

## Support and known issues
We strongly recommend using the latest upstream version of the driver as plenty of fixes are done to the drivers by the Linux kernel community and there is no database for all known issues. Issues spotted by us during development (like adding support for new hardware or improving feature coverage) may be reported to the [issue tracker](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle). Please use the regular upstream maintenance information for support. The maintenance information can be found from the linux [MAINTAINERS](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/MAINTAINERS) file. You can read the MAINTAINERS manually or use the [get_maintainer.pl](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/scripts/get_maintainer.pl)

### Please note:

The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. Fix is integrated to mainline in v6.7-rc7. If you use a kernel older than this, the fix can be found from [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
