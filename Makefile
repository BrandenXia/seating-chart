.PHONY: run seating-chart pgo clean

MODE ?= release

ifeq ($(MODE), debug)
	CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Debug
else
	CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Release
endif

all: seating-chart

build:
	cmake . -B build $(CMAKE_FLAGS)

src/6x8.h: preproc.py 6x8.txt
	python preproc.py

seating-chart: build src/6x8.h
	make -C build

run: ./build/seating-chart
	./build/seating-chart

pgo-gen: src/6x8.h
	mkdir -p pgo
	cmake . -B pgo/gen $(CMAKE_FLAGS) -DPGO_MODE=GEN
	make -C pgo/gen

pgo-run: pgo-gen
	mkdir -p profiles
	LLVM_PROFILE_FILE="profiles/default-%p.profraw" ./pgo/gen/seating-chart

pgo-use:
	llvm-profdata merge -sparse ./profiles/*.profraw -o ./profiles/default.profdata
	cmake . -B pgo/use $(CMAKE_FLAGS) -DPGO_MODE=USE
	make -C pgo/use

clean:
	rm -rf build src/6x8.h
	rm -rf pgo
	rm -rf profiles
