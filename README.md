# Hardware-Reader
A high-performance C++ utility for interfacing with system hardware components. Inspired by Neofetch.

## Prerequisites
- **Operating System:** System: Linux
- **Compiler:** g++ 13.3 or higher
- **Language Standard:** C++20
- **Build System:** CMake 3.10+

## Setup

### 1) Clone the Repository

```bash
git clone https://github.com/MrKarkeys/Hardware-Reader.git
cd Hardware-Reader
```

### 2) Install Dependencies
ex: Debian based:
```bash
sudo apt update
sudo apt install build-essential cmake
```

### 3) Build using CMake
```bash
mkdir build && cd build
cmake ..
make
```

### 4) Move executable to /usr/local/bin 
```bash
sudo cp hardware-reader /usr/local/bin/
```

### 5) Once installed, you can launch the application from any terminal session by simply typing:
```bash
hardware-reader
```

## Example Output
```bash
--- Hardware Info ---
User:       Username
Host:       Hostname
OS:         Your OS
Device:     Manufacturer Model Name
Kernel:     x.x.x-xxx-generic
Uptime:     0 days 00 hours 00 mins 00 seconds
Shell       bash
DE:         Desktop Environment
Terminal    Terminal Emulator
CPU:        CPU Model 
GPU 0:      Integrated Graphics Model
GPU 1:      Dedicated Graphics Model
Memory:     Total RAM Size MiB
```


## Developer Commands
1. Removes generated executable files.
```bash
make clean
```

2. Runs clang-format to enforce project code style standards.
```bash
make format
```
