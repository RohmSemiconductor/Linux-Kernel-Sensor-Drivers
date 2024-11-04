### Quickstart:

Below is an example of simple raw data captures from a sensor obtained using the IIO.

#### Reading one-shot data from the sensor via the IIO sysfs ABI.

1: Find the right IIO device based on the device name.

```
root@arm:/home/debian# grep -RIn kx022-accel /sys/bus/iio/devices/*/name
/sys/bus/iio/devices/iio:device0/name:1:kx022-accel
/sys/bus/iio/devices/trigger0/name:1:kx022-acceldata-rdy-dev0
root@arm:/home/debian#
```
In the example above:
- 'kx022-accel' is used as sensor name in 'grep' command. You should use the name matching your sensor.
- /sys/bus/iio/devices/iio:device0 is the kx022a IIO device
- /sys/bus/iio/devices/trigger0 is the kx022a data-ready interrupt trigger0


2. Read one-shot data for all axis from files in_accel_[x,y,z]_raw:

```
root@arm:/home/debian# cat /sys/bus/iio/devices/iio\:device0/in_accel_?_raw
811
-272
16178
```

3. scale values to human-understandable m/s^2 format.

Using Z-axis as example:

Read scale:

```
root@arm:/home/debian# cat /sys/bus/iio/devices/iio\:device0/in_accel_scale
0.000598550
```
 

Apply scale by multiplying the value with scale:

```
root@arm:/home/debian# echo "0.000598550 * 16178" |bc -l
9.683341900
```


#### Launching the measurement and buffering data from the sensor FIFO:


You can also experiment with continuous measurement and buffers. A quick way forward is using the iio_generic_buffer tool which is distributed in the Linux kernel sources “tools/iio” folder. Note that examples use sensor and trigger names for kx022-accel. Names you should use depend on the actual sensor model.

```
root@arm:/home/debian# /iio_generic_buffer -a -c 10 -g -n kx022-accel
iio device number being used is 0
trigger-less mode selected
Enabling all channels
Enabling: in_accel_y_en
Enabling: in_accel_x_en
Enabling: in_timestamp_en
Enabling: in_accel_z_en
0.384868 -0.217872 9.698306 946686982970308239
0.405218 -0.190937 9.686934 946686982990357739
0.457292 -0.189142 9.695911 946686983010239989
0.466270 -0.183755 9.692321 946686983030281114
0.445321 -0.170587 9.688130 946686983050142530
0.474650 -0.190339 9.689926 946686983070216114
0.487818 -0.193332 9.688729 946686983090065114
0.457891 -0.216675 9.688130 946686983110117655
0.443526 -0.215478 9.672568 946686983129995030
0.457292 -0.162207 9.692321 946686983150037530
Disabling: in_accel_y_en
Disabling: in_accel_x_en
Disabling: in_timestamp_en
Disabling: in_accel_z_en
root@arm:/home/debian#
```

Reading data with the data-ready trigger: 

```
root@arm:/home/debian# /iio_generic_buffer -a -c 10 -n kx022-accel -t kx022-acceldata-rdy-dev0
iio device number being used is 0
iio trigger number being used is 0
Enabling all channels
Enabling: in_accel_y_en
Enabling: in_accel_x_en
Enabling: in_timestamp_en
Enabling: in_accel_z_en
/sys/bus/iio/devices/iio:device0 kx022-acceldata-rdy-dev0
0.530315 -0.169988 9.586377 946687105247108253
0.521936 -0.205901 9.527719 946687105266973003
0.475847 -0.222062 9.564231 946687105287051253
0.465073 -0.190937 9.697707 946687105306900045
0.471059 -0.184952 9.725840 946687105326954587
0.434547 -0.177769 9.677357 946687105346837587
0.449511 -0.174178 9.655808 946687105366879712
0.496198 -0.195726 9.689926 946687105386751628
0.462081 -0.207098 9.687532 946687105406803295
0.415394 -0.188543 9.668977 946687105426684378
Disabling: in_accel_y_en
Disabling: in_accel_x_en
Disabling: in_timestamp_en
Disabling: in_accel_z_en
root@arm:/home/debian#
```


