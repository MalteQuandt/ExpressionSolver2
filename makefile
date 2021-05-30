FLAGS = -g3 -O0 -Wall -Wextra -std=c++17
CC = g++
INC = Token.hpp

all: main Solver start

main: main.cpp
	@echo "Compiling main to object..."
	$(CC) $(FLAGS) -c $< -I Token.hpp
Solver: Solver.cpp
	@echo "Compiling Solver to object..."
	$(CC) $(FLAGS) -c $<

start: main Solver
	@echo "Linking the object files..."
	$(CC) -o "main.exe" main.o Solver.o -I Token.hpp;
	@echo "Done!"
clean: 
	@echo "Deleting the objects..."
	rm *.o
	@echo "Deleting the executables."
	rm *.exe
	@echo "Done!"