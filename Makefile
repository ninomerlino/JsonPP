build:
	g++ test.cpp Json.cpp -o test -O2
debug:
	g++ -g test.cpp Json.cpp -o test
clean:
	rm test
