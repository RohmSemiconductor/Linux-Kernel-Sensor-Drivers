### Linux subsystem:
IIO (Industrial input/output) 

### General driver information

The driver is called “kionix-kx022a”. It supports multiple ROHM accelerometers. For the sake of simplicity, we refer to it as KX022A here.


The KX022A Linux driver supports obtaining acceleration data via standard Linux IIO interfaces. Both SPI and I2C are supported. One-shot data can be easily obtained using the IIO sysfs ABI. The KX022A driver also supports using the hardware FIFO with watermark interrupt, as well as a software buffer and a data-ready interrupt. Even though the sensor hardware supports data-rates up to 1600 Hz, the sensor driver limits the maximum data rate to 200 Hz to avoid performance issues. Selecting different G-ranges is also supported.


### Configuration options
Please include the following options to the kernel configuration:
- CONFIG_IIO_KX022A_I2C
- CONFIG_IIO_KX022A_SPI

selecting one of the above using the Linux configuration tools like 'menuconfig' will also enable the generic support option:
- CONFIG_IIO_KX022A. - It is not necessary to enable this manually.

