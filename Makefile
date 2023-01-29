NUMBER_FILES=10
NUMBER_TESTS=2
DIR=build/
all: compile
run: compile
	./a.out --batch --test-per-file $(NUMBER_TESTS) --max-test $(NUMBER_FILES) --dir $(DIR)
compile:
	mkdir -p build
	g++ -g generate_user_test.cpp
clean:
	rm -fr a.out build/