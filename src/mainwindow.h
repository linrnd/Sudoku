#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include "gridgen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GridGen* sudoku;

private slots:
  void on_cellChange(int nRow, int nCol);
  void on_sizeChange(int idx);
  void on_difficultyChange(int idx);
  void on_playerChange(int idx);
  void on_buttonRelease();

private:
  // UI global varaibles
  int CELLSIZE = 50;
  Ui::MainWindow *ui;
  QFrame *hLines[4];
  QFrame *vLines[4];
  QString playerStylesheets[5] = {"", "QLabel { color : magenta; }", "QLabel { color : green; }", "QLabel { color : blue; }", "QLabel { color : cyan; }"};
  QBrush playerColor[5] = {Qt::NoBrush, Qt::magenta, Qt::green, Qt::blue, Qt::cyan};
  // Game's global variables
  int N = 9;  //Sudoku Size
  int K = 3;  //Difficulty: Number of cells to complete = N*K
  int P = 1;  //Number of Players
  int activePlayer = 0;
  // Methods
  void redrawLines();
  void newSudoku(int n, int k);
  void createVictoryDialog();
  int nextPlayer();
};
#endif // MAINWINDOW_H
