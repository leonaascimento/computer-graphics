#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 public slots:
  void restartGame();

 private:
  Ui::MainWindow* ui;
  char currentPlayer;
  char gameBoard[3][3];
  bool gameOver;
  void render();
  void mark(QPushButton* button, int row, int column);
  void endGameWithWinner();
  void endGameWithDraw();
  void nextPlayer();
  void startGame();
  void setPlayer(char player);
};

#endif  // MAINWINDOW_H
