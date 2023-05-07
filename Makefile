.PHONY: test format

test:
	$(CXX) *.cpp -g -Wall -o test
	./test

format:
	clang-format -i *.{hpp,cpp}

clean:
	rm -f test
