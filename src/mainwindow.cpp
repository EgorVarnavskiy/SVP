#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameLogic(new GameLogic(this)),
      scoreX(0), scoreO(0), scoreDraw(0)
{
    setupUI();
    onNewGame();

    setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e1e;
        }
        QWidget#centralWidget {
            background-color: #1e1e1e;
        }
        QPushButton {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #4a9cff, stop:1 #3742fa);
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 6px;
            font-weight: bold;
            font-size: 12px;
            min-height: 30px;
        }
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #5aaaff, stop:1 #4752ff);
        }
        QPushButton:pressed {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3a8cff, stop:1 #2732ff);
        }
        QSpinBox {
            background-color: #2b2b2b;
            color: white;
            border: 2px solid #4a4a4a;
            padding: 6px;
            border-radius: 4px;
            selection-background-color: #4a9cff;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            background-color: #3a3a3a;
            border: 1px solid #4a4a4a;
            border-radius: 2px;
        }
        QSpinBox::up-button:hover, QSpinBox::down-button:hover {
            background-color: #4a4a4a;
        }
        QLabel {
            color: #e0e0e0;
            font-weight: bold;
        }
        QLabel#currentPlayerLabel {
            color: #ff6b6b;
            font-size: 14px;
            font-weight: bold;
        }
        QLabel#scoreXLabel {
            color: #ff6b6b;
            font-weight: bold;
            font-size: 13px;
        }
        QLabel#scoreOLabel {
            color: #4a9cff;
            font-weight: bold;
            font-size: 13px;
        }
        QLabel#scoreDrawLabel {
            color: #aaaaaa;
            font-weight: bold;
            font-size: 13px;
        }
    )");
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    newGameButton = new QPushButton("НОВАЯ ИГРА", this);
    boardSizeSpinBox = new QSpinBox(this);
    boardSizeSpinBox->setRange(3, 10);
    boardSizeSpinBox->setValue(3);
    boardSizeSpinBox->setPrefix("Размер: ");

    controlLayout->addWidget(newGameButton);
    controlLayout->addWidget(boardSizeSpinBox);
    controlLayout->addStretch();

    QHBoxLayout *statusLayout = new QHBoxLayout();

    currentPlayerLabel = new QLabel(" Ход: X", this);
    currentPlayerLabel->setObjectName("currentPlayerLabel");

    scoreXLabel = new QLabel(" X: 0", this);
    scoreXLabel->setObjectName("scoreXLabel");

    scoreOLabel = new QLabel(" O: 0", this);
    scoreOLabel->setObjectName("scoreOLabel");

    scoreDrawLabel = new QLabel("Ничьи: 0", this);
    scoreDrawLabel->setObjectName("scoreDrawLabel");

    statusLayout->addWidget(currentPlayerLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(scoreXLabel);
    statusLayout->addSpacing(15);
    statusLayout->addWidget(scoreOLabel);
    statusLayout->addSpacing(15);
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
        QString color = (player == GameLogic::PlayerX) ? "#ff6b6b" : "#4a9cff";
        QString symbol = (player == GameLogic::PlayerX) ? "X" : "O";
        currentPlayerLabel->setText(QString("<span style='color: %1'> Ход: %2 %3</span>")
                                   .arg(color)
                                   .arg(symbol)
                                   .arg(player == GameLogic::PlayerX ? "X" : "O"));
    });
    connect(gameLogic, &GameLogic::boardSizeChanged, gameBoard, &GameBoard::updateSize);

    QFont titleFont("Arial", 12, QFont::Bold);
    QFont scoreFont("Arial", 11, QFont::Bold);

    currentPlayerLabel->setFont(titleFont);
    scoreXLabel->setFont(scoreFont);
    scoreOLabel->setFont(scoreFont);
    scoreDrawLabel->setFont(scoreFont);

    resize(800, 650);
}

void MainWindow::onNewGame()
{
    gameLogic->newGame();
    gameBoard->update();
    currentPlayerLabel->setText("Ход: X");
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
    scoreDrawLabel->setText(QString(" Ничьи: %1").arg(scoreDraw));
}

void MainWindow::onGameFinished(GameLogic::Player winner)
{
    QString message;

    if (winner == GameLogic::PlayerX) {
        scoreX++;
        message = " ПОБЕДИТЕЛЬ: X!";
    } else if (winner == GameLogic::PlayerO) {
        scoreO++;
        message = " ПОБЕДИТЕЛЬ: O!";
    } else {
        scoreDraw++;
        message = " НИЧЬЯ!";
    }

    updateScores();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Игра окончена");
    msgBox.setText(QString("<center><b>%1</b></center>").arg(message));
    msgBox.setInformativeText("Начать новую игру?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    msgBox.setStyleSheet(R"(
        QMessageBox {
            background-color: #2b2b2b;
        }
        QMessageBox QLabel {
            color: white;
            font-size: 14px;
        }
        QMessageBox QPushButton {
            background-color: #4a9cff;
            color: white;
            border: none;
            padding: 8px 20px;
            border-radius: 4px;
            font-weight: bold;
            min-width: 80px;
        }
        QMessageBox QPushButton:hover {
            background-color: #5aaaff;
        }
    )");

    if (msgBox.exec() == QMessageBox::Yes) {
        onNewGame();
    }
}
