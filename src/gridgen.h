/* Sudoku grid generator - header file*/
#ifndef GridGen_H
#define GridGen_H

class GridGen {
public:
  /* Attributes */
  int** matrix; // 2-D Array for the complete Grid
  int N; // Grid size as the number of cells in each of the Sudoku sub-grids
         // e.g: N=9, 3x3 sub-grids, for a Grid of 9x9
  int SRN; // Sub-grid size, as the Square-root of N
  int K;  // Number of empty cells in the Sudoku grid

  /* Public methods */
  GridGen(int n, int k); // Constructor
  void printGrid(); // Print the complete grid
  bool isIntAllowed(int i, int j, int num);
  bool isSolved();
private:
  void fillDiagSubGrids();
  bool findEmptyCell(int& i, int& j);
  bool fillRestSubGrids();
  void fillSubGrid (int row, int col);
  bool isIntAllowedInSubGrid (int rowStart, int colStart, int num);
  bool isIntAllowedInRow(int i, int num);
  bool isIntAllowedInCol(int j, int num);
  void removeNumber();
};
#endif
