#include <vector>
#include <cassert>
#include <cmath>
#include <utility>
#include "SudokuDLXSolver.h"

/*
 * Initializing sparse constraint matrix and making DLX structure
 */
SudokuDLXSolver::SudokuDLXSolver(int gridSize) {
  root = new Node();
  root->right = root->left = root->up = root->down = root;
  solution = new std::stack<Node>();
  solved = false;
  size = gridSize;
  sizeSqrt = (int) sqrt(size);
  int numberOfColumns = size * size * 4;
  int numberOfRows = size * size * size;
  double eps = 0.000001;

  // Checking that field (box) is a perfect square
  assert(abs(floor(sqrt(size)) - sqrt(size)) < eps);

  // Creating empty (for now) constraint matrix. It includes 4 'big' columns of constraints: row, column, cell, box
  // Each 'big' column consists of size*size real columns
  Node*** matrix{new Node**[numberOfRows]{}};  // Allocate memory for a two-dimensional array
  // Allocate memory for nested arrays
  for (unsigned i{}; i < numberOfRows; i++) {
    matrix[i] = new Node*[numberOfColumns]{};
  }


  // Filling in the matrix with nodes
  // Linking the nodes in doubly linked list, so we can have horizontal connections between nodes in DLX structure
  int row;
  Node* rowNode;
  Node* columnNode;
  Node* cellNode;
  Node* boxNode;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      for (int k = 0; k < size; ++k) {
        row = (i * size * size + j * size + k);
        rowNode = matrix[row][(i * size + k)] = new Node(i, j, k); // CONSTRAINT 1: ROW
        columnNode = matrix[row][size * size + (j * size + k)] = new Node(i, j, k); // CONSTRAINT 2: COLUMN
        cellNode = matrix[row][2 * size * size + (i * size + j)] = new Node(i, j, k); // CONSTRAINT 3: CELL
        boxNode = matrix[row][3 * size * size + ((i / sizeSqrt + j / sizeSqrt * sizeSqrt) * size + k)] = new Node(i, j,
                                                                                                                  k); // CONSTRAINT 4: BOX
        rowNode->right = columnNode;
        rowNode->left = boxNode;
        columnNode->left = rowNode;
        columnNode->right = cellNode;
        cellNode->left = columnNode;
        cellNode->right = boxNode;
        boxNode->left = cellNode;
        boxNode->right = rowNode;
      }
    }
  }

  // Creating column headers for DLX structure
  // Linking the nodes in doubly linked list, so we can have vertical connections between nodes in DLX structure
  Node* columnHeader = new Node();
  columnHeader->up = columnHeader;
  columnHeader->down = columnHeader;
  columnHeader->columnHeader = columnHeader;
  columnHeader->left = root;
  columnHeader->right = root;
  root->right = columnHeader;
  root->left = columnHeader;

  for (int j = 1; j < numberOfColumns; ++j) {
    Node* nextColumnHeader = new Node();
    //nextColumnHeader->header = true;
    nextColumnHeader->up = nextColumnHeader;
    nextColumnHeader->down = nextColumnHeader;
    nextColumnHeader->columnHeader = nextColumnHeader;
    nextColumnHeader->right = columnHeader->right;
    columnHeader->right->left = nextColumnHeader;
    columnHeader->right =  nextColumnHeader;
    nextColumnHeader->left = columnHeader;
    columnHeader = nextColumnHeader;
  }

  int j = 0;
  Node* rowInColumnNode;
  for (Node* columnHeaderIter = root->right; columnHeaderIter != root; columnHeaderIter = columnHeaderIter->right, ++j) {
    rowInColumnNode = columnHeaderIter;
    for (int i = 0; i < numberOfRows; ++i) {
      if (matrix[i][j] != nullptr) {
        matrix[i][j]->up = rowInColumnNode;
        rowInColumnNode->down = matrix[i][j];
        matrix[i][j]->down = columnHeaderIter;
        columnHeader->up = matrix[i][j];
        matrix[i][j]->columnHeader = columnHeaderIter;
        rowInColumnNode = matrix[i][j];
      }
    }
  }
}

/*
 * Clearing the memory of two-dimensional DLX constraint matrix
 */
SudokuDLXSolver::~SudokuDLXSolver()
{
  Node* rowNode;
  Node* inlineNodeToDelete;
  Node* columnHeader = root->right;
  while(columnHeader != root) {
    rowNode = columnHeader->down;
    while(rowNode->columnHeader != rowNode) {
      inlineNodeToDelete = rowNode->down;
      delete rowNode;
      rowNode = inlineNodeToDelete;
    }
    inlineNodeToDelete = columnHeader->right;
    delete columnHeader;
    columnHeader = inlineNodeToDelete;
  }
  delete solution;
  delete root;
}

/*
 * Printing DLX lists structure
 */
void SudokuDLXSolver::Print()
{
  int totalNodes = 0;
  std::cout << "======= START OF CURRENT STRUCTURE =======" << std::endl;
  int columnCounter = 0;
  for (Node* headerIter = root->right; headerIter != root; headerIter = headerIter-> right, columnCounter++) {
    std::cout << columnCounter << ": ";
    for (Node* inlineNodeIter = headerIter->down; inlineNodeIter != headerIter; inlineNodeIter = inlineNodeIter->down) {
      std::cout << "(" << inlineNodeIter->row << ", " << inlineNodeIter->column << ", " << inlineNodeIter->value << ");  ";
      totalNodes++;
    }
    std::cout << std::endl;
  }
  std::cout << "======= END OF CURRENT STRUCTURE =======" << std::endl;
  std::cout << "Total nodes: " << totalNodes << std::endl;
}

