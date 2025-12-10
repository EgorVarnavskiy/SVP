#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include "gameboard.h"
#include "gamelogic.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onNewGame();
    void onGameFinished(GameLogic::Player winner);
    void onBoardSizeChanged(int size);

private:
    void setupUI();
    void updateScores();

    GameBoard *gameBoard;
    GameLogic *gameLogic;

    QLabel *scoreXLabel;
    QLabel *scoreOLabel;
    QLabel *scoreDrawLabel;
    QLabel *currentPlayerLabel;
    QSpinBox *boardSizeSpinBox;
    QPushButton *newGameButton;

    int scoreX;
    int scoreO;
    int scoreDraw;
};

#endif // MAINWINDOW_H
