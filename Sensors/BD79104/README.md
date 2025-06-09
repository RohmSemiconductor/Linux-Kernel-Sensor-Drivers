---
permalink: /Sensors/BD79104/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BD79104
compatible: rohm,bd79104
devicetype: adc
spifreq: 20000000
dtsupply: [vdd, iovdd]
upstreamlink: https://web.git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/adc/ti-adc128s052.c
upstreamed: v6.16-rc1
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BD79104+in%3Atitle
---

# ROHM BD79104 Analog to Digital Converter (ADC)

The ROHM BD79104 ADC is a 8-channel, 12-bit ADC which is controlled over SPI.

## Resources
- [Product page](https://www.rohm.com/products/data-converter/a-d-converters/12bit-a-d/bd79104fv-la-product)
- [Datasheet](https://fscdn.rohm.com/en/products/databook/datasheet/ic/data_converter/dac/bd79104fv-la-e.pdf)

## Linux Driver:

{% include source_upstream_status_new.md %}
The driver supports reading the ADC channels via the Linux IIO subsystem. SPI MODE 3 (CPOL=1, CPHA=1) should be used.

{% include subsys_iio.md %}

### Devicetree example:

```
    spi {
        adc: adc@0 {
            compatible = "rohm,bd79104";
            reg = <0>;
            vdd-supply = <&dummyreg>; 
            iovdd-supply = <&dummyreg>;
            spi-cpha;
            spi-cpol;
            spi-max-frequency = <20000000>;
        };
    }
```
