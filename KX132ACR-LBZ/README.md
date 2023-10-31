# ROHM 3-Axis Accelerometer IC KX132ACR-LBZ

KX132ACR-LBZ is an industrial grade 3-axis accelerometer from ROHM. The sensor features include variable ODRs, I2C and SPI control, FIFO with watermark IRQ, wake-up & back-to-sleep events, four acceleration ranges (2, 4, 8 and 16g), etc.

## Linux:

The upstream KX022A Linux driver which ROHM has developed in collaboration with the Linux kernel community can with minor modifications be used to drive the KX132ACR-LBZ. Work for supporting this variant in upstream driver 
is currently merged in [upstream Linux IIO git tree's testing branch](https://git.kernel.org/pub/scm/linux/kernel/git/jic23/iio.git/log/?h=testing) where it can be obtained for testing. Support is expected to be released in Linux v6.7.
You can also find the support for KX132ACR-LBZ added on top of Linux v6.5 [here](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/commits/kx132acr-lbz).
