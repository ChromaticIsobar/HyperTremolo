
# Building from Source
To build from source you have to clone the repository including the submodules

```bash
git clone https://github.com/ChromaticIsobar/HyperTremolo.git
cd HyperTremolo
git submodule update --init --recursive
```

## Linux

### Installing dependencies
Install dependency packages
```bash
apt-get install libasound2-dev libcurl4-openssl-dev libx11-dev libxinerama-dev libxext-dev libfreetype6-dev libwebkit2gtk-4.0-dev libglu1-mesa-dev libjack-jackd2-dev lv2-dev
```

### Building the projucer
Build the projucer using `make`.
From the repository root directory run
```bash
cd "JUCE/extras/Projucer/Builds/LinuxMakefile"
make -j4
```

### Exporting the build configuration
Use the projucer to export a build configuration for the plugin.
From the repository root directory run
```bash
"JUCE/extras/Projucer/Builds/LinuxMakefile/build/Projucer" --resave *.jucer
```

### Building the plugin
Use the exported build configuration to build the plugin.
From the repository root directory run
```bash
cd "Builds/LinuxMakefile"
make -j4 CONFIG=Release
```

You will find the build artifacts under the `build` subfolder
| Artifact   | Path                                         |
|------------|----------------------------------------------|
| Standalone | Builds/LinuxMakefile/build/HyperTremolo      |
| VST3       | Builds/LinuxMakefile/build/HyperTremolo.vst3 |


## MacOS
For building under MacOS you'll need `xcodebuild`

### Building the projucer
Build the projucer using `xcodebuild`.
From the repository root directory run
```bash
cd "JUCE/extras/Projucer/Builds/MacOSX"
xcodebuild -project *.xcodeproj -parallelizeTargets -alltargets | xcpretty
```

### Exporting the build configuration
Use the projucer to export a build configuration for the plugin.
From the repository root directory run
```bash
"JUCE/extras/Projucer/Builds/MacOSX/build/Release/Projucer.app/Contents/MacOS/Projucer" --resave *.jucer
```

### Building the plugin
Use the exported build configuration to build the plugin.
From the repository root directory run
```bash
cd "Builds/MacOSX"
xcodebuild -project *.xcodeproj -parallelizeTargets -alltargets -configuration Release | xcpretty
```

You will find the build artifacts under the `build` subfolder
| Artifact   | Path                                               |
|------------|----------------------------------------------------|
| Standalone | Builds/MacOSX/build/Release/HyperTremolo.app       |
| VST3       | Builds/MacOSX/build/Release/HyperTremolo.vst3      |
| AU         | Builds/MacOSX/build/Release/HyperTremolo.component |


## Windows
For building under Windows you'll need MSBuild. In the following instructions, substitute `<msbuild>` with the path to your MSBuild executable

### Building the projucer
Build the projucer using MSBuild.
From the repository root directory run
```bash
cd "JUCE/extras/Projucer/Builds/VisualStudio2019"
<msbuild> -m:4 *.sln /property:Configuration=Release
```

### Exporting the build configuration
Use the projucer to export a build configuration for the plugin.
From the repository root directory run
```bash
"JUCE/extras/Projucer/Builds/VisualStudio2019/x64/Release/App/Projucer.exe" --resave *.jucer
```

### Building the plugin
Use the exported build configuration to build the plugin.
From the repository root directory run
```bash
cd "Builds/VisualStudio2019"
```

#### x64
```bash
<msbuild> -m:4 *.sln /property:Configuration=Release /property:Platform=x64
```

#### Win32
```bash
<msbuild> -m:4 *.sln /property:Configuration=Release /property:Platform=Win32
```

You will find the build artifacts under a platform-specific subfolder
| Artifact   | Platform     | Path                                                                                |
|------------|--------------|------------------------------------------------------------------------------------|
| Standalone | x64<br>Win32 | Builds/VisualStudio2019/x64/Release/HyperTremolo.exe<br>Builds/VisualStudio2019/Win32/Release/HyperTremolo.exe   |
| VST3       | x64<br>Win32 | Builds/VisualStudio2019/x64/Release/HyperTremolo.vst3<br>Builds/VisualStudio2019/Win23/Release/HyperTremolo.vst3 |

