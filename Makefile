.PHONY: test

test:
	$(CXX) *.cpp -g -Wall -o test
	./test

clean:
	rm -f test
