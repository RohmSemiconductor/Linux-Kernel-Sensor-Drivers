# ROHM/Kionix 3-Axis Accelerometer IC KX022ACR-Z
\

\

\

> [!IMPORTANT]
> These markdown info pages are no longer updated. Here is more up to date [documentation](https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/Sensors/KX022A/)

\

\

\

KX022ACR-Z is a 3-axis accelerometer from ROHM/Kionix. The sensor features include variable ODRs, I2C and SPI control, FIFO/LIFO with watermark IRQ, tap/motion detection, wake-up & back-to-sleep events, four acceleration ranges (-2..+2, -4..+4, -8..+8 and -16..+16 g), etc.

## Linux:

### Source code:
Upstream Linux v6.2 onwards 
- code files kionix-kx022a* in the [Linux tree's IIO subsystem](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/accel).
- device-tree bindings in the [Linux device-tree binding documentation](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/devicetree/bindings/iio/accel/kionix,kx022a.yaml).


### Linux subsystem:
IIO (Industrial input/output) 

### General driver information

The driver is called “kionix-kx022a”. It supports multiple ROHM accelerometers. For the sake of simplicity, we refer to it as KX022A here.


The KX022A Linux driver supports obtaining acceleration data via standard Linux IIO interfaces. Both SPI and I2C are supported. One-shot data can be easily obtained using the IIO sysfs ABI. The KX022A driver also supports using the hardware FIFO with watermark interrupt, as well as a software buffer and a data-ready interrupt. Even though the sensor hardware supports data-rates up to 1600 Hz, the sensor driver limits the maximum data rate to 200 Hz to avoid performance issues. Selecting different G-ranges is also supported.

### Devicetree example:

```
    #include <dt-bindings/interrupt-controller/irq.h>
    i2c {
        #address-cells = <1>;
        #size-cells = <0>;
        accel@1f {
            compatible = "kionix,kx022a";
            reg = <0x1f>;
            interrupt-parent = <&gpio1>;
            interrupts = <29 IRQ_TYPE_LEVEL_LOW>;
            interrupt-names = "INT1";
            io-vdd-supply = <&iovdd>;
            vdd-supply = <&vdd>;
        };
    };
 ```

Where,

- “compatible” must be "kionix,kx022a" for the KX022ACR-Z.
- “reg” must be the I2C slave address sensor is using.
- “interrupt-parent” must reference the interrupt controller where the interrupt from the sensor is wired 
- “interrupts” must specify the pin in the interrupt controller. How this is done is controller specific and should be found from the interrupt controller’s device tree binding documents.
- “interrupt-names” should be set to INT1 or INT2 depending on the used pin.
- power supplies “io-vdd-supply” and “vdd-supply” should reference the regulator which is supplying power to the sensor. (The Linux kernel may populate a “dummy” supply for the sensor if supply regulators are not modeled in the device-tree).


### Configuration options
Please include the following options to the kernel configuration:
- CONFIG_IIO_KX022A_I2C
- CONFIG_IIO_KX022A_SPI

selecting one of the above using the Linux configuration tools like 'menuconfig' will also enable the generic support:
- CONFIG_IIO_KX022A. - It is not necessary to enable this manually.


### Quickstart:

Below is an example of simple raw data captures from a sensor obtained using the IIO.

#### Reading one-shot data from the sensor via the IIO sysfs ABI.

1: Find the right IIO device based on the device name:

```
root@arm:/home/debian# grep -RIn kx022-accel /sys/bus/iio/devices/*/name
/sys/bus/iio/devices/iio:device0/name:1:kx022-accel
/sys/bus/iio/devices/trigger0/name:1:kx022-acceldata-rdy-dev0
root@arm:/home/debian#
```

In the example above:
- /sys/bus/iio/devices/iio:device0 is the kx022a IIO device
- /sys/bus/iio/devices/trigger0 is the kx022a data-ready interrupt trigger0


2. Read one-shot data for all axis from files in_accel_[x,y,z]_raw:

```
root@arm:/home/debian# cat /sys/bus/iio/devices/iio\:device0/in_accel_?_raw
811
-272
16178
```

3. scale values to human-understandable m/s^2 format.

Using Z-axis as example:

Read scale:

```
root@arm:/home/debian# cat /sys/bus/iio/devices/iio\:device0/in_accel_scale
0.000598550
```
 

Apply scale by multiplying the value with scale:

```
root@arm:/home/debian# echo "0.000598550 * 16178" |bc -l
9.683341900
```


#### Launching the measurement and buffering data from the sensor FIFO:


You can also experiment with continuous measurement and buffers. A quick way forward is using the iio_generic_buffer tool which is distributed in the Linux kernel sources “tools/iio” folder.

```
root@arm:/home/debian# /iio_generic_buffer -a -c 10 -g -n kx022-accel
iio device number being used is 0
trigger-less mode selected
Enabling all channels
Enabling: in_accel_y_en
Enabling: in_accel_x_en
Enabling: in_timestamp_en
Enabling: in_accel_z_en
0.384868 -0.217872 9.698306 946686982970308239
0.405218 -0.190937 9.686934 946686982990357739
0.457292 -0.189142 9.695911 946686983010239989
0.466270 -0.183755 9.692321 946686983030281114
0.445321 -0.170587 9.688130 946686983050142530
0.474650 -0.190339 9.689926 946686983070216114
0.487818 -0.193332 9.688729 946686983090065114
0.457891 -0.216675 9.688130 946686983110117655
0.443526 -0.215478 9.672568 946686983129995030
0.457292 -0.162207 9.692321 946686983150037530
Disabling: in_accel_y_en
Disabling: in_accel_x_en
Disabling: in_timestamp_en
Disabling: in_accel_z_en
root@arm:/home/debian#
```

Reading data with the data-ready trigger: 

```
root@arm:/home/debian# /iio_generic_buffer -a -c 10 -n kx022-accel -t kx022-acceldata-rdy-dev0
iio device number being used is 0
iio trigger number being used is 0
Enabling all channels
Enabling: in_accel_y_en
Enabling: in_accel_x_en
Enabling: in_timestamp_en
Enabling: in_accel_z_en
/sys/bus/iio/devices/iio:device0 kx022-acceldata-rdy-dev0
0.530315 -0.169988 9.586377 946687105247108253
0.521936 -0.205901 9.527719 946687105266973003
0.475847 -0.222062 9.564231 946687105287051253
0.465073 -0.190937 9.697707 946687105306900045
0.471059 -0.184952 9.725840 946687105326954587
0.434547 -0.177769 9.677357 946687105346837587
0.449511 -0.174178 9.655808 946687105366879712
0.496198 -0.195726 9.689926 946687105386751628
0.462081 -0.207098 9.687532 946687105406803295
0.415394 -0.188543 9.668977 946687105426684378
Disabling: in_accel_y_en
Disabling: in_accel_x_en
Disabling: in_timestamp_en
Disabling: in_accel_z_en
root@arm:/home/debian#
```

## Resources
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx022acr-z-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx022acr-z-product#productDetail)

## Support and known issues
We strongly recommend using the latest upstream version of the driver as plenty of fixes are done to the drivers by the Linux kernel community, and there is no database for all known issues. Issues spotted by us during development (like adding support for new hardware or improving feature coverage) may be reported to the [issue tracker](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle). Please use the regular upstream maintenance information for support. The maintenance information can be found from the linux [MAINTAINERS](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/MAINTAINERS) file. You can read the MAINTAINERS manually or use the [get_maintainer.pl](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/scripts/get_maintainer.pl)

### Please note:

The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. The fix is integrated to mainline in v6.7-rc7. If you use a kernel older than this, the fix can be found from [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
