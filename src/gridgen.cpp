/* Sudoku grid generator */
#include <bits/stdc++.h>
#include "gridgen.h"
#include <random>
#include <stdio.h>
#include <time.h>

GridGen::GridGen(int n, int k){
  N = n;
  K = k;

  // Calculate Square-root or N
  double SRN_ = sqrt(N);
  SRN = (int)SRN_;

  matrix = new int*[N]; // Array of point to create the 2-D array
  for(int i = 0; i < N; i++){
    matrix[i] = new int[N];
    // Initialize all entries as false
    memset(matrix[i], 0, N * sizeof(int));
  }
  fillDiagSubGrids();
  fillRestSubGrids();
  removeNumber();
};

/*
Generate a single random integer r, between 1 and a specified maximum
1 <= r <= max
*/
int randomInteger(int max){
  //srand(time(NULL));
  return 1 + (rand() % static_cast<int>(max));
};

void GridGen::fillDiagSubGrids(){
  for(int i = 0; i < N; i += SRN){
    fillSubGrid(i, i);
  }
};

void GridGen::fillSubGrid(int row, int col){
  int number;
  for(int i = 0; i < SRN; i++){
    for(int j = 0; j < SRN; j++){
      do {
        number = randomInteger(N);
      } while (!isIntAllowedInSubGrid(row, col, number));
      matrix[row+i][col+j] = number;
    }
  }
};

/* Search for empty cell in the sudoku matrix
Update the values of input arguments i and j with the indices of the first empty cell found and return true,
unless no empty cell exists in the matrix and therefore return false.
*/
bool GridGen::findEmptyCell(int& i, int& j){
  for(i = 0; i < N; i++){
    for(j = 0; j < N; j++){
      if(matrix[i][j] == 0){
        return true;
      }
    }
  }
  return false;
};

/* Fill the empty cells in the sudoku matrix with valid numbers, recursively using "backtracking" algorithm
*/
bool GridGen::fillRestSubGrids(){
  int i, j;
  if(!findEmptyCell(i, j)){
    return true; // All cells are filled
  }
  for(int n = 1; n <= N; n++){
    if(isIntAllowed(i, j, n)){
      matrix[i][j] = n;
      if(fillRestSubGrids()){
        return true;
      }
      matrix[i][j] = 0; //reset cell when conditions are not OK
    }
  }
  return false;
};

bool GridGen::isIntAllowed(int i, int j, int num){
  return (isIntAllowedInRow(i, num)
       && isIntAllowedInCol(j, num)
       && isIntAllowedInSubGrid(i - i % SRN, j - j % SRN, num)
  );
};

/*
Check if we are allowed to place the number num in the subgrid
return true if num is not present in subgrid
return false if num is already present in subgrid
*/
bool GridGen::isIntAllowedInSubGrid(int rowStart, int colStart, int num){
 for(int i = 0; i < SRN; i++){
    for(int j = 0; j < SRN; j++){
      if(matrix[rowStart+i][colStart+j] == num){
        return false;
      }
    }
  }
  return true;
};

/* Check if a number num is allowed on the row i according to sudoku rules
return true if allowed and false otherwise
*/
bool GridGen::isIntAllowedInRow(int i, int num){
  for (int j = 0; j < N; j++){
    if(matrix[i][j] == num){
      return false;
    }
  }
  return true;
};

/* Check if a number num is allowed on the column j according to sudoku rules
return true if allowed and false otherwise
*/
bool GridGen::isIntAllowedInCol(int j, int num){
  for (int i = 0; i < N; i++){
    if(matrix[i][j] == num){
      return false;
    }
  }
  return true;
};

/* Remove number from grid
*/
void GridGen::removeNumber(){
  int k = K;
  while(k != 0){
    int index = randomInteger(N*N)-1;
    int i = index/N;
    int j = index%N;
    if (j != 0){
      j -= 1;
    }
    if(matrix[i][j] != 0){
      k--;
      matrix[i][j] = 0;
    }
  }
}

/* Print Sudoku GridGen
*/
void GridGen::printGrid(){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      std::cout << matrix[i][j] << "\t";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool GridGen::isSolved(){
    int i = 0; int j = 0; int num = 0;
    // if we find any empty cell, then the sudoku is not complete
    if(findEmptyCell(i, j)){
      return false;
    }
    // Else, the grid is complete, run full verification
    else{
      for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
          // Because the method isIntAllowed with check all cells,
          // the number under test need to be removed from the sudoku matrix for testing,
          // and put back in afterwards
          num = matrix[i][j];
          matrix[i][j] = 0;
          // If any incorrect answer is found in the grid, return false
          if(!isIntAllowed(i, j, num)){
            matrix[i][j] = num;
            return false;
          }
          matrix[i][j] = num;
        }
      }
      // We found no mistake, the grid is complete and correct
      return true;
    }
}
