#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Initialize and generate windows and widgets from the .ui file
    // Initialize grid lines
    for(int i = 0; i < 4; i++){
      hLines[i] = new QFrame(this);
      hLines[i]->setFrameShape(QFrame::HLine);
      hLines[i]->setFrameShadow(QFrame::Plain);
      hLines[i]->setLineWidth(3);
      vLines[i] = new QFrame(this);
      vLines[i]->setFrameShape(QFrame::VLine);
      vLines[i]->setFrameShadow(QFrame::Plain);
      vLines[i]->setLineWidth(3);
    }
    redrawLines();

    // Configure TableWidget
    // This change of StyleSheet prevents the next cell on the right or the cell currently being edited to disappear
    ui->tableWidget->setStyleSheet("QLineEdit { background : transparent }");

    // Settings Combo Boxes
    connect(ui->comboBoxSize, SIGNAL( currentIndexChanged(int) ), this, SLOT( on_sizeChange(int) ) );
    connect(ui->comboBoxDifficulty, SIGNAL( currentIndexChanged(int) ), this, SLOT( on_difficultyChange(int) ) );
    connect(ui->comboBoxPlayers, SIGNAL( currentIndexChanged(int) ), this, SLOT( on_playerChange(int) ) );

    // Generate Button
    connect(ui->pushButtonGenerate, SIGNAL( released() ), this, SLOT( on_buttonRelease() ) );

    // Player Label
    ui->labelActivePlayer->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Redraw the sub-grid lines for new sudoku grids
*/
void MainWindow::redrawLines(){
  double SRN_ = sqrt(N);
  int srn = (int)SRN_;
  int xOffset = ui->tableWidget->geometry().x();
  int yOffset = ui->tableWidget->geometry().y();
  for(int i = 0; i < 4; i++){
    hLines[i]->setGeometry(xOffset, CELLSIZE*srn*(i+1)+yOffset-10, CELLSIZE*N+1, 21);
    vLines[i]->setGeometry(CELLSIZE*srn*(i+1)+xOffset-20, yOffset+0, xOffset+1, CELLSIZE*N+1);
    if(i < srn-1){
      hLines[i]->setVisible(true);
      vLines[i]->setVisible(true);
    }
    else{
      hLines[i]->setVisible(false);
      vLines[i]->setVisible(false);
    }
  }
  repaint();
}

void MainWindow::newSudoku(int n, int k){
    // Disconnect the signal is previously connected, otherwise all numbers from the new grid are checked and marked red
    disconnect(ui->tableWidget, SIGNAL( cellChanged(int, int) ), 0, 0);

    // Set the size of the table matching the sudoku matrix
    ui->tableWidget->setRowCount(n);
    ui->tableWidget->setColumnCount(n);

    // Generate sudoku matrix
    sudoku = new GridGen(n, k);

    // Initialize the Table with the data from the sudoku matrix
    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
        ui->tableWidget->setItem(i, j, new QTableWidgetItem);
        if(sudoku->matrix[i][j] > 0){
          QString str = QString::number(sudoku->matrix[i][j]);
          ui->tableWidget->item(i, j)->setText(str);
          ui->tableWidget->item(i, j)->setFlags(Qt::NoItemFlags); // Makes the cell read-only / cannot be edited by the user
        }
        ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
      }
    }
    // Connect signal to check user inputs into the grid
    connect(ui->tableWidget, SIGNAL( cellChanged(int, int) ), this, SLOT( on_cellChange(int, int) ));
}

void MainWindow::on_cellChange(int nRow, int nCol){
  disconnect(ui->tableWidget, SIGNAL( cellChanged(int, int) ), 0, 0);
  QTableWidgetItem* cell = ui->tableWidget->item(nRow, nCol);
  cell->setForeground(playerColor[activePlayer]);
  if(cell){
    if(cell->text().toInt() > 0){ // Input is an INT [Valid Input]
      if(cell->text().toInt() > sudoku->N){ // Input > 9 or 16 or 25 [Invalid Input]
        sudoku->matrix[nRow][nCol] = 0; // Erase Input from Sudoku matrix
        cell->setText(""); // Erase Input from Table
        cell->setBackground(Qt::NoBrush); // Clear color back to default
      }
      else if(sudoku->isIntAllowed(nRow, nCol, cell->text().toInt()) == false){ // Valid input but incorrect answer
        sudoku->matrix[nRow][nCol] = cell->text().toInt(); // Write input to sudoku matrix
        cell->setBackground(Qt::red); // Change cell to red
        activePlayer = nextPlayer();
      }
      else{ // Valid input and correct answer
        sudoku->matrix[nRow][nCol] = cell->text().toInt(); // Write input to sudoku matrix
        cell->setBackground(Qt::NoBrush); // Reset cell color in table
        if(sudoku->isSolved()){
          createVictoryDialog();
        }
      }
    }
    else{ // Input is not an INT
      sudoku->matrix[nRow][nCol] = 0; // Erase Input from Sudoku matrix
      cell->setText(""); // Erase Input from Table
      cell->setBackground(Qt::NoBrush); // Reset cell color in table
    }
  }
  connect(ui->tableWidget, SIGNAL( cellChanged(int, int) ), this, SLOT( on_cellChange(int, int) ));
}

void MainWindow::on_sizeChange(int idx){
  switch( idx ){
    case 0: // 9x9
      N = 9; break;
    case 1: // 16x16
      N = 16; break;
    case 2: // 25x25
      N = 25; break;
    default:
      N = 9; break;
  }
}

void MainWindow::on_difficultyChange(int idx){
  switch( idx ){
    case 0: // Easy
      K = 3; break;
    case 1: // Medium
      K = 5; break;
    case 2: // Hard
      K = 7; break;
    default:
      K = 3; break;
  }
}

void MainWindow::on_playerChange(int idx){
  switch( idx ){
    case 0:
      P = 1; break;
    case 1:
      P = 2; break;
    case 2:
      P = 3; break;
    case 3:
      P = 4; break;
    default:
      P = 0; break;
  }
}

void MainWindow::on_buttonRelease(){
  int l1 = CELLSIZE*N+1;

  newSudoku(N, N*K);

  // Resize widgets
  ui->tableWidget->setGeometry(ui->tableWidget->geometry().x(), ui->tableWidget->geometry().y(), l1, l1);
  this->setGeometry(this->geometry().x(), this->geometry().y(), ui->tableWidget->geometry().width()+80, ui->tableWidget->geometry().height()+190);
  activePlayer = 0;
  activePlayer = nextPlayer();
  redrawLines();
}

void MainWindow::createVictoryDialog(){
  QDialog *victoryDialog = new QDialog;
  QLabel *victoryLabel = new QLabel(victoryDialog);
  victoryDialog->setWindowTitle("Solved !");
  victoryLabel->setText("Congratulations, you solved this Sudoku !");
  victoryLabel->show();
  victoryDialog->exec();
}

int MainWindow::nextPlayer(){
    int p = activePlayer;
    p++;
    if(p > P){
      p = 1;
    }
    if(P > 1){ // Multi-player case
      ui->labelActivePlayer->setText("It is Player " + QString::number(p) + "'s turn to play");
      ui->labelActivePlayer->setStyleSheet(playerStylesheets[p]);
      ui->labelActivePlayer->setVisible(true);
    }
    else{ // Single player case
      ui->labelActivePlayer->setVisible(false); //hid player turn label
      p = 0; // To keep the text style to system default (no color)
    }
    return p;
}
