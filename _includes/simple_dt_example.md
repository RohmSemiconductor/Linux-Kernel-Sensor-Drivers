### Devicetree example:

```
    i2c {
      #address-cells = <1>;
      #size-cells = <0>;
{% if page.devicetype -%}
      {{ page.devicetype }}@38 {
{%- else -%}
      sensor@38 {
{%- endif %}
        compatible = {{ page.compatible }};
        reg = <0x38>;
      };
    };
```

Where,

- “compatible” must be "{{ page.compatible }}" for the {{ page.sensorname }}.

{% if page.bindinglink -%}
See [the binding document]({{ page.bindinglink }}) for more information.
{% endif %}
