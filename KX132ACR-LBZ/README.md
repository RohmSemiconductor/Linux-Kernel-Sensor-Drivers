# ROHM 3-Axis Accelerometer IC KX132ACR-LBZ

KX132ACR-LBZ is an industrial grade 3-axis accelerometer from ROHM/Kionix. The sensor features include variable ODRs, I2C and SPI control, FIFO with watermark IRQ, wake-up & back-to-sleep events, four acceleration ranges (2, 4, 8 and 16g), etc.

## Linux:

The upstream KX022A Linux driver which ROHM has developed in collaboration with the Linux kernel community can with minor modifications be used to drive the KX132ACR-LBZ. Work for supporting this variant in upstream driver will be done but in order to avoid unnecessary conflicts with [KX132-1211](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/KX132-1211) upstream work, sending the support to upstream is postponed. Meanwhile you can find the support for KX132ACR-LBZ added on top of Linux v6.5 [here](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/commits/kx132acr-lbz).
