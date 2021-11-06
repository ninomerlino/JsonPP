build:
	g++ test.cpp Json.cpp -lpcrecpp -o test
debug:
	g++ -g test.cpp Json.cpp -lpcrecpp -o test
clean:
	rm test
