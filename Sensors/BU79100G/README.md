---
permalink: /Sensors/BU79100G/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BU79100G
iiodevname: bu79100g
compatible: rohm,bu79100g
devicetype: adc
spifreq: 20000000
dtsupply: [vcc]
patchlink: https://lore.kernel.org/all/cover.1747123883.git.mazziesaccount@gmail.com/
expectupstreamed: v6.17
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BU79100G+in%3Atitle
---

# ROHM BU79100G Analog to Digital Converter (ADC)

The ROHM BU79100G ADC is a single-channel, 12-bit ADC which can be read over SPI.

## Resources
- [Product page](https://www.rohm.com/products/data-converter/a-d-converters/12bit-a-d/bu79100g-la-product#productDetail)
- [Datasheet](https://fscdn.rohm.com/en/products/databook/datasheet/ic/data_converter/dac/bu79100g-la-e.pdf)

## Linux Driver:

{% include source_upstream_status_new.md %}
The driver supports reading the ADC channels via the Linux IIO subsystem.

{% include subsys_iio.md %}

{% include quickstart_adc.md %}

### Devicetree example:

```
    spi {
        adc: adc@0 {
            compatible = "rohm,bd79100g";
            reg = <0>;
            vcc-supply = <&dummyreg>; 
            spi-max-frequency = <20000000>;
        };
    };
```
