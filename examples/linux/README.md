# Linux example for CPP Python TTS library

This will go through how to setup the build environment for the test application on Linux.

## Prerequisites

This project uses CMake as the build tool. Therefore CMake is necessary to be able to build the project.

Python3 needs to be installed. This project has only been tested to work with Python 3.8.10. Others might work but this is not guaranteed.

A version of GCC needs to be installed as build tool. Other compilers might work but has not been tested.

The CppPythonTTS library also needs to be installed and be able to be found by cmake. Follow the three first steps in the [README.md linux section](/README.md#linux).

## Setup Python for the library

Install the requirements for the library if not already done:

```bash
python3 -m pip install -r "<repo-root>/requrements.txt"
```

## Build the project

Go to the build folder (E.g. `<repo-root>/examples/linux/build`). From here run the following command:

```bash
cmake ..
```

The build the project by running.

```bash
make -j$(nproc)
```

## Run the test

When the test is build it can be run from:

```bash
./<repo-root>/examples/linux/build/TTSLinuxExample
```
