### Build for rt1900ac

The toolchain of Synology's routers and nases can be found at [source
forge](https://sourceforge.net/projects/dsgpl/?source=navbar).

Just simply download it and extract it.

```
$ curl -L https://nchc.dl.sourceforge.net/project/dsgpl/SRM%201.1%20Tool%20Chains/BROADCOM%20NORTHSTARPLUS%20Linux%203.6.5/gcc493_glibc220_hard-GPL.tgz > 1900.tgz
$ mkdir ~/1900; tar -hxf 1900.tgz -C ~/1900
```

### Cross-compile `libpcap`

We need the arm version of `libpcap` to build our sheepwall. We can just use the rt1900's toolchain to build `libpcap`.

```
# folder to put the arm version(1900 version) of libpcap
# download the libpcap source code
$ curl -L https://github.com/the-tcpdump-group/libpcap/archive/libpcap-1.8.1.tar.gz > libpcap.tar.gz
# extract it
$ tar -hxf libpcap.tar.gz
```

compile it!!

Here I explicitly set the `CC`, `CFLAGS` and `LDFLAGS` to the toolchain path of rt1900ac to compile the `libpcap`.

```
$ mkdir ~/arm-libpcap
$ CC=/home/fatminmin/1900/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc \
CFLAGS=-I/home/fatminmin/1900/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/usr/include/ \
LDFLAGS=-L/home/fatminmin/1900/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/usr/lib \
./configure --host=arm-linux --prefix=/home/fatminmin/arm-libpcap --with-pcap=linux
$ make -j4
$ make install
```

Finally, the compiled `libpcap` will be put under `/home/fatminmin/arm-libpcap`

### Download the boost

I just use some boost features which are header-only, so just dowload it and there's no need to compile it.

```
$ curl -L https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz > boost.tar.gz
$ tar -hxf boost.tar.gz
```

### Compile sheepwall

In the `Makefile`, I re-defined the [Target-specific Variable
Values](https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html) to compile sheepwall for rt1900ac.

```makefile
1900: CXX := /home/fatminmin/1900/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++
1900: CXXFLAGS += -I/home/fatminmin/boost_1_66_0 -I/home/fatminmin/arm-libpcap/include
1900: LDFLAGS += -L/home/fatminmin/arm-libpcap/lib
1900: build/sheepwall
```

