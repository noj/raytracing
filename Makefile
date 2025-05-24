.PHONY: view

view:
	@./ray > render.ppm
	@imv-wayland render.ppm
	@rm render.ppm

./ray: ray.cpp arena.hpp
	@clang++ -std=c++20 -O2 -Wall -pedantic ray.cpp -o ray
