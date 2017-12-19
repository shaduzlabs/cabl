# CABL / Controller ABstraction Layer #
--------------------
[![Build Status](https://travis-ci.org/shaduzlabs/cabl.svg?branch=develop)](https://travis-ci.org/shaduzlabs/cabl) [![Build status](https://ci.appveyor.com/api/projects/status/7pfqx8o4df2oy09t/branch/develop?svg=true)](https://ci.appveyor.com/project/shaduzlabs/cabl/branch/develop) [![Coverage Status](https://coveralls.io/repos/github/shaduzlabs/cabl/badge.svg?branch=develop)](https://coveralls.io/github/shaduzlabs/cabl?branch=develop)

## Supported platforms ##

* OSX
* Linux
* Windows
* Various embedded platforms (in progress)

## Supported controllers ##
* Ableton Push
* Ableton Push2
* Komplete Kontrol S25
* Komplete Kontrol S49
* Komplete Kontrol S61
* Komplete Kontrol S88
* Maschine Jam
* Maschine Mk1
* Maschine Mk2
* Maschine Mikro Mk1
* Maschine Mikro Mk2
* Traktor Kontrol F1 Mk2

## Binaries ##
* [ ![Download](https://api.bintray.com/packages/shaduzlabs/cabl/cabl-osx-64/images/download.svg) ](https://bintray.com/shaduzlabs/cabl/cabl-osx-64/_latestVersion) OSX 64 bit
* [ ![Download](https://api.bintray.com/packages/shaduzlabs/cabl/cabl-linux-64/images/download.svg) ](https://bintray.com/shaduzlabs/cabl/cabl-linux-64/_latestVersion) Linux x86 64 bit
* [ ![Download](https://api.bintray.com/packages/shaduzlabs/cabl/cabl-win-32/images/download.svg) ](https://bintray.com/shaduzlabs/cabl/cabl-win-32/_latestVersion) Windows 32 bit
* [ ![Download](https://api.bintray.com/packages/shaduzlabs/cabl/cabl-win-64/images/download.svg) ](https://bintray.com/shaduzlabs/cabl/cabl-win-32/_latestVersion) Windows 64 bit



## Building from the source ##

### OSX ###
Make sure the following packages/applications are installed:
- [Homebrew ](http://brew.sh/)
- [CMake](http://www.cmake.org/) (>=3.4)
- Xcode (>= 6.4) or GCC (>= 4.9)

Then install libUSB (>=1.0.20), HIDAPI (>= 0.8.0) and RtMIDI (>=2.1.0) if needed:
```
brew install hidapi libusb rtmidi
```
If you want to build the python wrapper, you'll need to install Boost.python:
```
brew install boost --with-python
brew install boost-python
```
**IMPORTANT**: the python wrapper has only been tested with python 2.7.x

And if you want to build the documentation, you'll need to install Doxygen:
```
brew install doxygen
```
Clone this repository locally, initialize the submodules and create a **build** subdirectory:
```
git clone https://github.com/shaduzlabs/cabl.git
cd cabl
git submodule update --init --recursive
mkdir build && cd build
```
Now launch CMake, build and (eventually) install:
```
cmake ..
make
sudo make install
```
Please have a look at the CMake appendix for more CMake options.

### Linux ###
Make sure the following packages/applications are installed:
- [CMake](http://www.cmake.org/) (>=3.4)
- Clang (>= 3.7) or GCC (>= 4.9)

Then install libUSB (>=1.0.20), HIDAPI (>= 0.8.0) and RtMIDI (>=2.1.0) if needed (depending on your linux distribution and dependency manager, the command below might look slightly different):
```
sudo apt-get install libusb-1.0-0-dev libhidapi-dev librtmidi-dev
```
If you want to build the python wrapper, you'll need to install Boost.python:
```
sudo apt-get install libboost-python-dev
```
**IMPORTANT**: the python wrapper has only been tested with python 2.7.x

And if you want to build the documentation, you'll need to install Doxygen:
```
sudo apt-get install doxygen
```
Clone this repository locally, initialize the submodules and create a **build** subdirectory:
```
git clone https://github.com/shaduzlabs/cabl.git
cd cabl
git submodule update --init --recursive
mkdir build && cd build
```
Now launch CMake, build and (eventually) install:
```
cmake ..
make
sudo make install
```
Please have a look at the CMake appendix for more CMake options.

### Windows ###
Make sure the following packages/applications are installed:
- [CMake](http://www.cmake.org/) (>=3.4)
- Visual Studio 2015 or above

If you want to build the documentation, you'll need to install Doxygen.

Clone this repository locally and create a **build** subdirectory:
```
git clone https://github.com/shaduzlabs/cabl.git
cd cabl
git submodule update --init --recursive
mkdir build
cd build
```
Now launch CMake:
```
cmake ..
```
If all worked fine, you shoould end up with several VS project files and a solution (cabl.sln).

Please have a look at the CMake appendix for more CMake options.

#### Python bindings on Windows ####
To build the Python bindings on Windows, you'll need:
* Python 2.7.12
* Boost Python 1.62

If you don't have them installed, the easiest solution is to use [Chocolatey](https://chocolatey.org/). After installing Chocolatey, you can install nuget with
```
choco install nuget.commandline
```
Then, from the directory you want to install boost_python to, run
```
nuget install boost_python-vc140 -Version 1.62.0
```
Download and install Python ([32bit](https://www.python.org/ftp/python/2.7.12/python-2.7.12.msi) or [64bit](https://www.python.org/ftp/python/2.7.12/python-2.7.12.amd64.msi)), then set the following variables:
```
set PYTHON_ROOT=C:\Python27
set MY_LIBS_ROOT=C:\MyBuildDir
```
The first one points to the Python installation directory, the second points to the directory where you installed boost_python.

Now follow the Windows build steps above, except the cmake command, which will have to be:
```
cmake -DPYTHON_LIBRARY=%PYTHON_ROOT%\libs\python27.lib -DBOOST_LIBRARYDIR=%MY_LIBS_ROOT%\boost_python-vc140.1.62.0.0\lib\native\address-model-32\lib -DBOOST_INCLUDEDIR=%MY_LIBS_ROOT%\boost.1.62.0.0\lib\native\incl
ude -DPYTHON_INCLUDE_DIR=%PYTHON_ROOT%\include ..
```


## Appendix ##
### CMake options ###
By default, CMake is configured to build only the static version of the library. You can override this setting using:
- BUILD_STATIC_LIBS=[ON|OFF]
- BUILD_SHARED_LIBS=[ON|OFF]
For example, if you want to build both the shared and the static library, you'll need to write:
```
cmake -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=ON ..
```
On Windows LibUSB, HIDAPI and RtMIDI are downloaded from the respective repositories and built locally in the build subdirectory, so you don't need to have them installed on the system. If for some reason you want the same behavior on Linux or OSX, you can override CABL_FORCE_BUILD_DEPENDENCIES (default is OFF) and set it to ON.
```
cmake -DCABL_FORCE_BUILD_DEPENDENCIES=ON ..
```
