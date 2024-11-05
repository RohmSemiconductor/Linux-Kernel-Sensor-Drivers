### Devicetree example:

```
    #include <dt-bindings/interrupt-controller/irq.h>
    i2c {
        #address-cells = <1>;
        #size-cells = <0>;
        accel@1f {
            compatible = "{{ page.compatible }}";
            reg = <0x1f>;
            interrupt-parent = <&gpio1>;
            interrupts = <29 IRQ_TYPE_LEVEL_LOW>;
            interrupt-names = "INT1";
            io-vdd-supply = <&iovdd>;
            vdd-supply = <&vdd>;
        };
    };
 ```

Where,

- “compatible” must be "{{ page.compatible }}" for the {{ page.sensorname }}.
- “reg” must be the I2C slave address sensor is using.
- “interrupt-parent” must reference the interrupt controller where the interrupt from the sensor is wired 
- “interrupts” must specify the pin in the interrupt controller. How this is done is controller specific and should be found from the interrupt controller’s device tree binding documents.
- “interrupt-names” should be set to INT1 or INT2 depending on the used pin.
- power supplies “io-vdd-supply” and “vdd-supply” should reference the regulator which is supplying power to the sensor. (The Linux kernel may populate a “dummy” supply for the sensor if supply regulators are not modeled in the device-tree).


