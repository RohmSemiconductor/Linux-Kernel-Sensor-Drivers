---
permalink: /Sensors/BD79112/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BD79112
compatible: rohm,bd79112
devicetype: adc
spifreq: 20000000
dtsupply: [vdd, iovdd]
patchlink: https://lore.kernel.org/all/20250915-bd79112-v5-0-a74e011a0560@gmail.com/
expectupstreamed: v6.18
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BD79112+in%3Atitle
---

# ROHM BD79112 Analog to Digital Converter (ADC)

The ROHM BD79112 ADC is a 32-channel, 12-bit ADC which is controlled over SPI. The ADC inputs can also be used as GPIO.

## Resources
- Product page: TBD
- Datasheet: TBD

## Linux Driver:

{% include source_upstream_status_new.md %}
The driver supports reading the ADC channels via the Linux IIO subsystem. SPI MODE 3 (CPOL=1, CPHA=1) should be used. The channels which are not present in the device tree are exposed as GPIO pins via the Linux GPIO framework.

{% include subsys_iio.md %}

### Devicetree example:

```
    spi {
        adc: adc@0 {
            compatible = "rohm,bd79112";
            reg = <0>;

            vdd-supply = <&dummyreg>; 
            iovdd-supply = <&dummyreg>;

            spi-cpha;
            spi-cpol;
            spi-max-frequency = <20000000>;

            /* Channels which aren't present will be used for GPIO */
            channel@0 {
                reg = <0>;
            };
            channel@1 {
                reg = <1>;
            };
            channel@2 {
                reg = <2>;
            };
            channel@16 {
                reg = <16>;
            };
            channel@20 {
                reg = <20>;
            };
        };
    }
```
