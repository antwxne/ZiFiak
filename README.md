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
mkdir -p build && cd build && conan install .. --build=missing && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
```

## Push to Epitech repository

```shell
git remote add epitech git@github.com:EpitechPromo2024/B-YEP-500-PAR-5-1-zia-arthur.fouquet.git
git push epitech master:main
```