# Linux-Kernel-Sensor-Drivers
Linux sensor drivers for ROHM / Kionix sensors

ROHM has been collaborating with the Linux kernel community and sees
the value of open-source. Hence we want to contribute to community
and attempt to upstream the Linux drivers for our components.

## Upstream driver questions
If you have questions related to the Linux community drivers - please
use the linux community mail-lists and maintainer information. Once the
drivers are upstreamed the code changes are no longer in our hands - and the
best experts for those drivers can be found from the commnity. This does not
mean ROHM is out of the game - we have our personnel in Linux driver reviewers/
maintainers - but we don't "own" these components or frameworks anymore. You
get the best possible contacts via the [MAINTAINERS](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/MAINTAINERS) file and [get_maintainer.pl](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/scripts/get_maintainer.pl) script.

## Contents of this repository
We do also occasionally develop something which does not perfectly fit into
the upstream Linux frameworks or policies. This content may include something
which is too product specific or something which requires functionality not
present in upstream kernels.

Sensor drivers are a special example of this. Many of the device vendors use
"legacy" Linux subsystems for their sensors. Typically this is the Linux input
subsystem. The upstream kernel community has not been accepting new sensor
drivers for input subsystem in a while. In order to serve our customers we have
been actively developing drivers for the input subsystem - with the cost of
maintainig them ourself and having no community support. This means most of our
sensor drivers are not in the upstream kernel but are hosted in our own
repositories.

So, this is the place to look for ROHM/Kionix Sensor IC specific Linux drivers/
extensions. Please be aware that these drivers / extensions are provided as
reference implementation only, without warranty, and they may not be actively
developed/maintained.

### Upstream status
* BU27008 RGBC. Fully upstream - [BU27008](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/Sensors/BU27008)
* BU27010 RGBC/IR + Fully upstream - [BU27010](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/Sensors/BU27010)
* BU27034ANUC Ambient Light Sensor. Fully upstream - [BU27034](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/Sensors/BU27034)
* KX022A accelerometer. Fully upstream - [KX022A](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/KX022A)
* KX132ACR-LBZ accelerometer. Fully upstream - [KX132ACR-LBZ](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/KX132ACR-LBZ)
* RPR0521 ambient light/proximity. Fully upstream - [RPR0521](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/RPR0521)
* BM1390 pressure senosr. Fully upstream - [BM1390](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/BM1390)
* BU27034NUC Ambient Light Sensor. Obsoleted - [BU27034](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/Sensors/BU27034)

### Upstreamed by others
* KX132-1211 accelerometer - [KX132-1211](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/KX132-1211)
* KXCJK-1013 Accelerometer - [KXCJK-1013](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/KXCJK-1013)
* KXSD9 Accelerometer - [KXSD9](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/KXSD9)
* BH1710 Ambient light sensor - [BH1710](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/BH17XX)
* BH1715 Ambient light sensor - [BH1715](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/BH17XX)
* BH1721 Ambient light sensor - [BH1721](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/BH17XX)
* BH1750 Ambient light sensor - [BH1750](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/BH17XX)
* BH1751 Ambient light sensor - [BH1751](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/Sensors/BH17XX)
* BH1745 Colour sensor

### Input drivers (not upstream)
Please see the [ROHM/Kionix input drivers](https://github.com/RohmSemiconductor/Linux-Kernel-Input-Drivers) for:
* KX112 accelerometer
* KX122 accelerometer
* KX126 accelerometer + pedometer
* KMX62 accelerometer + magnetometer
* BH1792GLC optical sensor for heart rate monitor
* BH1749 color sensor

### Generic upstream effort
ROHM aims giving back to the community. We know it's two-way road, really. We get
from the community a working platform, bug fixes, porting to new versions,
discussions, education and a chance to impact the direction Linux is heading
to. We value all of this and want to give back innovations and improvements
which may be small or big and aren't always directly relaed to our products.
This helps us all.

### Help!
#### Revceived a .patch file? What should I do with this?

A patch file is describing changes. Typoically used to deliver fixes or new code features as a set of changes that should be applied to some known 'base' release of the code. In case of our driver patch this is usually some Linux release which can be found from cover-letter patch (line like base-commit: 5e99f692d4e32e3250ab18d511894ca797407aec at the bottom), or can be said in email/website/whatever you found the patch from. Most usual way of applying the patch in a Linux is to use 'git' tool. Often the work-flow goes like:
1. Clone the upstream Linux kernel repository (or, if you work with git and linux, add the upstream repository as a remote for your work repository).
2. Fetch the sources and check-out the base-commit (or tag).
3. use git am filename.patch to apply the chanes.

After this you can use the usual git functionality to view the changes, merge them to other branches, diff them, review why some lines were changed and by whom etc.

#### I added the source but it does not seem to build?

The Linux kernel supports huge set of features. In order to keep the binary size sane, not all of the features are enabled at the same time. Also, some features can be mutually exclusive. Selecting what to build is done by the config system. You can add/remove features to build using tools like menuconfig to enable/disable features. It is possible your newly added code is not enabled to be compiled. Usually you can easily track if your file is compiled by opening the Makefile in folder where your code file is, and finding the respective config name from this file. Then you can search for the config using '/' in menuconfig tool. Please note that some configs have dependencies which need to also be enabled before the specific feature can be enabled.
