#include <vector>
#include "assert_macro.h"
#include "../DLXSolver_lib/SudokuDLXSolver.h"

int test_matrix_9x9() {
    int err_code = 0;
    int size = 9;
    auto* sudokuDLXSolver = new SudokuDLXSolver(size);
    std::vector<std::vector<int>> input = {{0, 0, 0, 0, 3, 7, 6, 0, 0}, {0, 0, 0, 6, 0, 0, 0, 9, 0}, {0, 0, 8, 0, 0, 0, 0, 0, 4}, {0, 9, 0, 0, 0, 0, 0, 0, 1}, {6, 0, 0, 0, 0, 0, 0, 0, 9}, {3, 0, 0, 0, 0, 0, 0, 4, 0}, {7, 0, 0, 0, 0, 0, 8, 0, 0}, {0, 1, 0, 0, 0, 9, 0, 0, 0}, {0, 0, 2, 5, 4, 0, 0, 0, 0}};
    std::vector<std::vector<int>> expected = {{9,5,4,1,3,7,6,8,2}, {2,7,3,6,8,4,1,9,5}, {1,6,8,2,9,5,7,3,4}, {4,9,5,7,2,8,3,6,1}, {6,8,1,4,5,3,2,7,9}, {3,2,7,9,6,1,5,4,8}, {7,4,9,3,1,2,8,5,6}, {5,1,6,8,7,9,4,2,3}, {8,3,2,5,4,6,9,1,7}};
    input = (sudokuDLXSolver->SolveWrap(input));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            assertEqual (input[i][j] == expected[i][j]);
    return err_code;
}

int main() {
    return test_matrix_9x9();
}
