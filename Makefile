CXX ?= g++
# TODO: remove -fpermissive
CXXFLAGS := --std=c++14 -fpermissive -I./include
LDFLAGS :=
LDLIBS := -pthread -lpcap -lboost_program_options

SOURCES := $(shell find src -name "*.cc")

.PHONY: all x86 arm clean

all: x86 arm

# x86 target
x86: build/x86/sheepwall build/test/unit_test

X86_OBJECTS := $(addprefix build/x86/, $(SOURCES:src/%.cc=%.o))
build/x86/sheepwall: $(X86_OBJECTS)
	if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/x86/%.o: src/%.cc
	if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# test
TEST_SOURCES=$(shell find test -name "*.cc")
TEST_OBJECTS := $(addprefix build/test/, $(TEST_SOURCES:test/%.cc=%.o) $(filter-out main.o, $(SOURCES:src/%.cc=%.o)))
TEST_CXXFLAGS := $(CXXFLAGS) --coverage
TEST_LDFLAGS := $(LDFLAGS)
TEST_LDLIBS := $(LDLIBS) -lgtest -lgtest_main -lgcov

build/test/unit_test: $(TEST_OBJECTS)
	$(CXX) $(TEST_CXXFLAGS) $(LDFLAGS) -o $@ $^ $(TEST_LDLIBS)

build/test/%.o: test/%.cc
	if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(TEST_CXXFLAGS) -o $@ -c $<

build/test/%.o: src/%.cc
	if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(TEST_CXXFLAGS) -o $@ -c $<

# ARM target
arm: build/arm/sheepwall

# using ubuntu's g++-arm-linux-gnueabi package
arm: CXX := arm-linux-gnueabi-g++
arm: CXXFLAGS += -I/home/fatminmin/arm-boost/include -I/home/fatminmin/arm-libpcap/include
arm: LDFLAGS += -L/home/fatminmin/arm-boost/lib -L/home/fatminmin/arm-libpcap/lib -static

ARM_OBJECTS := $(addprefix build/arm/, $(SOURCES:src/%.cc=%.o))
build/arm/sheepwall: $(ARM_OBJECTS)
	if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/arm/%.o: src/%.cc
	if [ ! -d `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf build/*
