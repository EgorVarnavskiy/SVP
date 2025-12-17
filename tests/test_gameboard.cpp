#include <QtTest>
#include "gameboard.h"
#include "gamelogic.h"

class TestGameBoard : public QObject
{
    Q_OBJECT

private slots:
    void testGameBoardCreation();
};

void TestGameBoard::testGameBoardCreation()
{
    GameBoard board;
    GameLogic logic;

    board.setGameLogic(&logic);
    QVERIFY(board.sizeHint().isValid());
}

QTEST_MAIN(TestGameBoard)
#include "test_gameboard.moc"
