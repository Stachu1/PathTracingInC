CC := gcc

main: build/main.o build/vec3.o build/ray.o build/camera.o build/skybox.o
	$(CC) -o $@ $^ -L`pwd`/lib -lsdl2

build/%.o: src/%.c | mkdir
	$(CC) -c -o $@ $< -I`pwd`/include

.PHONY: clean run build

mkdir:
	mkdir -p build

build: clean build main

run: clean build main
	./main

clean:
	rm -rf main build
