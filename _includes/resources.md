{% if page.datasheet or page.productpage -%}
## Resources
{% endif %}
{%- if page.datasheet %}
- [Datasheet]({{ page.datasheet }})
{%- endif %}
{%- if page.productpage %}
- [Product Page]({{ page.productpage }})
{%- endif %}

