.PHONY        := cleanup
CC            := g++
STD           := -std=c++11
FLAGS         := -Wall -Wextra -Wpedantic -Wconversion -Wstrict-overflow=5 -Wshadow -Wunused-macros -Wcast-qual -Wcast-align -Wwrite-strings -Wdangling-else -Wlogical-op -Winline
SOURCE        := *.cpp

static: $(SOURCE)
	$(CC) $(INCLUDES) $(FLAGS) $(STD) -D NDEBUG -O3 -c $(SOURCE)
	ar rcs cppjson.a *.o
	make cleanup

test1.exe: ./tests/test1.cpp $(SOURCE)
	$(CC) $(FLAGS) $(STD) -g -o test1 ./tests/test1.cpp $(SOURCE)

test2_debug.exe: ./tests/test2.cpp $(SOURCE)
	$(CC) $(FLAGS) $(STD) -g -Og -o test2_debug ./tests/test2.cpp $(SOURCE)

test2_release.exe: ./tests/test2.cpp $(SOURCE)
	$(CC) $(FLAGS) $(STD) -D NDEBUG -O3 -o test2_release ./tests/test2.cpp $(SOURCE)

test3.exe: ./tests/test3.cpp $(SOURCE)
	$(CC) $(FLOAGS) $(STD) -g -Og -o test3 ./tests/test3.cpp $(SOURCE)

cleanup:
	rm *.o