#ifndef SUDOKUDLXSOLVER_NODE_H
#define SUDOKUDLXSOLVER_NODE_H

struct Node
{
  Node* left;
  Node* right;
  Node* up;
  Node* down;
  Node* columnHeader;
  short int row;
  short int column;
  short int value;

  Node()
  {
    up = down = left = right = columnHeader = nullptr;
    row=column=value=-1;
  }

  Node(int r,int c,int v)
  {
    up = down = left = right = columnHeader = nullptr;
    row = r;
    column = c;
    value = v;
  }
};
#endif
