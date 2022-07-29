# Subnet Calculator (sc)

![Latest Release](https://img.shields.io/github/v/release/gvicentin/sc)
![License](https://img.shields.io/github/license/gvicentin/sc)
![Build And Tests Status](https://img.shields.io/github/workflow/status/gvicentin/sc/Pull%20Request%20Validation?label=build%20and%20tests&logo=c&logoColor=lightblue)

A command line tool that enables subnet network calculations using **CIDR notation**.

To use it, you can type your range directly in CIDR notation, and see the IP
information about that range.

This simple subnet calculator is **written in C**. It was created as a study about using
bitwise operators and basic IPv4 subnetting (Networking course).

## Installing

Check the release page if you don't want to build from
the source code.

```sh
# Clone repository and compile
git clone https://github.com/gvicentin/sc.git && cd sc/
make
```

Make sure to include the binary on your `$PATH`.
For example.

```sh
# Copy binary and create symbolic link
LOCALBIN="${HOME}/.local/bin"
cp bin/sc-0.1.0 $LOCALBIN
cd $LOCALBIN && ln -s sc-0.1.0 sc
```

## Usage

Use the command passing the IP in the CIDR notation,
e.g. `sc 172.16.65.23/20`.

```sh
sc 172.16.65.23/20
====================================
|            SUBNET CALC           |
====================================
IP:                     172.16.65.23
IP Hex:                     AC104117
IP Long:                  2886746391
CIDR Block:           172.16.64.0/20
Broadcast:             172.16.79.255
------------------------------------
Mask:                  255.255.240.0
Mask Hex:                   FFFFF000
Mask Long:                4294963200
Mask Bits:                        20
Num of Hosts:                   4096
```


## Local development

To run unit tests, simply do the following.

```sh
make test
```
