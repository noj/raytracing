.PHONY: view

view: render.ppm

ray: ray.cpp
	@clang++ -std=c++20 -O2 -Wall -pedantic ray.cpp -o ray

render.ppm: ./ray
	@./ray > $@
	@imv-wayland $@
