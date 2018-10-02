#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  render();
  startGame();
}

void clearGameBoard(char (&gameBoard)[3][3]) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      gameBoard[i][j] = '\0';
}

bool canUpdateGameBoard(char (&gameBoard)[3][3], int row, int column) {
  return gameBoard[row][column] == '\0';
}

void updateGameBoard(char (&gameBoard)[3][3],
                     int row,
                     int column,
                     char player) {
  gameBoard[row][column] = player;
}

bool gameBoardHasWinner(char (&gameBoard)[3][3]) {
  bool hasWinner;

  hasWinner = (gameBoard[0][0] == gameBoard[1][1] &&
               gameBoard[1][1] == gameBoard[2][2] && gameBoard[2][2] != '\0') ||
              (gameBoard[0][2] == gameBoard[1][1] &&
               gameBoard[1][1] == gameBoard[2][0] && gameBoard[2][0] != '\0');

  for (int i = 0; i < 3 && !hasWinner; i++)
    hasWinner =
        (gameBoard[i][0] == gameBoard[i][1] &&
         gameBoard[i][1] == gameBoard[i][2] && gameBoard[i][2] != '\0') ||
        (gameBoard[0][i] == gameBoard[1][i] &&
         gameBoard[1][i] == gameBoard[2][i] && gameBoard[2][i] != '\0');

  return hasWinner;
}

bool gameBoardHasDraw(char (&gameBoard)[3][3]) {
  bool hasDraw = true;

  for (int i = 0; i < 3 && hasDraw; i++)
    for (int j = 0; j < 3 && hasDraw; j++)
      hasDraw = gameBoard[i][j] != '\0';

  return hasDraw;
}

void MainWindow::render() {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      QPushButton* button = new QPushButton(this);
      button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      ui->gridLayout->addWidget(button, i, j);
      connect(button, &QPushButton::clicked,
              [this, button, i, j]() { mark(button, i, j); });
    }

  connect(ui->restartButton, SIGNAL(clicked()), this, SLOT(restartGame()));
}

void MainWindow::startGame() {
  gameOver = false;
  clearGameBoard(gameBoard);
  setPlayer('X');
}

void MainWindow::mark(QPushButton* button, int row, int column) {
  if (canUpdateGameBoard(gameBoard, row, column) && !gameOver) {
    updateGameBoard(gameBoard, row, column, currentPlayer);
    button->setText(QString(currentPlayer));

    if (gameBoardHasWinner(gameBoard)) {
      gameOver = true;
      endGameWithWinner();
    } else if (gameBoardHasDraw(gameBoard)) {
      gameOver = true;
      endGameWithDraw();
    } else
      nextPlayer();
  }
}

void MainWindow::endGameWithWinner() {
  ui->gameStatusLabel->setText(QString("%1 is the winner!").arg(currentPlayer));
}

void MainWindow::endGameWithDraw() {
  ui->gameStatusLabel->setText(QString("It's a draw!"));
}

void MainWindow::nextPlayer() {
  setPlayer(((currentPlayer == 'X') ? 'O' : 'X'));
}

void MainWindow::setPlayer(char player) {
  currentPlayer = player;
  ui->gameStatusLabel->setText(QString("%1 turn").arg(currentPlayer));
}

void MainWindow::restartGame() {
  QList<QPushButton*> buttons =
      ui->gridLayoutWidget->findChildren<QPushButton*>();
  for (const auto button : buttons)
    button->setText(QString(""));

  startGame();
}

MainWindow::~MainWindow() {
  delete ui;
}
