#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameLogic(new GameLogic(this)),
      scoreX(0), scoreO(0), scoreDraw(0)
{
    setupUI();
    onNewGame();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *controlLayout = new QHBoxLayout();

    newGameButton = new QPushButton("Новая игра", this);
    boardSizeSpinBox = new QSpinBox(this);
    boardSizeSpinBox->setRange(3, 10);
    boardSizeSpinBox->setValue(3);
    boardSizeSpinBox->setPrefix("Размер: ");

    controlLayout->addWidget(newGameButton);
    controlLayout->addWidget(boardSizeSpinBox);
    controlLayout->addStretch();

    QHBoxLayout *statusLayout = new QHBoxLayout();

    currentPlayerLabel = new QLabel("Ход: X", this);
    scoreXLabel = new QLabel("X: 0", this);
    scoreOLabel = new QLabel("O: 0", this);
    scoreDrawLabel = new QLabel("Ничьи: 0", this);

    statusLayout->addWidget(currentPlayerLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(scoreXLabel);
    statusLayout->addWidget(scoreOLabel);
    statusLayout->addWidget(scoreDrawLabel);

    gameBoard = new GameBoard(this);
    gameBoard->setGameLogic(gameLogic);

    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(statusLayout);
    mainLayout->addWidget(gameBoard, 1);

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(boardSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onBoardSizeChanged);
    connect(gameLogic, &GameLogic::gameFinished, this, &MainWindow::onGameFinished);
    connect(gameLogic, &GameLogic::currentPlayerChanged, [this](GameLogic::Player player) {
        currentPlayerLabel->setText(player == GameLogic::PlayerX ? "Ход: X" : "Ход: O");
    });
    connect(gameLogic, &GameLogic::boardSizeChanged, gameBoard, &GameBoard::updateSize);
}

void MainWindow::onNewGame()
{
    gameLogic->newGame();
    gameBoard->update();
    currentPlayerLabel->setText("Ход: X");
}

void MainWindow::onGameFinished(GameLogic::Player winner)
{
    QString message;

    if (winner == GameLogic::PlayerX) {
        scoreX++;
        message = "Победил X!";
    } else if (winner == GameLogic::PlayerO) {
        scoreO++;
        message = "Победил O!";
    } else {
        scoreDraw++;
        message = "Ничья!";
    }

    updateScores();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Игра окончена");
    msgBox.setText(message);
    msgBox.setInformativeText("Начать новую игру?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    if (msgBox.exec() == QMessageBox::Yes) {
        onNewGame();
    }
}

void MainWindow::onBoardSizeChanged(int size)
{
    gameLogic->setBoardSize(size);
    onNewGame();
}

void MainWindow::updateScores()
{
    scoreXLabel->setText(QString("X: %1").arg(scoreX));
    scoreOLabel->setText(QString("O: %1").arg(scoreO));
    scoreDrawLabel->setText(QString("Ничьи: %1").arg(scoreDraw));
}
