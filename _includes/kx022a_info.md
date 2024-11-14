## Linux Driver:

### Source code:
{% if page.upstreamed %}
Upstream Linux {{ page.upstreamed }} onwards
- code files kionix-kx022a* in the [Linux tree's IIO subsystem](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/drivers/iio/accel).
- device-tree bindings in the [Linux device-tree binding documentation](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Documentation/devicetree/bindings/iio/accel/kionix,kx022a.yaml).
{%- else %}
{%- if page.patchlink %}
Being upstreamed. See [Patches]({{ page.patchlink }}).
{%- endif %}
{%- if page.downstreamlink %}
Here is an unmaintained [reference driver]({{ page.downstreamlink }}) which you can try. Please note that this reference driver is provided as is, without a warranty. It is not a "production ready quality", and you are required to do all porting, fixing and testing while writing your driver using it as a starting point.
{%- endif %}
{%- if page.expectupstreamed %}
Upstream driver is currently expected to land in Linux {{ page.expectupstreamed }}
{%- endif %}
{%- endif %}


### Linux subsystem:
IIO (Industrial input/output) 

### General driver information

The driver is called “kionix-kx022a”. It supports multiple ROHM accelerometers. For the sake of simplicity, we refer to it as KX022A here.


The KX022A Linux driver supports obtaining acceleration data via standard Linux IIO interfaces. Both SPI and I2C are supported. One-shot data can be easily obtained using the IIO sysfs ABI. The KX022A driver also supports using the hardware FIFO with watermark interrupt, as well as a software buffer and a data-ready interrupt. Even though the sensor hardware supports data-rates up to 1600 Hz, the sensor driver limits the maximum data rate to 200 Hz to avoid performance issues. Selecting different G-ranges is also supported.

<!-- TODO: Split this include to kx022a_info.md, extauthor.md and kconfig.md -->
{% if page.extauthor %}
  The driver support for this IC has been authored by {{ page.extauthor }}.
{% endif %}

### Configuration options
Please include the following options to the kernel configuration:
- CONFIG_IIO_KX022A_I2C
- CONFIG_IIO_KX022A_SPI

selecting one of the above using the Linux configuration tools like 'menuconfig' will also enable the generic support option:
- CONFIG_IIO_KX022A. - It is not necessary to enable this manually.

