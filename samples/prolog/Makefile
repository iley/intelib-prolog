.PHONY: default clean build intelib

default: intelib build

intelib:
	$(MAKE) -C ../.. library

build:
	cd cpp && $(MAKE) build
	cd pl && $(MAKE) build

clean:
	cd cpp && $(MAKE) clean
	cd pl && $(MAKE) clean
