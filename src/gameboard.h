#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include "gamelogic.h"

class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr);
    void setGameLogic(GameLogic *logic);
    QSize sizeHint() const override;

public slots:
    void updateSize();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void drawGrid(QPainter &painter);
    void drawSymbols(QPainter &painter);
    QRect cellRect(int row, int col) const;
    QPoint cellAtPosition(const QPoint &pos) const;
    int cellSize() const;

    GameLogic *gameLogic;
};

#endif // GAMEBOARD_H
