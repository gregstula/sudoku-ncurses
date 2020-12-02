CXX=g++
CXXFLAGS=-Wall -std=c++11
LIBS=-lncurses
SRC=main.cpp

all:
	$(CXX) $(CXXFLAGS) $(LIBS) -o sudoku $(SRC)
clean:
	@rm -f sudoku
	@rm -f *.o
format:
	clang-format --verbose --sort-includes -i *.cpp *.hpp
