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
* BU27008 RGBC. Under development - [BU27008](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/BU27008)
* BU27010 RGBC/IR + Flicker. Under development - [BU27010](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/BU27010)
* BU27034 Ambient Light Sensori. Under development - [BU27034](https://github.com/RohmSemiconductor/Linux-Kernel-Sensor-Drivers/tree/master/BU27034)
* KX022A accelerometer. Fully upstream - [KX022A](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/KX022A)
* RPR0521 ambient light/proximity. Fully upstream - [RPR0521](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/RPR0521)

### Upstreamed by others
* KX132-1211 - [upstreaming ongoing](https://lore.kernel.org/lkml/cover.1682019544.git.mehdi.djait.k@gmail.com/)
* KXCJK-1013 Accelerometer - [KXCJK-1013](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/KXCJK-1013)
* KXSD9 Accelerometer - [KXSD9](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/KXSD9)
* BH1710 Ambient light sensor - [BH1710](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/BH17XX)
* BH1715 Ambient light sensor - [BH1715](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/BH17XX)
* BH1721 Ambient light sensor - [BH1721](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/BH17XX)
* BH1750 Ambient light sensor - [BH1750](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/BH17XX)
* BH1751 Ambient light sensor - [BH1751](https://github.com/RohmSemiconductor/Linux-Kernel-sensor-Drivers/tree/master/BH17XX)

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
