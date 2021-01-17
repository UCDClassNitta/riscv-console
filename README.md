# Getting Started
The simulation environment and toolchain have been setup to run within a Docker container. The directions assume that you have git, Docker, X-11 support and a bash shell on your machine. The code is available on github. Assuming you have ssh setup for git start by cloning the repository with the following command:
```
git clone git@github.com:UCDClassNitta/riscv-console.git
```
You should see something like:
```
Cloning into 'riscv-console'...
remote: Enumerating objects: 176, done.
remote: Counting objects: 100% (176/176), done.
remote: Compressing objects: 100% (111/111), done.
remote: Total 176 (delta 71), reused 163 (delta 63), pack-reused 0
Receiving objects: 100% (176/176), 694.17 KiB | 3.71 MiB/s, done.
Resolving deltas: 100% (71/71), done.
```
Once the repository has been cloned, change directories into the riscv-console and run the console launch script with the command:
```
./rvconsole.sh
```
The script will build the base RISC-V Docker image riscv_base that has the RISC-V build tool chain. This will take an exceptional amount of time, potentially on the order of hours. This build should only have to be done once. Once the base image is built the RISC-V development environment Docker image `riscv_console_dev`. The second image the builds upon the base should build quickly. Once the image is built the script will launch a container name `riscv_console_run`. This will mount the current riscv-console directory into the `/code` directory of the container allowing for source files to be edited on the host machine as well as within the container. Once the container is launched you should see a prompt like:
```
root@fedcba9876543210:/code#
```
The next step is to build the RISC-V example program to test on the simulator. Change directory into `/code/riscv-example` and then run `make`. This should build an example that will put `Hello World!` on the simulator screen and have an `X` that will move based upon the controller direction buttons. You can launch the simulator with the full path using the command:
```
/code/runsim.sh
```
This should build the simulator if it hasn't been built yet and will launch the simulator using X11 assuming everything is installed properly. The script will launch the simulator under the docker user instead of root. This may be necessary for Linux clients. The You should see the simulator window that looks like: 

![](img/console-screenshot.png)

You can now select the example program by hitting the Firmware button and navigating to the `/code/riscv-example/bin` directory. Either the `riscv-console-example`, or `riscv-console-example.strip` file can be loaded. Once the PWR button can be pushed and the `Hello World!` message with an X should appear on the screen. Pressing the direction buttons on the simulator or the associated keys should move the `X` in the particular direction. The simulated CPU can be shut down by hitting the PWR button again. This should dump the status of the registers and memory so may take a while. Once the simulated CPU is shutdown the PWR button will no longer be highlighted. The simulator can be closed by hitting the appropriate close button for the window.
