#include <iostream>
#include <random>
#include <QApplication>
#include "gridgen.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  // std::cout << "Hello World\n";
  // int N = 9;
  // int K = 20;
  // GridGen* sudoku = new GridGen(N, K);

  // sudoku->printGrid();
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
