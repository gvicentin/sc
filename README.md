# Subnet Calculator (sc)

A command line tool that enables subnet network calculations using **CIDR notation**.

To use it, You can type your range directly in CIDR notation, and see the IP
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
cp bin/sc-0.1.0 ~/.local/bin
ln -s ~/.local/bin/sc-0.1.0 ~/.local/bin/sc
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