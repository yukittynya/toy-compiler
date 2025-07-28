CC = clang
FLAGS = -O3 -ffast-math -flto

build/bin/compiler: build/main.o build/token.o build/lexer.o | build/bin
	$(CC) $(FLAGS) $? -o $@
	chmod +x build/bin/compiler

build/main.o: src/main.c | build
	$(CC) $(FLAGS) -c $? -o $@

build/token.o: src/lib/token/token.c | build
	$(CC) $(FLAGS) -c $? -o $@

build/lexer.o: src/lib/lexer/lexer.c | build
	$(CC) $(FLAGS) -c $? -o $@

build: 
	mkdir -p build

build/bin: 
	mkdir -p build/bin

.PHONY: clean
clean:
	rm -rvf build
