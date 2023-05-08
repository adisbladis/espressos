.PHONY: test format

test:
	$(CXX) *.cpp -g -Wall -Werror -o test
	./test

format:
	clang-format -i examples/*/*.ino *.{hpp,cpp}

clean:
	rm -f test