/*
 * Translating stack representation of the solution into human-readable and order-independent vector representation.
 */
std::vector<std::vector<int>> SudokuDLXSolver::SolveWrap(std::vector<std::vector<int>> puzzle)
{
  solution = Solve(puzzle);
  Node next;
  if (solution == nullptr)
  {
    std::cerr << "Solution not found" << std::endl;
    exit(1);
  }
  while(!solution->empty())
  {
    next = solution->top();
    puzzle[next.row][next.column] = next.value + 1;
    solution->pop();
  }
  return puzzle;
}

/*
 * Covering read puzzles, eliminating them from the constraint matrix
 */
std::stack<Node>* SudokuDLXSolver::Solve(std::vector<std::vector<int>> puzzle)
{
  int nextVal;
  Node* inputNodeToCover;
  for (int i = 0;i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      nextVal = puzzle[i][j];
      if (nextVal != 0) {
        inputNodeToCover = FindNode(i, j, nextVal - 1);
        if (inputNodeToCover == nullptr) {
          std::cout << "Error: ("<< i << ", " << j << ", " << nextVal << ") node does not exist" << std::endl;
          return nullptr;
        }

        Cover(inputNodeToCover->columnHeader);
        for (Node* inrowNode = inputNodeToCover->right; inrowNode != inputNodeToCover; inrowNode = inrowNode->right) {
          Cover(inrowNode->columnHeader);
        }
        solution->push(*inputNodeToCover);
      }
    }
  }

  if (Solve())
    return solution;
  else
    return nullptr;
}

/*
 * Cover columns according to Algorithm X
 */
void SudokuDLXSolver::Cover(Node* columnHeaderNode)
{
  Node* columnNode = columnHeaderNode;
  columnNode->right->left = columnNode->left;
  columnNode->left->right = columnNode->right;
  for(Node* rowNodeIter = columnNode->down; rowNodeIter != columnNode; rowNodeIter = rowNodeIter->down) {
    for(Node* inlineNodeIter = rowNodeIter->right; inlineNodeIter != rowNodeIter; inlineNodeIter = inlineNodeIter->right) {
      inlineNodeIter->up->down = inlineNodeIter->down;
      inlineNodeIter->down->up = inlineNodeIter->up;
    }
  }
}

/*
 * Uncover columns according to Algorithm X
 */
void SudokuDLXSolver::Uncover(Node* node)
{
  Node* columnHeader = node->columnHeader;
  columnHeader->right->left = columnHeader;
  columnHeader->left->right = columnHeader;
  for (Node* rowInColumnIter = columnHeader->up; rowInColumnIter != columnHeader; rowInColumnIter = rowInColumnIter->up) {
    for (Node* inlineIter = rowInColumnIter->left; inlineIter != rowInColumnIter; inlineIter = inlineIter->left) {
      inlineIter->up->down = inlineIter;
      inlineIter->down->up = inlineIter;
    }
  }
}

/*
 * Recursively trying different variants choosing lines from constraint matrix
 */
bool SudokuDLXSolver::Solve()
{
  if (root->right == root) {
    return true;
  }

  auto res = FindColumnWithMinNumberOfNodes();
  Node* columnHeaderToCover = res.second;

  if (!res.first)
  {
    return false;
  }

  Cover(columnHeaderToCover);
  for (Node* rowInColumnIter = columnHeaderToCover->down; rowInColumnIter != columnHeaderToCover && !solved; rowInColumnIter = rowInColumnIter->down) {
    solution->push(*rowInColumnIter);
    for (Node* inlineNodeIter = rowInColumnIter->right; inlineNodeIter != rowInColumnIter; inlineNodeIter = inlineNodeIter->right) {
      Cover(inlineNodeIter->columnHeader);
    }

    solved = Solve();
    if (!solved) {
      solution->pop();
    }

    for (Node* inlineNodeIter = rowInColumnIter->right; inlineNodeIter != rowInColumnIter; inlineNodeIter = inlineNodeIter->right) {
      Uncover(inlineNodeIter->columnHeader);
    }
  }
  Uncover(columnHeaderToCover);
  return solved;
}

/*
 * Finds column with the lowest number of nodes as Algorithm X optimization says
 */
std::pair<bool, Node*> SudokuDLXSolver::FindColumnWithMinNumberOfNodes()
{
  Node* columnWithTheLowest;
  int localCounter;
  int minimumNumberOfNodes = -1;
  for (Node* columnHeaderIter = root->right; columnHeaderIter != root; columnHeaderIter = columnHeaderIter->right) {
    localCounter = 0;
    for (Node* rowNodeIter = columnHeaderIter->down; rowNodeIter != columnHeaderIter; rowNodeIter = rowNodeIter->down) {
      localCounter++;
    }
    if (localCounter < minimumNumberOfNodes || minimumNumberOfNodes == -1) {
      columnWithTheLowest = columnHeaderIter;
      minimumNumberOfNodes = localCounter;
    }
  }

  bool enoughNodes = minimumNumberOfNodes > 0;
  return {enoughNodes, columnWithTheLowest};
}

/*
 * Finds node in DLX structure which is responsible for provided row, colum and value combination.
 */
Node* SudokuDLXSolver::FindNode(int r, int c, int v)
{
  for (Node* columnHeaderIter = root->right; columnHeaderIter != root; columnHeaderIter = columnHeaderIter->right) {
    for (Node* rowInColumnIter = columnHeaderIter->down; rowInColumnIter != columnHeaderIter; rowInColumnIter = rowInColumnIter->down) {
      if (rowInColumnIter->row == r && rowInColumnIter->column == c && rowInColumnIter->value == v) {
        return rowInColumnIter;
      }
    }
  }
  return nullptr;
}
