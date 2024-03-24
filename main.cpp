#include <iostream>
#include <vector>
#include "DLXSolver_lib/SudokuDLXSolver.h"

int main()
{
    int size;
    std::vector<std::vector<int>> puzzle;
    std::vector<int> rowPuzzle;
    std::string filename = "input.txt";
    double eps = 0.000001;
    std::ifstream fin;
    fin.open(filename);
    if (fin.fail()) {
        std::cout << "Error, could not open " << filename << " for reading" << std::endl;
        fin.close();
        return 1;
    }
    int readValue;
    fin >> size;

    // Checking that field (box) is a perfect square
    if (std::abs(floor(sqrt(size)) - sqrt(size)) >= eps) {
        std::cerr << "Invalid field" << std::endl;
        return 1;
    }

    for (int i = 0; i < size; ++i) {
        rowPuzzle.clear();
        for (int j = 0; j < size; ++j) {
            fin >> readValue;
            rowPuzzle.push_back(readValue);
        }
        puzzle.push_back(rowPuzzle);
    }
    auto* sudokuDLXSolver = new SudokuDLXSolver(size);
    puzzle = sudokuDLXSolver->SolveWrap(puzzle);

    std::cout << "Found solution: " << std::endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << puzzle[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
