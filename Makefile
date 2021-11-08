build:
	g++ test.cpp Json.cpp -lpcrecpp -o test -O2
debug:
	g++ -g test.cpp Json.cpp -lpcrecpp -o test
clean:
	rm test
