# Sudoku solver
This is a sudoku solver for sudoku grids which present perfect squares (4x4, 9x9, 16x16, ...)
# Idea
Having a broader look at sudoku problem, we can notice that it is a special case of NP-complete exact cover problem with 4 constraints:
<ul>
  <li>Each cell should contain exectly one number from 1 to 9</li>
  <li>Each row should contain exectly one number from 1 to 9</li>
  <li>Each column should contain exectly one number from 1 to 9</li>
  <li>Each box should contain exectly one number from 1 to 9</li>
</ul>

Although there is no "effective" solution, the best know approach is implemented. It can solve any sudoku puzzle which size is up to 25x25 in a reasonable time.
The approach was proposed by Donald E. Knuth in the paper [Dancing Links, Stanford, 2000](https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf) also know as "Algorithm X".
## To run the code:
1. clone this respository
2. Make sure you have MinGW (gcc and g++ compilers) installed and paths set
3. Make sure you have CMake installed and path set
4. In project root create Build directory `mkdir Build`
5. Move to newly created directory `cd Build`
6. Generate Makefile: `cmake ../ -G"MinGW Makefiles"`
7. Build the project: `cmake --build .`
8. OPTIONAL: Run the tests: `ctest`
9. Put file with input data `input.txt` into Build directory (example of input.txt provided below)
10. Run the code: `DLXSolver_run.exe`
11. Results will be shown in CLI (Command line interface)

### Example `input.txt`
9<br>
0 0 0 0 3 7 6 0 0<br>
0 0 0 6 0 0 0 9 0<br>
0 0 8 0 0 0 0 0 4<br>
0 9 0 0 0 0 0 0 1<br>
6 0 0 0 0 0 0 0 9<br>
3 0 0 0 0 0 0 4 0<br>
7 0 0 0 0 0 8 0 0<br>
0 1 0 0 0 9 0 0 0<br>
0 0 2 5 4 0 0 0 0<br>
