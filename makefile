make: compile
	./main
compile:
	g++ main.cpp ./classes/Individual.cpp -o main