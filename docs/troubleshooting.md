# Troubleshooting
This troubleshooting guide has some of the common issues that people have run into.

## Common issues

* [When I run ./runsim.sh I get "Unable to init server: Could not connect: Connection refused"](#x-windows-blocking-connection)
* [When I run ./runconsole.sh during the build I get "ERROR \[internal\] load metadata for docker.io/library/riscv_base:lates"](#buildkit-enabled-causing-problems)
* [When I run ./runconsole.sh during the build I get certificate error during "git clone --recursive https://github.com/riscv/riscv-gnu-toolchain" step](#outdated-certificates)
* [When I run ./runsim.sh for first time I get "./include/GraphicFactoryCairo.h:5:10 fatal error: 'gtk/gtk.h' file not found"](#missing-gtk3-library)


## X-Windows Blocking Connection
When running the `./runsim.sh` or attempting to launch the simulator directly you are seeing something like:
```
Unable to init server: Could not connect: Connection refused

(riscv-console-sim:34): Gtk-WARNING **: 05:36:51.740: cannot open display: :0
```
There are several reasons for this error. 

### DISPLAY Environmental Variable Incorrect
The `DISPLAY` environmental variable may not be set correctly. 
Depending upon the host OS the value that `DISPLAY` should be set to will vary. 
You can get the `DISPLAY` environmental variable through running the command `env | grep DISPLAY`.
Here are what the `DISPLAY` values should be depending upon the host OS.
* OS-X (`DISPLAY=host.docker.internal:0`)
* Windows(`DISPLAY=YOUR_IP_ADDRESS:0` where `YOUR_IP_ADDRESS` is your current IPv4 address, sometimes this ends up with multiples due to the PowerShell script, but should only have one)
* Linux (`DISPLAY=:0`)

You can update the `DISPLAY` environmental variable with the command:
```export DISPLAY="THE_CORRECT_SETTING"```
Where `THE_CORRECT_SETTING` is the whatever the correct setting value should be.

### X-Server Blocking Incoming Connections
If the `DISPLAY` variable is setup correctly, it could be an issue of the X-Server blocking the connection. If you are on OS-X or Windows first make sure you have the X-Server installed (likely XQuartz for OS-X and VcXsrv for Windows). 
#### OS-X XQuartz
XQuartz may be blocking the connection. You may be able to fix this by going to `XQuartz` &rarr; `Preferences` &rarr; `Security` and making sure that `Allow connections from network clients` is selected.
#### Windows VcXsrv
First make sure that in XLaunch that the `Disable access control` is selected in the `Extra settings` as is outlined in the [PowerShell Getting Started](./powershell.md). If that is already set, the next thing to check is the Windows Firewall, make sure that the firewall is allowing VcXsrv to accept incoming connections.

## Buildkit Enabled Causing Problems
When running the `./runconsole.sh` during the build an error occurs that looks something like:
```
=> ERROR [internal] load metadata for docker.io/library/riscv_base:lates 1.3s
```
This issue appears to be related to the BuildKit being enabled. It appears to be fixed by disabling BuildKit. Try following the description at [https://forums.docker.com/t/strange-docker-output-or-help-me-please-im-very-noob/100788/2](https://forums.docker.com/t/strange-docker-output-or-help-me-please-im-very-noob/100788/2) 

## Outdated Certificates
When running the `./runconsole.sh` during the build an error occurs that looks something like:
```
fatal: unable to access 'https://git.qemu.org/git/qemu.git/': server certificate verification failed. CAfile: none CRLfile: none
fatal: clone of 'https://git.qemu.org/git/qemu.git' into submodule path '/home/bill/Desktop/project2/build/riscv-gnu-toolchain/qemu' failed
Failed to clone 'qemu'. Retry scheduled
```
Depending upon the OS some have had success by either reinstalling the certificates, or by disabling git from requiring certificates. Here are several links that some have found helpful:

* [https://fabianlee.org/2019/01/28/git-client-error-server-certificate-verification-failed/](https://fabianlee.org/2019/01/28/git-client-error-server-certificate-verification-failed/)
* [https://stackoverflow.com/questions/35821245/github-server-certificate-verification-failed](https://stackoverflow.com/questions/35821245/github-server-certificate-verification-failed)
* [https://stackoverflow.com/questions/21181231/server-certificate-verification-failed-cafile-etc-ssl-certs-ca-certificates-c/22477284#22477284](https://stackoverflow.com/questions/21181231/server-certificate-verification-failed-cafile-etc-ssl-certs-ca-certificates-c/22477284#22477284)

## Missing GTK3 Library
When running the `./runsim.sh` or attempting to launch the simulator directly you are seeing something like:
```
./include/GraphicFactoryCairo.h:5:10 fatal error: 'gtk/gtk.h' file not found
#include <gtk/gtk.h>
         ^~~~~~~~~~~
```
The problem is that the GTK+3 development library is not installed on the system you are trying to run it on. Depending upon the OS you will want to install the GTK+3 package. 
### Ubuntu
To install the package you will want to run the following commands:
```
sudo apt-get update
sudo apt-get install libgtk-3-dev
```
### OS-X Macports or Homebrew
Using Macports or Homebrew you will want to install the `gtk3` package. For Macports do:
```
sudo port selfupdate
sudo port install gtk3
``` 
For Homebrew do:
```
brew update
brew install gtk3
```