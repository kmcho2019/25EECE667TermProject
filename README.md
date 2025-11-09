# Placement TermProject

[![Build Status](https://img.shields.io/badge/complete-template%20-green)]()

This repository is for implementing the placement algorithm.
Used API is based on [OpenDB](https://github.com/The-OpenROAD-Project/OpenDB) API.

Currently, this project is for giving template of term project, which is for coursework of `POSTECH`, Korea:

## External Dependencies

You can use `Dockerfile` in the `OpenDB` submodule,
or you are required to install `cmake`, `siwg`, `spdlog`, `boost` in the `Ubuntu` Environment.
When working on grading server(CentOS 7) you can use `source tool.env` to load the modules.
Check the GuideLine.md for more details.

## How to build

```shell
mkdir build
cd build
cmake ..
make
./placer
```

## Lisence

`CSDL` Laboratory, `POSTECH`, Korea.
