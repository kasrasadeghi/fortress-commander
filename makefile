default:
	[[ -d build ]] || mkdir build
	cd build; cmake .. && make && ./fortress-commander