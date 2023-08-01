# Node system monitor

Linux system monitor bindings for Node

## Compiling

This library requires `libsensors`, you can install it using the following commands

|   OS   | Command                                               |
|--------|-------------------------------------------------------|
| Ubuntu | `sudo apt-get install build-essential libsensors-dev` |
| Fedora | `sudo yum install gcc-c++ lm_sensors`                 |
| Arch   | `sudo pacman -S base-devel lm_sensors`                |

Compile from source

```sh
npm run build
```

## Usage

You can check typings and testings for what values are included in return values.

- **getCPUUsage()**

  Returns an array of CPU time differences between the last call.
  The first result should be discarded if necessary because it's a total value since starting.

  A `general` property is included for all CPU threads.

- **getDiskUsage(file)**

  Returns the disk usage of a specific file or directory

- **getMemoryUsage()**

  Returns the current physical memory and swap usage

- **getNetworkUsage()**

  Returns an array of sent/received bytes and packets for each network device.
  The first result should be discarded if necessary because it's a total value since starting.

- **getSensors()**

  Returns an array of sensors and values like hardware temperatures.
  `libsensors` is required for this feature.
