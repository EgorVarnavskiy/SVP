#include <QtTest>
#include "gamelogic.h"

class TestGameLogic : public QObject
{
    Q_OBJECT

private slots:
    void testInitialState();
    void testMakeMove();
    void testWinConditions();
    void testDrawCondition();
    void testBoardSizeChange();
    void testIsCellEmpty();
    void testIsValidMove();
    void testMoveCount();
    void testBoardState();
    void testWinner();
};

void TestGameLogic::testInitialState()
{
    GameLogic logic;
    QCOMPARE(logic.gameState(), GameLogic::StatePlaying);
    QCOMPARE(logic.currentPlayer(), GameLogic::PlayerX);
    QCOMPARE(logic.cellState(0, 0), GameLogic::CellEmpty);
    QCOMPARE(logic.isCellEmpty(0, 0), true);
    QCOMPARE(logic.moveCount(), 0);
    QCOMPARE(logic.winner(), GameLogic::PlayerNone);
}

void TestGameLogic::testMakeMove()
{
    GameLogic logic;
    logic.makeMove(0, 0);
    QCOMPARE(logic.cellState(0, 0), GameLogic::CellX);
    QCOMPARE(logic.currentPlayer(), GameLogic::PlayerO);
    QCOMPARE(logic.isCellEmpty(0, 0), false);
    QCOMPARE(logic.moveCount(), 1);
}

void TestGameLogic::testWinConditions()
{
    GameLogic logic;

    logic.makeMove(0, 0); // X
    logic.makeMove(1, 0); // O
    logic.makeMove(0, 1); // X
    logic.makeMove(1, 1); // O
    logic.makeMove(0, 2); // X - победа

    QCOMPARE(logic.gameState(), GameLogic::StateFinished);
    QCOMPARE(logic.winner(), GameLogic::PlayerX);
}

void TestGameLogic::testDrawCondition()
{
    GameLogic logic;
    logic.setBoardSize(3);
    logic.makeMove(0, 0); // X
    logic.makeMove(0, 1); // O
    logic.makeMove(0, 2); // X
    logic.makeMove(1, 0); // O
    logic.makeMove(1, 2); // X
    logic.makeMove(1, 1); // O
    logic.makeMove(2, 0); // X
    logic.makeMove(2, 2); // O
    logic.makeMove(2, 1); // X - ничья

    QCOMPARE(logic.gameState(), GameLogic::StateFinished);
    QCOMPARE(logic.winner(), GameLogic::PlayerNone);
    QCOMPARE(logic.moveCount(), 9);
}

void TestGameLogic::testBoardSizeChange()
{
    GameLogic logic;
    logic.setBoardSize(5);
    QCOMPARE(logic.boardSize(), 5);
    QCOMPARE(logic.moveCount(), 0);

    auto board = logic.boardState();
    QCOMPARE(board.size(), 5);
    QCOMPARE(board[0].size(), 5);

    QCOMPARE(logic.isValidMove(4, 4), true); // Последняя клетка
    logic.makeMove(4, 4);
    QCOMPARE(logic.cellState(4, 4), GameLogic::CellX);
}

void TestGameLogic::testIsCellEmpty()
{
    GameLogic logic;

    QCOMPARE(logic.isCellEmpty(0, 0), true);
    QCOMPARE(logic.isCellEmpty(1, 1), true);

    logic.makeMove(0, 0);

    QCOMPARE(logic.isCellEmpty(0, 0), false);
    QCOMPARE(logic.isCellEmpty(-1, 0), false);
    QCOMPARE(logic.isCellEmpty(0, 10), false);
}

void TestGameLogic::testIsValidMove()
{
    GameLogic logic;

    QCOMPARE(logic.isValidMove(0, 0), true);

    logic.makeMove(0, 0);

    QCOMPARE(logic.isValidMove(0, 0), false);

    QCOMPARE(logic.isValidMove(0, 1), true);

    QCOMPARE(logic.isValidMove(-1, 0), false);
    QCOMPARE(logic.isValidMove(10, 10), false);
}

void TestGameLogic::testMoveCount()
{
    GameLogic logic;
    logic.setBoardSize(3);

    QCOMPARE(logic.moveCount(), 0);

    logic.makeMove(0, 0);
    QCOMPARE(logic.moveCount(), 1);

    logic.makeMove(0, 1);
    QCOMPARE(logic.moveCount(), 2);

    logic.makeMove(1, 1);
    QCOMPARE(logic.moveCount(), 3);

    logic.makeMove(2, 2);
    QCOMPARE(logic.moveCount(), 4);

    logic.makeMove(0, 2);
    QCOMPARE(logic.moveCount(), 5);

    logic.makeMove(2, 0);
    QCOMPARE(logic.moveCount(), 6);

    logic.makeMove(1, 0);
    QCOMPARE(logic.moveCount(), 7);

    logic.makeMove(1, 2);
    QCOMPARE(logic.moveCount(), 8);

    logic.makeMove(2, 1);
    QCOMPARE(logic.moveCount(), 9);

    QCOMPARE(logic.gameState(), GameLogic::StateFinished);
}

void TestGameLogic::testBoardState()
{
    GameLogic logic;
    logic.setBoardSize(3);

    auto board = logic.boardState();
    QCOMPARE(board.size(), 3);
    QCOMPARE(board[0].size(), 3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QCOMPARE(board[i][j], GameLogic::CellEmpty);
        }
    }

    logic.makeMove(0, 0);
    board = logic.boardState();
    QCOMPARE(board[0][0], GameLogic::CellX);
    QCOMPARE(board[0][1], GameLogic::CellEmpty);
    QCOMPARE(board[0][2], GameLogic::CellEmpty);
    QCOMPARE(board[1][0], GameLogic::CellEmpty);
    QCOMPARE(board[1][1], GameLogic::CellEmpty);
    QCOMPARE(board[1][2], GameLogic::CellEmpty);
    QCOMPARE(board[2][0], GameLogic::CellEmpty);
    QCOMPARE(board[2][1], GameLogic::CellEmpty);
    QCOMPARE(board[2][2], GameLogic::CellEmpty);
}

void TestGameLogic::testWinner()
{
    GameLogic logic;

    QCOMPARE(logic.winner(), GameLogic::PlayerNone);

    logic.makeMove(0, 0); // X
    logic.makeMove(1, 0); // O
    logic.makeMove(0, 1); // X
    logic.makeMove(1, 1); // O
    logic.makeMove(0, 2); // X - победа

    QCOMPARE(logic.winner(), GameLogic::PlayerX);
    QCOMPARE(logic.gameState(), GameLogic::StateFinished);

    GameLogic logic2;
    logic2.setBoardSize(3);

    logic2.makeMove(0, 0); // X
    logic2.makeMove(0, 1); // O
    logic2.makeMove(0, 2); // X
    logic2.makeMove(1, 1); // O
    logic2.makeMove(1, 0); // X
    logic2.makeMove(1, 2); // O
    logic2.makeMove(2, 1); // X
    logic2.makeMove(2, 0); // O
    logic2.makeMove(2, 2); // X - ничья

    QCOMPARE(logic2.gameState(), GameLogic::StateFinished);
    QCOMPARE(logic2.winner(), GameLogic::PlayerNone);
}

QTEST_APPLESS_MAIN(TestGameLogic)
#include "test_gamelogic.moc"
