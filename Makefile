.PHONY: run seating-chart clean

all: seating-chart

build:
	cmake . -B build -DCMAKE_BUILD_TYPE=Release

src/6x8.h: preproc.py 6x8.txt
	python preproc.py

seating-chart: build src/6x8.h
	make -C build

run: ./build/seating-chart
	./build/seating-chart

clean:
	rm -rf build src/6x8.h
