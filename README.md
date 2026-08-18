# SimulIDE for Apple Silicon (ARM64) & Cross-Platform

[![macOS ARM64](https://img.shields.io/badge/Platform-macOS%20ARM64%20(Apple%20Silicon)-black?style=flat&logo=apple)](https://github.com/nur-srijan/SimulIDE-on-ARM)
[![Qt](https://img.shields.io/badge/Qt-6.x%20%7C%205.x-41CD52?style=flat&logo=qt)](https://www.qt.io/)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](COPYING)

**SimulIDE** is a fast, real-time electronic circuit simulator designed for hobbyists, educators, and engineers to experiment with analog, digital, and microcontroller-based circuits.

This fork provides **native Apple Silicon (ARM64) macOS support** with modern Qt 6 compatibility, retina display scaling, enhanced Dark Mode support, and UI fixes.

---

## Key Features

- **Real-Time Simulation**: Deeply optimized engine providing instant interactive feedback and low CPU overhead.
- **Microcontrollers & Processors**: Built-in support for Arduino, AVR, PIC, MCS-51 (8051), 6502, and Z80.
- **Rich Component Libraries**: 7400-series TTL ICs, CMOS logic, passive and active components, displays, motors, sensors, and meters.
- **Integrated Code Editor & Debugger**: Write, compile, and debug firmware directly inside SimulIDE with breakpoints, memory watches, and register inspection.
- **Custom Subcircuits**: Create and package reusable subcircuits and modular ICs.
- **Modern UI & Dark Mode**: Native look and feel, high-contrast dark mode palettes, scalable icons, and high-DPI support.

---

## Building on macOS (Apple Silicon ARM64)

### Prerequisites

Install Xcode Command Line Tools and Qt 6 via [Homebrew](https://brew.sh):

```bash
# Install Homebrew dependencies
brew install qt@6 qtsvg qtmultimedia qtserialport
```

Ensure the Homebrew Qt 6 bin directory is in your `PATH`:

```bash
export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"
```

### Build Instructions

1. **Clone the repository**:
   ```bash
   git clone git@github.com:nur-srijan/SimulIDE-on-ARM.git
   cd SimulIDE-on-ARM
   ```

2. **Generate the Makefile with qmake**:
   ```bash
   cd build_XX
   qmake ../SimulIDE.pro
   ```

3. **Compile**:
   ```bash
   make -j$(sysctl -n hw.ncpu)
   ```

4. **Run SimulIDE**:
   The compiled native `.app` bundle is located in `build_XX/executables/SimulIDE_2.0.0-/`:
   ```bash
   open executables/SimulIDE_2.0.0-/simulide.app
   ```

---

## Building on Linux

### Prerequisites (Ubuntu/Debian)

```bash
sudo apt-get install build-essential qtbase5-dev qtmultimedia5-dev \
    libqt5svg5-dev libqt5serialport5-dev qt5-qmake
```

### Build

```bash
cd build_XX
qmake ../SimulIDE.pro
make -j$(nproc)
```

The executable will be located in `build_XX/executables/SimulIDE_x.x.x/simulide`.

---

## Upstream & Acknowledgements

- **Original Project**: Created and maintained by [Santiago González](https://simulide.com) ([Arcachofo/SimulIDE-dev](https://github.com/Arcachofo/SimulIDE-dev)).
- **License**: GNU General Public License v3 (see [COPYING](COPYING)).
