.PHONY        := cleanup
CC            := g++
STD           := -std=c++11
INCLUDES      :=
FLAGS         := -Wall -Wextra -Wpedantic -Wconversion -Wstrict-overflow=5 -Wshadow -Wunused-macros -Wcast-qual -Wcast-align -Wwrite-strings -Wdangling-else -Wlogical-op -Winline -fno-exceptions -fdiagnostics-all-candidates
SOURCE        := *.cpp

test1.exe: ./tests/test1.cpp $(SOURCE)
	$(CC) $(INCLUDES) $(FLAGS) $(STD) -g -Og -o test1 ./tests/test1.cpp $(SOURCE)

test2_debug.exe: ./tests/test2.cpp $(SOURCE)
	$(CC) $(INCLUDES) $(FLAGS) $(STD) -g -Og -o test2_debug ./tests/test2.cpp $(SOURCE)

test2_release.exe: ./tests/test2.cpp $(SOURCE)
	$(CC) $(INCLUDES) $(FLAGS) $(STD) -D NDEBUG -O3 -o test2_release ./tests/test2.cpp $(SOURCE)