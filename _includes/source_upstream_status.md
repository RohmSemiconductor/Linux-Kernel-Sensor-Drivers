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


