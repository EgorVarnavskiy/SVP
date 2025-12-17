#include "gameboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent), gameLogic(nullptr)
{
    setMinimumSize(240, 240);
    setStyleSheet("background-color: #1e1e1e;");
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

    // Фон с градиентом
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor("#2a2a2a"));
    gradient.setColorAt(1, QColor("#1a1a1a"));
    painter.fillRect(rect(), gradient);

    drawGrid(painter);
    drawSymbols(painter);
}

void GameBoard::drawGrid(QPainter &painter)
{
    int size = gameLogic->boardSize();
    int cellSizeValue = cellSize();

    painter.setPen(QPen(QColor("#4a4a4a"), 3));

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
                // X с градиентом
                QLinearGradient xGradient(rect.topLeft(), rect.bottomRight());
                xGradient.setColorAt(0, QColor("#ff6b6b"));
                xGradient.setColorAt(1, QColor("#ff4757"));
                painter.setPen(QPen(xGradient, 5));
                painter.setBrush(Qt::NoBrush);

                int padding = cellSizeValue / 8;
                painter.drawLine(rect.topLeft() + QPoint(padding, padding),
                               rect.bottomRight() - QPoint(padding, padding));
                painter.drawLine(rect.topRight() + QPoint(-padding, padding),
                               rect.bottomLeft() + QPoint(padding, -padding));
            } else if (cell == GameLogic::CellO) {
                // O с градиентом
                QLinearGradient oGradient(rect.topLeft(), rect.bottomRight());
                oGradient.setColorAt(0, QColor("#4a9cff"));
                oGradient.setColorAt(1, QColor("#3742fa"));
                painter.setPen(QPen(oGradient, 5));
                painter.setBrush(Qt::NoBrush);

                int padding = cellSizeValue / 8;
                painter.drawEllipse(rect.adjusted(padding, padding, -padding, -padding));
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
