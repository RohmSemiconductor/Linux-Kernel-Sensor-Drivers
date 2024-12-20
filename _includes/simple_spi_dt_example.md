### Devicetree example:

```
    spi {
      #address-cells = <1>;
      #size-cells = <0>;
{%- if page.devicetype %}
      {{ page.devicetype }}@0 {
{%- else %}
      sensor@0 {
{%- endif %}
        compatible = {{ page.compatible }};
        reg = <0>;
{%- if page.spifreq %}
        spi-max-frequency = <{{ page.spifreq }}>;
{%- endif %}
{%- if page.dtsupply %}
    {%- for s in page.dtsupply %}
        {{ s }}-supply = <&{{ s }}>;
    {% endfor -%}
{%- endif %}
{%- if page.dtintparent %}
        interrupt-parent =  {{ page.dtintparent }};
{%- endif %}
{%- if page.dtints %}
        interrupts = {{ page.dtints }};
{%- endif %}
      };
    };
```

Where,

- "compatible" must be "{{ page.compatible }}" for the {{ page.sensorname }}.
- "reg" must be the number of the chip select device is connected.
{%- if page.spifreq %}
 - spi-max-frequency must be {{ page.spifreq }} Hz
{%- endif %}
{%- if page.dtsupply %}
    {%- for s in page.dtsupply %}
- "{{ s }}-supply" refers to a regulator supplying "{{ s }}" power to the device.
    {%- endfor %}
{%- endif %}
{%- if page.dtintparent %}
- "interrupt-parent" refers to the interrupt controlled IRQ is wired to.
{%- endif %}
{%- if page.dtints %}
- "interrupts" specifies the pin in parent interrupt controller. See interrupt controller binding document for details.
{% endif %}
{% if page.bindinglink %}
See [the binding document]({{ page.bindinglink }}) for more information.
{% endif %}
