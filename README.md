<p align="center">
  <a href="#">
    <img alt="Dunk" src="https://i.imgur.com/k2EFwSq.png" />
  </a>
</p>

<p align="center">
    <a href="https://sert2521.org"><img alt="team 2521" src="https://img.shields.io/badge/team-2521-7d26cd.svg?style=flat-square" /></a>
    <a href="https://github.com/SouthEugeneRoboticsTeam/dunk/blob/master/LICENSE"><img alt="team 2521" src="https://img.shields.io/github/license/SouthEugeneRoboticsTeam/dunk.svg?style=flat-square" /></a>
    <a href="https://travis-ci.org/SouthEugeneRoboticsTeam/dunk"><img alt="team 2521" src="https://img.shields.io/travis/SouthEugeneRoboticsTeam/dunk/master.svg?style=flat-square" /></a>
</p>

<h4 align="center">
  A fast ICP SLAM implementation for FRC robots.
</h4>

# Building

Before building, ensure you have [MRPT](https://github.com/MRPT/mrpt) 2.x
installed. MRPT requires Ubuntu 16.04 or later. If using Ubuntu 16.04, you'll
also need to install gcc-7. Instructions on installing gcc-7 are available in
the [MRPT documentation](https://github.com/MRPT/mrpt#31-ubuntu).

To get the latest version of MRPT, either build it from source or install it
using `apt-get`:

```bash
$ sudo add-apt-repository ppa:joseluisblancoc/mrpt
$ sudo apt-get update
$ sudo apt-get install libmrpt-dev mrpt-apps
```

Then, build a `dunk` binary using the following commands:

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```

# Usage

```bash
$ ./dunk <config.ini>

# For example...
$ ./dunk icp-slam-config.ini
```
