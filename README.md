# C++ Python TTS library

This is a C++ library that pipes python calls to the TTS package of Python and returns the data for use in C++ projects. Right now only the python class `TTS.api.TTS` is implemented, and this library is intended to synthesize text to speech and not training the models from C++.

## Building the library

This project uses CMake as a build system. Since Python in debug mode can be hard to work with, it is recommended to build the project directly in release mode even when developing on the library. That is also why the library is dynamically linked so this library can be in Release mode while having the main program still being build in debug mode.

### Windows

**First:** Configure the project:

- Create a build folder. E.g. called `build` in the root of this repository.

- Open a terminal in the build folder.

- Assuming the build folder is in the root of this repository run:

```powershell
cmake .. -A x64
```

**Second:** Build the project:

- Open a terminal in the build folder.

- From the build folder run:

```powershell
cmake --build . --config Release
```

**Third:** Install the library for use in other projects by running the following command as admin:

```powershell
cmake --build . --config Release --target install
```

**Forth (Optional):** Setup the embedded python environment for the project that uses this library:

**NB:** This embedded python environment will be setup with the same version of python that is currently installed on your system. This library was only tested with Python version 3.10.10.

- Open a terminal in the folder `embedded\windows`.

- Install required libraries with pip:

```powershell
python -m pip install -r requirements.txt
```

- Install the embedded python environment to your build folder:

```powershell
python .\setup.py <path_to_build_folder>
```

### Linux

**First:** Configure the project:

- Create a build folder. E.g. called `build` in the root of this repository.

- Open a terminal in the build folder.

- Assuming the build folder is in the root of this repository run:

```bash
cmake .. -DCMAKE_BUILD_TYPE=release
```

**Second:** Build the project:

- Open a terminal in the build folder.

- From the build folder run:

```bash
make -j$(nproc)
```

**Third** Install the library for use in other projects. This might need root permissions:

```bash
make install
```

**Forth (Optional):** Install required libraries for python in order for the library to work:

**NB:** Right now embedded python environments is not tested on Linux. The library is also only tested with Python version 3.8.10 installed on the Linux system.

- Open a terminal in the root of this repository.

- Install required libraries with pip:

```bash
python -m pip install -r requirements.txt
```

- If the last step fails do to a missing Python package please note that recursive installs, installs all packages at once and will enables them at the same time. Therefore you might need to install the following packages manually first:

```
packaging
numpy
Cython
```

## License

This project is under the Lesser General Public License V2.1. The examples in the folder `examples`, all have separate licenses. Their licenses can be found in the examples' separate folders.
