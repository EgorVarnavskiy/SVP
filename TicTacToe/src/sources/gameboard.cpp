#include "gameboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent), gameLogic(nullptr)
{
    setMinimumSize(240, 240);
}

void GameBoard::setGameLogic(GameLogic *logic)
{
    gameLogic = logic;
    connect(gameLogic, &GameLogic::boardChanged, this, QOverload<>::of(&GameBoard::update));
    if (gameLogic) {
        connect(gameLogic, &GameLogic::boardSizeChanged, this, &GameBoard::updateSize);
    }
}

QSize GameBoard::sizeHint() const
{
    if (!gameLogic) return QSize(300, 300);

    int size = gameLogic->boardSize();
    int totalSize = size * cellSize();
    return QSize(totalSize, totalSize);
}

void GameBoard::updateSize()
{
    updateGeometry();
    update();
}

int GameBoard::cellSize() const
{
    if (!gameLogic) return 80;

    int size = gameLogic->boardSize();

    if (size <= 3) return 80;
    if (size <= 5) return 60;
    if (size <= 7) return 45;
    return 35;
}

void GameBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!gameLogic) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Фон
    painter.fillRect(rect(), Qt::white);

    drawGrid(painter);
    drawSymbols(painter);
}

void GameBoard::drawGrid(QPainter &painter)
{
    int size = gameLogic->boardSize();
    int cellSizeValue = cellSize();

    painter.setPen(QPen(Qt::black, 2));

    for (int i = 1; i < size; ++i) {
        int x = i * cellSizeValue;
        painter.drawLine(x, 0, x, size * cellSizeValue);
    }

    for (int i = 1; i < size; ++i) {
        int y = i * cellSizeValue;
        painter.drawLine(0, y, size * cellSizeValue, y);
    }
}

void GameBoard::drawSymbols(QPainter &painter)
{
    int size = gameLogic->boardSize();
    int cellSizeValue = cellSize();

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            GameLogic::CellState cell = gameLogic->cellState(row, col);
            QRect rect = cellRect(row, col);

            if (cell == GameLogic::CellX) {
                painter.setPen(QPen(Qt::red, 4));
                painter.drawLine(rect.topLeft(), rect.bottomRight());
                painter.drawLine(rect.topRight(), rect.bottomLeft());
            } else if (cell == GameLogic::CellO) {
                painter.setPen(QPen(Qt::blue, 4));
                painter.drawEllipse(rect.adjusted(5, 5, -5, -5));
            }
        }
    }
}

QRect GameBoard::cellRect(int row, int col) const
{
    int cellSizeValue = cellSize();
    return QRect(col * cellSizeValue, row * cellSizeValue, cellSizeValue, cellSizeValue);
}

QPoint GameBoard::cellAtPosition(const QPoint &pos) const
{
    int cellSizeValue = cellSize();
    int row = pos.y() / cellSizeValue;
    int col = pos.x() / cellSizeValue;
    return QPoint(row, col);
}
void GameBoard::mousePressEvent(QMouseEvent *event)
{
    if (!gameLogic || gameLogic->gameState() != GameLogic::StatePlaying) {
        return;
    }

    QPoint cell = cellAtPosition(event->pos());
    int size = gameLogic->boardSize();

    if (cell.x() >= 0 && cell.x() < size && cell.y() >= 0 && cell.y() < size) {
        gameLogic->makeMove(cell.x(), cell.y());
    }
}
