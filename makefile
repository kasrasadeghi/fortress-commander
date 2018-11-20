
.PHONY: default
default: cmake
	cd build && make && ./fortress-commander

.PHONY: cmake
cmake:
	[[ -d build ]] || mkdir build
	cd build; cmake .. 

.PHONY: format
format:
	clang-format -i src/*.cpp src/*.h

SFML:
	git clone https://github.com/SFML/SFML

# .PHONY: SFML
SFML-install:	SFML
	cd SFML;       [[ -d build ]] || mkdir build
	cd SFML/build; cmake ..
	cd SFML/build; make -j8 DESTDIR=../../SFML-install install

	cd SFML-install/usr/local; cp -r . ../..
	cd SFML-install;           rm -rf usr

.PHONY: clean
clean:
	rm -rf build