---
permalink: /Sensors/BD79703/
markdownhint: If you're reading the raw-text, you can find the included stuff from the _includes folder. Or, you can head to the pages in https://rohmsemiconductor.github.io/Linux-Kernel-Sensor-Drivers/
sensorname: BD79703
bd7970xnumch: 6
compatible: rohm,bd79703
devicetype: dac
spifreq: 30000000
dtsupply: [vcc, vfs]
upstreamed: v6.14-rc1
datasheet: https://fscdn.rohm.com/en/products/databook/datasheet/ic/data_converter/dac/bd79702fv-lb_bd79703fv-lb-e.pdf
productpage: https://www.rohm.com/products/data-converter/d-a-converters/8bit-d-a/bd79703fv-lb-product
upstreamlink: https://web.git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/dac/rohm-bd79703.c
patchlink: https://lore.kernel.org/lkml/cover.1734608215.git.mazziesaccount@gmail.com/ 
issuelink: https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/issues?q=is%3Aissue+repo%3ALinux-Kernel-Sensor-Drivers+BD79703+in%3Atitle
---

{% include bd7970x.md %}

## Linux Driver:

{% include source_upstream_status_new.md %}
The driver supports setting the output for each of the channels individually.

{% include subsys_iio.md %}

{% include simple_spi_dt_example.md %}

