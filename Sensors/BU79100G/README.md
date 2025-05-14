---
permalink: /Sensors/BU79100G/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BU79100G
iiodevname: ads7866
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

The ROHM BU79100G can be operated using the upstream ad7476 driver. Support for BU79100G like devices should be found from the Linux v5.1-rc1 onwards.

NOTE: The device-tree must use ti,ads7866 as a fallback compatible for the BU79100G because some of the device drivers may use ti,ads7866 code-branch to avoid code duplication. Please, see the device-tree example below for the details.

The driver supports reading the ADC channels via the Linux IIO subsystem.

{% include subsys_iio.md %}

{% include quickstart_adc.md %}

### Devicetree example:

```
    spi {
        adc: adc@0 {
            compatible = "rohm,bd79100g", "ti,ads7866";
            reg = <0>;
            vcc-supply = <&dummyreg>; 
            spi-max-frequency = <20000000>;
        };
    };
```

Please note the compatible:
```
compatible = "rohm,bd79100g", "ti,ads7866";
```
Using the ti,ads7866 as a fallback ensures that the sensor is working even when driver is optimized to avoid code duplication. (Device-tree validation is failing until [this patch](https://lore.kernel.org/all/4907a096eee1f54afae834213cf721b551382d4e.1747203712.git.mazziesaccount@gmail.com/) gets merged).

