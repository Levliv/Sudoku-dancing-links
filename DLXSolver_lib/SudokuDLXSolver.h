#ifndef SUDOKUDLXSOLVER_SUDOKUDLXSOLVER_H
#define SUDOKUDLXSOLVER_SUDOKUDLXSOLVER_H
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <cstdlib>
#include "Node.h"

class SudokuDLXSolver
{
public:
  explicit SudokuDLXSolver(int);

  ~SudokuDLXSolver();

  std::vector<std::vector<int>> SolveWrap(std::vector<std::vector<int>>);

private:
  Node* root;
  std::stack<Node>* solution;
  bool solved;
  int size;
  int sizeSqrt;

  void Print();

  bool Solve();

  std::stack<Node>* Solve(std::vector<std::vector<int>> puzzle);

  void Cover(Node* columnHeaderNode);

  void Uncover(Node* node);

  Node* FindNode(int, int, int);

  std::pair<bool, Node*> FindColumnWithMinNumberOfNodes();
};

#endif
