# Sheepwall

[![Build Status](https://travis-ci.org/chiehmin/sheepwall.svg?branch=master)](https://travis-ci.org/chiehmin/sheepwall)

This is a http traffic parser intended to record unprotected account, password and cookies running on **router devices**.

Features:
- High performance
	- It is designed to be run on **routers** so it is written in C/C++
	- It has been tested to be work on [rt1900ac](https://www.synology.com/en-global/products/RT1900ac) and [rt2600ac](https://www.synology.com/en-global/products/RT2600ac)

## Demo

Just run the sheepwall and wait for the sheep to come in :).

[![asciicast](https://asciinema.org/a/35e6JGtbVK5igSeFd6I4bvqXB.png)](https://asciinema.org/a/35e6JGtbVK5igSeFd6I4bvqXB)

## Usage

Quick installation

```
# Download sheepwall binary and put it into you router
SynologyRouter> curl -L https://github.com/chiehmin/sheepwall/releases/download/v0.1.1/sheepwall_arm > sheepwall
```

```
$ sheepwall -i <interface> [-w <log file>] [-D]
options:
  -h [ --help ]          Print help messages
  -i [ --interface ] arg Interface sniffered by sheepwall
  -w [ --write_to ] arg  Log file
  -D [ --daemon ]        Daemonizing... run in the background
```

## Building

This project uses `libpcap`, `boost` and `googletest`. After installing all above libraries, just simply execute `make` to build this project.

### Build for ARM-based devices

### Cross-compile `libpcap`

We need the arm version of `libpcap` to build our sheepwall. Here I used the `arm-linux-gnueabi-gcc` from Ubuntu's `g++-arm-linux-gnueabi` package.

Download it!

```
$ mkdir ~/arm-libpcap
# download the libpcap source code
$ curl -L https://github.com/the-tcpdump-group/libpcap/archive/libpcap-1.8.1.tar.gz > libpcap.tar.gz
# extract it
$ tar -hxf libpcap.tar.gz
```

compile it!!

```
$ mkdir ~/arm-libpcap
$ CC=arm-linux-gnueabi-gcc \
./configure --host=arm-linux --prefix=/home/fatminmin/arm-libpcap --with-pcap=linux
$ make -j4
$ make install
```

Finally, the compiled `libpcap` will be put under `/home/fatminmin/arm-libpcap`

### Cross-compile boost

```
$ curl -L https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz > boost.tar.gz
$ tar -hxf boost.tar.gz && cd boost_1_66_0
# we only use program_options library here
$ ./bootstrap.sh --with-libraries=program_options
```

Modify `using gcc ;` to  `using gcc : arm : arm-linux-gnueabi-g++ ;` in `project-config.jam`

```
# compile it
$ mkdir ~/arm-boost
$ ./bjam install toolset=gcc-arm runtime-link=static --prefix=/home/fatminmin/arm-boost
```

### Compile sheepwall

In the `Makefile`, I re-defined the [Target-specific Variable](https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html) to compile sheepwall for ARM deviced.

```makefile
arm: CXX := arm-linux-gnueabi-g++
arm: CXXFLAGS += -I/home/fatminmin/arm-boost/include -I/home/fatminmin/arm-libpcap/include
arm: LDFLAGS += -L/home/fatminmin/arm-boost/lib -L/home/fatminmin/arm-libpcap/lib -static
arm: build/sheepwall
```

### Run it

Send it to your arm devices(Synology rt1900ac for example) and run it.

```
$ scp build/sheepwall root@router.synology.com:/root
# in the router console
SynologyRouter> ./sheepwall -i eth0 -w cred.log -D
```

## Disclaimer

Usage of sheepwall for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program.

## Author

Chieh-Min Wang a.k.a FatMinMin

