# Windows example for CPP Python TTS library

This will go through how to setup the build environment for the test application on Windows.

## Prerequisites

This project uses CMake as the build tool. Therefore CMake is necessary to be able to build the project.

Python3 needs to be installed. This project has only been tested to work with Python 3.10.10. Others might work but this is not guaranteed.

A version of MSVC needs to be installed as build tool. Other compilers might work but has not been tested.

The CppPythonTTS library also needs to be installed and be able to be found by cmake. Follow the three first steps in the [README.md windows section](/README.md#windows).

## Setup the build folder with Python

Create a folder where you want to build the project in. (This could be in `<repo-root>/examples/windows/build`). Also create a Release or Debug folder inside this folder, by running the command `mkdir` in PowerShell. E.g.:

```PowerShell
mkdirs "<repo-root>/examples/windows/build/<Release/Debug>"
```

Then run the following command to install requirements needed for the embedded python setup script to work:

```PowerShell
python -m pip install -r "<repo-root>/embedded/windows/requrements.txt"
```

To run the embedded python setup script run it with the build folder as first argument. E.g.:

```PowerShell
python "<repo-root>/embedded/windows/setup.py" "<repo-root>/examples/windows/build/<Release/Debug>"
```

## Build the project

Go to the build folder (E.g. `<repo-root>/examples/windows/build`, not the Release or Debug folder inside the debug folder.). From here run the following command:

```PowerShell
cmake .. -A x64
```

The build the project by running.

```PowerShell
cmake --build . --config <Release/Debug>
```

## Run the test

When the test is build it can be run from:

```PowerShell
&"<repo-root>/examples/windows/build/<Release/Debug>/TTSWinExample.exe"
```
