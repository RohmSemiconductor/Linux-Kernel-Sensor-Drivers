---
permalink: /Sensors/BD79100/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BD79100
compatible: rohm,bd79100
devicetype: adc
spifreq: 20000000
dtsupply: [vdd, iovdd]
patchlink: https://lore.kernel.org/all/cover.1755159847.git.mazziesaccount@gmail.com/
expectupstreamed: v6.18
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BD79100+in%3Atitle
---

# ROHM BD79100 Analog to Digital Converter (ADC)

The ROHM BD79100 ADC is a single-channel, 12-bit ADC which is controlled over SPI.

{% include resources.md %}

## Linux Driver:

{% include source_upstream_status_new.md %}
The driver supports reading the ADC channels via the Linux IIO subsystem. SPI MODE 3 (CPOL=1, CPHA=1) should be used.

{% include subsys_iio.md %}

### Devicetree example:

```
    spi {
        adc: adc@0 {
            compatible = "rohm,bd79100";
            reg = <0>;
            vdd-supply = <&dummyreg>; 
            iovdd-supply = <&dummyreg>;
            spi-cpha;
            spi-cpol;
            spi-max-frequency = <20000000>;
        };
    }
```
