all: ps6

ps6: regex.o
	g++ regex.o -o ps6 -lboost_regex

regex.o: regex.cpp
	g++ regex.cpp -c -g -ansi -Wall -Werror -pedantic -std=c++17

clean:
	rm ps6 regex.o