SHELL := /bin/bash

default:
.PHONY: default
default: build\:false
	cd build; ./fortress-commander

.PHONY: test
test: build\:true
	cd build; ./fortress-commander_TEST

.PHONY: build
build\:%: cmake\:%
	cd build; make -j8

# usage cmake:true for testing, cmake:false otherwise
.PHONY: cmake\:%
cmake\:%: buildfolder
	cd build; cmake -DTEST=$* ..

.PHONY: buildfolder
buildfolder:
	[[ -d build ]] || mkdir build

.PHONY: format
format:
	clang-format -i src/*.cpp src/*.h src/*/*.cpp src/*/*.h

.PHONY: clean
clean:
	rm -rf build

.PHONY: cpp
cpp:
	@python3 checker.py

.PHONY: gdb
gdb:
	gdb build/fortress-commander

.PHONY: profile
profile: build\:true
	valgrind --tool=callgrind build/fortress-commander_TEST
	kcachegrind callgrind.out.*
	rm -f callgrind.out.*
