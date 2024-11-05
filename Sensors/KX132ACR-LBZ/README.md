---
permalink: /Sensors/KX132ACR-LBZ/
---

# ROHM 3-Axis Accelerometer IC KX132ACR-LBZ

KX132ACR-LBZ is an industrial grade 3-axis accelerometer from ROHM. The sensor features include variable ODRs, I2C and SPI control, FIFO with watermark IRQ, wake-up & back-to-sleep events, four acceleration ranges (-2..+2, -4..+4, -8..+8 and -16..+16 g), etc.

## Linux:

### Source code
Upstream Linux v6.7 onwards
- code files kionix-kx022a* in the [Linux tree's IIO subsystem](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/accel).
- device-tree bindings in the [Linux device-tree binding documentation](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/devicetree/bindings/iio/accel/kionix,kx022a.yaml).

<!-- If you're reading the raw-text, you can find the included stuff from the _includes folder.
Or you can head to the and not the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
and see the processed output -->
{% include kx022a_info.md %}

### Devicetree example:

```
    #include <dt-bindings/interrupt-controller/irq.h>
    i2c {
        #address-cells = <1>;
        #size-cells = <0>;
        accel@1f {
            compatible = "rohm,kx132acr-lbz";
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

- “compatible” must be "rohm,kx132acr-lba" for the KX132ACR-LBZ.
- “reg” must be the I2C slave address sensor is using.
- “interrupt-parent” must reference the interrupt controller where the interrupt from the sensor is wired 
- “interrupts” must specify the pin in the interrupt controller. How this is done is controller specific and should be found from the interrupt controller’s device tree binding documents.
- “interrupt-names” should be set to INT1 or INT2 depending on the used pin.
- power supplies “io-vdd-supply” and “vdd-supply” should reference the regulator which is supplying power to the sensor. (The Linux kernel may populate a “dummy” supply for the sensor if supply regulators are not modeled in the device-tree).

<!-- If you're reading the raw-text, you can find the included stuff from the _includes folder.
Or you can head to the and not the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
and see the processed output -->
{% include upstream_support.md %}

## Resources
- [Datasheet](https://fscdn.rohm.com/kionix/en/datasheet/kx132acr-lbz-e.pdf)
- [Product Page](https://www.rohm.com/products/sensors-mems/accelerometer-ics/kx132acr-lbz-product)

### Known issues
We strongly recommend using the latest upstream version of the driver as plenty of fixes are done to the drivers by the Linux kernel community and there is no database for all known issues. Issues spotted by us during development (like adding support for new hardware or improving feature coverage) may have been reported to the [issue tracker](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+KX022A+in%3Atitle).

### Please note:
The driver had a [scaling bug](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues/5) which made it reporting values in micro m/s^2 instead of m/s^2. The fix is integrated to mainline in v6.7-rc7. If you use a kernel older than this, the fix can be found from [here](https://lore.kernel.org/all/ZTEt7NqfDHPOkm8j@dc78bmyyyyyyyyyyyyydt-3.rev.dnainternet.fi/).
