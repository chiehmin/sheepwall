CXX := g++
# TODO: remove -fpermissive
CXXFLAGS := --std=c++14 -fpermissive -I./include
LDFLAGS :=
LDLIBS := -lpcap

SOURCES=$(shell find src -name "*.cc")
OBJECTS=$(addprefix build/, $(SOURCES:src/%.cc=%.o))

.PHONY: all 1900 clean

all: build/sheepwall build/test/unit_test

1900: CXX := /home/fatminmin/1900/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++
1900: CXXFLAGS += -I/home/fatminmin/boost_1_66_0 -I/home/fatminmin/arm-libpcap/include
1900: LDFLAGS += -L/home/fatminmin/arm-libpcap/lib
1900: build/sheepwall

build/sheepwall: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# test
TEST_SOURCES=$(shell find test -name "*.cc")
TEST_OBJECTS=$(addprefix build/test/, $(TEST_SOURCES:test/%.cc=%.o)) $(filter-out build/main.o, $(OBJECTS))
TEST_CXXFLAGS := $(CXXFLAGS)
TEST_LDFLAGS := $(LDFLAGS)
TEST_LDLIBS := $(LDLIBS) -lgtest -lgtest_main

build/test/unit_test: $(TEST_OBJECTS)
	$(CXX) $(TEST_CXXFLAGS) $(LDFLAGS) -o $@ $^ $(TEST_LDLIBS)

build/test/%.o: test/%.cc
	if [ ! -d build/test ]; then mkdir -p build/test; fi
	$(CXX) $(TEST_CXXFLAGS) -o $@ -c $<

clean:
	rm -rf build/*
