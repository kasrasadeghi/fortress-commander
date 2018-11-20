.PHONY: default
default:
	[[ -d build ]] || mkdir build
	cd build; cmake .. && make && ./fortress-commander

.PHONY: format
format:
	clang-format -i src/*.cpp src/*.h

.PHONY: clean
clean:
	rm -rf build