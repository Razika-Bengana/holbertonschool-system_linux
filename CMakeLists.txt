cmake_minimum_required(VERSION 3.26)
project(ls C)

set(CMAKE_C_STANDARD 23)

add_executable(ls ls/hls.c
        ls/main.h
        ls/my_strcmp.c
        ls/list_directory.c
        ls/my_strlen.c
        ls/hls.c
        ls/hls.c
        ls/my_strncpy.c
        getline/_getline.h
        getline/laps.h
        getline/0-main.c
        getline/1-main.c
        getline/_getline.c
        getline/laps.c
        readelf/elf_header.h
        readelf/elf_header-main.c
        readelf/elf_header-details.c
        readelf/elf_header-machine.c
        readelf/elf_header-type.c
        readelf/elf_header-abi_version.c
        readelf/elf_header-os_abi.c
        readelf/elf_header-version.c
        readelf/elf_header-data.c
        readelf/elf_header-class.c
        readelf/elf_header-magic.c
        readelf/elf_header-read.c
        readelf/elf_header-endian.c
        readelf/elf_sec_header.c)
