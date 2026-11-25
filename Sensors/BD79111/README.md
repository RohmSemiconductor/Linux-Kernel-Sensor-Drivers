---
permalink: /Sensors/BD79111/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BD79111
compatible: rohm,bd79111
devicetype: adc
productpage: https://www.rohm.com/products/data-converter/a-d-converters/12bit-a-d/bd79111muf-c-product
datasheet: https://fscdn.rohm.com/en/products/databook/datasheet/ic/data_converter/dac/bd79111muf-c-e.pdf
spifreq: 10000000
dtsupply: [vdd, iovdd]
patchlink: https://lore.kernel.org/all/b3dd43b931928ea59cfe8ce6b42168e2b3537b57.1795421473.git.mazziesaccount@gmail.com/
upstreamlink: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/adc/rohm-bd79112.c
upstreamdriverexisted: true
expectupstreamed: v7.5
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BD79112+in%3Atitle
---

# ROHM BD79111 Analog to Digital Converter (ADC) / GPIO controller

The ROHM BD79111 ADC is a 16-channel, 12-bit ADC which is controlled over SPI. The ADC inputs can also be used as GPIO.

{% include resources.md %}

## Linux Driver:

{% include source_upstream_status_new.md %}
The driver supports reading the ADC channels via the Linux IIO subsystem. SPI MODE 3 (CPOL=1, CPHA=1) should be used. The channels which are not present in the device tree are exposed as GPIO pins via the Linux GPIO framework.

{% include subsys_iio.md %}

### Devicetree example:

```
    spi {
        adc: adc@0 {
            compatible = "rohm,bd79111";
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
            channel@14 {
                reg = <14>;
            };
            channel@15 {
                reg = <15>;
            };
        };
    }
```
