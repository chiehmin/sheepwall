CXX := g++
# TODO: remove -fpermissive
CXXFLAGS := --std=c++14 -fpermissive -I./include
LDFLAGS :=
LDLIBS := -lpcap

SOURCES=$(shell find src -name "*.cc")
OBJECTS=$(addprefix build/, $(SOURCES:src/%.cc=%.o))

.PHONY: all 1900 clean

all: build/sheepwall build/test/unit_test

# using ubuntu's g++-arm-linux-gnueabi package
arm: CXX := arm-linux-gnueabi-g++
arm: CXXFLAGS += -I/home/fatminmin/boost_1_66_0 -I/home/fatminmin/arm-libpcap/include
arm: LDFLAGS += -L/home/fatminmin/arm-libpcap/lib -static
arm: build/sheepwall

build/sheepwall: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: src/%.cc
	if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# test
TEST_SOURCES=$(shell find test -name "*.cc")
TEST_OBJECTS=$(addprefix build/test/, $(TEST_SOURCES:test/%.cc=%.o)) $(filter-out build/main.o, $(OBJECTS))
TEST_CXXFLAGS := $(CXXFLAGS) $(shell pkg-config --libs gtest_main)
TEST_LDFLAGS := $(LDFLAGS)
TEST_LDLIBS := $(LDLIBS) $(shell pkg-config --libs gtest_main)

build/test/unit_test: $(TEST_OBJECTS)
	$(CXX) $(TEST_CXXFLAGS) $(LDFLAGS) -o $@ $^ $(TEST_LDLIBS)

build/test/%.o: test/%.cc
	if [ ! -d build/test ]; then mkdir -p build/test; fi
	$(CXX) $(TEST_CXXFLAGS) -o $@ -c $<

clean:
	rm -rf build/*
