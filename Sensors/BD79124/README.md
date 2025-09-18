---
permalink: /Sensors/BD79124/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BD79124
compatible: rohm,bd79124
devicetype: adc
spifreq: 30000000
dtsupply: [vdd, iovdd]
upstreamlink: https://web.git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/adc/rohm-bd79124.c
upstreamed: v6.16-rc1
patchlink: https://lore.kernel.org/all/cover.1742560649.git.mazziesaccount@gmail.com/
datasheet: https://fscdn.rohm.com/en/products/databook/datasheet/ic/data_converter/dac/bd79124muf-c-e.pdf 
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BD79124+in%3Atitle
---

# ROHM BD79124 Analog to Digital Converter (ADC)

The ROHM BD79124 ADC is a 8-channel, 12-bit ADC which is controlled over I2C. It supports voltage monitoring and voltage out-of-window interrupts. In addition to the ADC the ADC input pins can be configured to be used as general purpose outputs.

{% include resources.md %}

## Linux Driver:

{% include source_upstream_status_new.md %}
The driver supports reading the ADC channels and configuring the voltage limits via Linux IIO subsystem. Over/under voltage will be notified using IIO events.
Rest of the pins are exposed as GPOs via the Linux GPIO subsystem. Pins are configured as ADC or GPO using devicetree.

{% include subsys_iio.md %}

### Devicetree example:

```
    i2c {
        clock-frequency = <100000>;
        status = "okay";

        adc: adc@10 {
            #gpio-cells = <2>;

            compatible = "rohm,bd79124";
            reg = <0x10>;
            interrupt-parent = <&gpio1>;
            interrupts = <29 8>;
            vdd-supply = <&dummyreg>; 
            iovdd-supply = <&dummyreg>;
            gpio-controller;

            /* Channels which aren't listed here will be used as GPOs */
            channel@0 {
                reg = <0>;
            };
            channel@1 {
                reg = <1>;
            };
            channel@2 {
                reg = <2>;
            };
            channel@3 {
                reg = <3>;
            };
            channel@4 {
                reg = <4>;
            };
            channel@5 {
                reg = <5>;
            };
            channel@6 {
                reg = <6>;
            };
        };
    }
```
