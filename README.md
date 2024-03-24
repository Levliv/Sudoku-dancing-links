# Sudoku solver
# Idea
Having a broader look at sudoku problem, we can notice that it is a special case
The approach implemented in this solution was proposed by Donald E. Knuth in the paper [Dancing Links, Stanford, 2000](https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf) also know as "Algorithm X"
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
