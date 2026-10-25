all: main test

test: test.o Pomodoro.o 
	g++ -o test Pomodoro.o test.o -lncurses -Wall

main: ergasia.o Pomodoro.o
	g++ -o sessions ergasia.o Pomodoro.o -lncurses -Wall
	
Promodoro.o: Pomodoro.cpp Pomodoro.hpp
	g++ -c Pomodoro.cpp
	
main.o: main.cpp
	g++ -c ergasia.cpp

test.o: test.cpp
	g++ -c test.cpp
