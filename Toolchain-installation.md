Setup environment
=================

In order to use this repository, the user shall install the following
resources and utilities.

## Install toolchain prerequisities

Some of the requirements for installing and using this software are (for Ubuntu):
- Update the system (optional step):
```
sudo apt-get update
sudo apt upgrade
```
- Install all the necessary packages:
```
sudo apt-get install git libusb-1.0-0-dev cmake
```

## Toolchain installation

### ARM development toolchain installation

The GNU Embedded Toolchain for ARM is a ready-to-use, open source suite of
tools for C, C++ and Assembly programming targeting ARM Cortex-M and Cortex-R
family of processors.

The PPA we want in this case is from the GCC ARM Embedded Maintainer’s
team.

#### Linux (Ubuntu 18.04 and older)

```
sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
sudo apt-get update
sudo apt-get install gcc-arm-embedded
```

#### Linux (Ubuntu 20.04)

This case is a bit trickier: go to [GNU Arm Embedded Toolchain Downloads](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) website and download the latest archive with the toolchain (as of now, **gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2**).

Unpack the tarball to the install directory, like this:

```
cd ${INSTALL_DIR} && tar xjf gcc-arm-none-eabi-_version_-linux.tar.bz2
```

*${INSTALL_DIR}* is directory where you downloaded your file.
*\_version\_* is version of arm toolchain.

Afterwards, export PATH to the `~/.bashrc`:

```
echo "export PATH="$PATH:${INSTALL_DIR}/gcc-arm-none-eabi-*/bin"" >> ~/.bashrc && source ~/.bashrc
```

Instead of \* write full name of directory (version of gnu arm toolchain).

To check if toolchain was succesfully installed write this in cmd line:

```
arm-none-eabi-gcc --version
```

#### Linux (Fedora)

```
sudo yum install arm-none-eabi-gcc-cs
sudo yum install arm-none-eabi-binutils-cs
sudo yum install arm-none-eabi-newlib
```

#### MacOS

```
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

If it is installed correctly, calling autocomplete:

```
arm-none (press tab twice)
```

should yield the list of installed binaries:

```
arm-none-eabi-addr2line   arm-none-eabi-gcc-ar      arm-none-eabi-nm
arm-none-eabi-ar          arm-none-eabi-gcc-nm      arm-none-eabi-objcopy
arm-none-eabi-as          arm-none-eabi-gcc-ranlib  arm-none-eabi-objdump
arm-none-eabi-c++         arm-none-eabi-gcov        arm-none-eabi-ranlib
arm-none-eabi-c++filt     arm-none-eabi-gcov-dump   arm-none-eabi-readelf
arm-none-eabi-cpp         arm-none-eabi-gcov-tool   arm-none-eabi-size
arm-none-eabi-elfedit     arm-none-eabi-gdb         arm-none-eabi-strings
arm-none-eabi-g++         arm-none-eabi-gprof       arm-none-eabi-strip
arm-none-eabi-gcc         arm-none-eabi-ld
arm-none-eabi-gcc-7.3.1   arm-none-eabi-ld.bfd
```

### QEMU installation

#### Overview

*QEMU* is a generic and open source machine emulator and virtualizer. The most common way is for “system emulation”, where it provides a virtual model of an entire machine (CPU, memory and emulated devices) to run a guest OS.

**xPack QEMU Arm** that will be used in our labs is a cross-platform binary distribution of the public open-source QEMU project.

Go to [xPACK-QEMU-ARM releases](https://github.com/xpack-dev-tools/qemu-arm-xpack/releases/) and download the latest archive with the toolchain and corresponding version (as of now, **xpack-qemu-arm-7.1.0-1-linux-x64.tar.gz**).

#### Unpack

To manually install the *xPack QEMU Arm*, unpack the archive and move it to `~/.local/xPacks/qemu-arm/xpack-qemu-arm-\_version\_`:

!!! Instead of *\_version\_* put your own version, instead of \* write full name of directory (version of xpack-qemu-arm toolchain).

```
mkdir -p ~/.local/xPacks/qemu-arm
cd ~/.local/xPacks/qemu-arm

tar xvf ~/${INSTALL_DIR}/xpack-qemu-arm-_version_.tar.gz
chmod -R -w xpack-qemu-arm-*
```

After export updated PATH to the \~/.bashrc:
```
echo "export PATH="$PATH:~/.local/xPacks/qemu-arm/xpack-qemu-arm-*/bin"" >> ~/.bashrc && source ~/.bashrc
```

#### Test

To check if the manually installed QEMU starts, use something like

```
qemu-system-gnuarmeclipse --version
```

### ST-link installation

Stlink package is the software for the ST-Link programmer that works with many
ST boards. On the discovery boards, the programmer is embedded at the
top. You just need to make sure you plug in your mini-USB cable into the
center-most port that is labeled ST-Link.

#### Linux (Ubuntu)

Сlone the repository st-link:

```
git clone https://github.com/texane/stlink.git
```

Every thing can be built from the top of directory:

```
cd stlink
make
```

To install it run the following **evil** command with root permission:

```
sudo make install
```

For some reason, the supplementary library might not be installed automatically,
thus **Linux** users should move the library manually to the system directory.

```
cd build/Release/lib
sudo cp libstlink.so.1 /usr/lib/
```

#### Linux (Fedora)

```
sudo yum install stlink
```

##### MacOS
```
brew install stlink
```

Dunzo! Welcome aboard!
