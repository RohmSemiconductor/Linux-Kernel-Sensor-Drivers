### Quickstart:

Below is an example of simple data capture from an ADC, obtained using the IIO.

#### Reading one-shot data from the sensor via the IIO sysfs ABI.

1: Find the right IIO device based on the device name.

```
root@arm:/home/debian# grep -RIl {{ page.iiodevname }} /sys/bus/iio/devices/*/name
/sys/bus/iio/devices/iio:device0/name
root@arm:/home/debian#
```
In the example above:
- /sys/bus/iio/devices/iio:device0 is our IIO device

2: Read the ADC scale which is a multiplier to convert the read raw values to millivolts:

```
root@arm:~# cat /sys/bus/iio/devices/iio\:device0/in_voltage_scale 
0.805664062
root@arm:~#
```


3: Read one-shot data for a channel 0 from :

```
root@arm:~# cat /sys/bus/iio/devices/iio\:device0/in_voltage0_raw 
2230
root@arm:~#
```

4: scale values to milli Volts:

Apply scale by multiplying the value with scale:

```
root@arm:~# echo "2230 * 0.805664062" |bc -l
1796.630858260
root@arm:~#
```
