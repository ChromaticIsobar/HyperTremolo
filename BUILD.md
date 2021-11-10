
# Building from Source
To build from source you have to clone the repository including the submodules

```
git clone https://github.com/ChromaticIsobar/HyperTremolo.git
cd HyperTremolo
git submodule update --init --recursive
```

## Linux

### Installing dependencies
Install dependency packages
```
apt-get install libasound2-dev libcurl4-openssl-dev libx11-dev libxinerama-dev libxext-dev libfreetype6-dev libwebkit2gtk-4.0-dev libglu1-mesa-dev libjack-jackd2-dev lv2-dev
```

### Building the projucer
Build the projucer using `make`.
From the repository root directory run
```
cd "JUCE/extras/Projucer/Builds/LinuxMakefile"
make -j4
```

### Exporting the build configuration
Use the projucer to export a build configuration for the plugin.
From the repository root directory run
```
"JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer" --resave *.jucer
```

### Building the plugin
Use the exported build configuration to build the plugin.
From the repository root directory run
```
cd "Builds/LinuxMakefile"
make -j4 CONFIG=Release
```

You will find the build artifacts under the `build` subfolder
| Artifact   | Path                                         |
|------------|----------------------------------------------|
| Standalone | Builds/LinuxMakefile/build/HyperTremolo      |
| VST3       | Builds/LinuxMakefile/build/HyperTremolo.vst3 |

