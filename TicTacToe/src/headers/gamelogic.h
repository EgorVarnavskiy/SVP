#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QVector>

class GameLogic : public QObject
{
    Q_OBJECT

public:
    enum Player { PlayerNone, PlayerX, PlayerO };
    enum CellState { CellEmpty, CellX, CellO };
    enum GameState { StatePlaying, StateFinished };

    explicit GameLogic(QObject *parent = nullptr);

    void newGame();
    void makeMove(int row, int col);
    CellState cellState(int row, int col) const;
    GameState gameState() const { return m_gameState; }
    Player currentPlayer() const { return m_currentPlayer; }
    int boardSize() const { return m_boardSize; }
    void setBoardSize(int size);

    QVector<QVector<CellState>> boardState() const { return m_board; }
    Player winner() const { return m_winner; }
    bool isCellEmpty(int row, int col) const;
    int moveCount() const;
    bool isValidMove(int row, int col) const;

signals:
    void boardChanged();
    void gameFinished(Player winner);
    void currentPlayerChanged(Player player);
    void boardSizeChanged();

private:
    bool checkWin(Player player) const;
    bool checkDraw() const;
    void switchPlayer();

    QVector<QVector<CellState>> m_board;
    Player m_currentPlayer;
    Player m_winner;
    GameState m_gameState;
    int m_boardSize;
};

#endif // GAMELOGIC_H
