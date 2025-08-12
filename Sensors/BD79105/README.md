---
permalink: /Sensors/BD79105/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BD79105
iiodevname: bd79105
compatible: rohm,bd79105
devicetype: adc
spifreq: 66600000
dtsupply: [vcc, vref, vdrive]
patchlink: https://lore.kernel.org/all/cover.1754901948.git.mazziesaccount@gmail.com/
expectupstreamed: v6.18
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BD79105+in%3Atitle
---

# ROHM BD79105 Analog to Digital Converter (ADC)

The ROHM BD79105 ADC is a single-channel, 16-bit ADC which can be read over SPI.

## Linux Driver:

{% include source_upstream_status_new.md %}

The driver supports reading the ADC channels via the Linux IIO subsystem.

{% include subsys_iio.md %}

{% include quickstart_adc.md %}

{% include simple_spi_dt_example.md %}
