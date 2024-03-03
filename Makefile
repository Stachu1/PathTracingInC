CC := gcc

main: build/main.o build/vec3.o build/ray.o build/camera.o build/skybox.o
	$(CC) -o $@ $^ -L`pwd`/lib -lsdl2

build/%.o: src/%.c
	$(CC) -c -o $@ $< -I`pwd`/include

.PHONY: clean run build

build: main

run: main
	./main

clean:
	rm -f main build/*.o