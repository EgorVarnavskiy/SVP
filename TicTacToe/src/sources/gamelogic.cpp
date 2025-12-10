#include "gamelogic.h"
#include <QDebug>

GameLogic::GameLogic(QObject *parent)
    : QObject(parent), m_currentPlayer(PlayerX), m_gameState(StatePlaying),
      m_winner(PlayerNone), m_boardSize(3)
{
    newGame();
}

void GameLogic::newGame()
{
    m_board.resize(m_boardSize);
    for (int i = 0; i < m_boardSize; ++i) {
        m_board[i].resize(m_boardSize);
        for (int j = 0; j < m_boardSize; ++j) {
            m_board[i][j] = CellEmpty;
        }
    }

    m_currentPlayer = PlayerX;
    m_gameState = StatePlaying;
    m_winner = PlayerNone;

    emit boardChanged();
    emit currentPlayerChanged(m_currentPlayer);
}

void GameLogic::makeMove(int row, int col)
{
    if (!isValidMove(row, col)) {
        return;
    }

    m_board[row][col] = (m_currentPlayer == PlayerX) ? CellX : CellO;

    if (checkWin(m_currentPlayer)) {
        m_gameState = StateFinished;
        m_winner = m_currentPlayer;
        emit gameFinished(m_winner);
    } else if (checkDraw()) {
        m_gameState = StateFinished;
        m_winner = PlayerNone;
        emit gameFinished(m_winner);
    } else {
        switchPlayer();
    }

    emit boardChanged();
}

GameLogic::CellState GameLogic::cellState(int row, int col) const
{
    if (row < 0 || row >= m_boardSize || col < 0 || col >= m_boardSize) {
        return CellEmpty;
    }
    return m_board[row][col];
}

bool GameLogic::isCellEmpty(int row, int col) const
{
    if (row < 0 || row >= m_boardSize || col < 0 || col >= m_boardSize) {
        return false;
    }
    return m_board[row][col] == CellEmpty;
}

bool GameLogic::isValidMove(int row, int col) const
{
    return m_gameState == StatePlaying &&
           row >= 0 && row < m_boardSize &&
           col >= 0 && col < m_boardSize &&
           m_board[row][col] == CellEmpty;
}

int GameLogic::moveCount() const
{
    int count = 0;
    for (int i = 0; i < m_boardSize; ++i) {
        for (int j = 0; j < m_boardSize; ++j) {
            if (m_board[i][j] != CellEmpty) {
                count++;
            }
        }
    }
    return count;
}

void GameLogic::setBoardSize(int size)
{
    if (size >= 3 && size <= 10 && size != m_boardSize) {
        m_boardSize = size;
        newGame();
        emit boardSizeChanged();
    }
}

bool GameLogic::checkWin(Player player) const
{
    CellState target = (player == PlayerX) ? CellX : CellO;

    for (int i = 0; i < m_boardSize; ++i) {
        bool rowWin = true;
        bool colWin = true;

        for (int j = 0; j < m_boardSize; ++j) {
            if (m_board[i][j] != target) rowWin = false;
            if (m_board[j][i] != target) colWin = false;
        }

        if (rowWin || colWin) return true;
    }

    bool diag1Win = true;
    bool diag2Win = true;

    for (int i = 0; i < m_boardSize; ++i) {
        if (m_board[i][i] != target) diag1Win = false;
        if (m_board[i][m_boardSize - 1 - i] != target) diag2Win = false;
    }

    return diag1Win || diag2Win;
}

bool GameLogic::checkDraw() const
{
    for (int i = 0; i < m_boardSize; ++i) {
        for (int j = 0; j < m_boardSize; ++j) {
            if (m_board[i][j] == CellEmpty) {
                return false;
            }
        }
    }
    return true;
}

void GameLogic::switchPlayer()
{
    m_currentPlayer = (m_currentPlayer == PlayerX) ? PlayerO : PlayerX;
    emit currentPlayerChanged(m_currentPlayer);
}
