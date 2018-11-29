SHELL := /bin/bash

default:
.PHONY: default
default: build
	cd build; cmake -DTEST=false .. && make -j2
	cd build; ./fortress-commander

.PHONY: test
test: build
	cd build; cmake -DTEST=true .. && make -j2
	cd build; ./fortress-commander_TEST

.PHONY: build
build:
	[[ -d build ]] || mkdir build

.PHONY: format
format:
	clang-format -i src/*.cpp src/*.h

.PHONY: clean
clean:
	rm -rf build

.PHONY: cpp
cpp:
	@python3 checker.py

.PHONY: gdb
gdb:
	gdb build/fortress-commander