CXX := g++
# TODO: remove -fpermissive
CXXFLAGS := --std=c++14 -fpermissive -I./include
LDFLAGS :=
LDLIBS := -lpcap

SOURCES=$(shell find src -name "*.cc")
OBJECTS=$(addprefix build/, $(SOURCES:src/%.cc=%.o))

.PHONY: all clean

all: build/sheepwall

build/sheepwall: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf build/*
