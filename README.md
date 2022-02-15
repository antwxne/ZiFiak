# ZIA

## Require

- cmake >= 3.21
- conan

## Build

Linux and Mac OS

```shell 
make
```

Windows

```shell
mkdir -p build && cd build && conan install .. --build=missing && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . ; cd .. 
```

## Push to Epitech repository

```shell
./peuch_datte.sh
```
