.PHONY: default
default: build
	cd build; ./fortress-commander

.PHONY: test
test: build
	cd build; ./fortress-commander_TEST

.PHONY: build
build:
	[[ -d build ]] || mkdir build
	cd build; cmake .. && make -j2

.PHONY: format
format:
	clang-format -i src/*.cpp src/*.h

.PHONY: clean
clean:
	rm -rf build