#include <vector>
#include "assert_macro.h"
#include "../DLXSolver_lib/SudokuDLXSolver.h"

int test_matrix_4x4() {
    int err_code = 0;
    int size = 4;
    auto* sudokuDLXSolver = new SudokuDLXSolver(size);
    std::vector<std::vector<int>> input = {{1, 4, 3, 2}, {0, 0, 1, 4}, {4, 1, 2, 3}, {2, 3, 0, 0}};
    std::vector<std::vector<int>> expected = {{1, 4, 3, 2}, {3, 2, 1, 4}, {4, 1, 2, 3}, {2, 3, 4, 1}};
    input = (sudokuDLXSolver->SolveWrap(input));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            assertEqual (input[i][j] == expected[i][j]);
    return err_code;
}

int main() {
    return test_matrix_4x4();
}
