build:
	g++ test.cpp Json.cpp -o test -O2 -std=c++17
debug:
	g++ -g test.cpp Json.cpp -o test -std=c++17
clean:
	rm test
