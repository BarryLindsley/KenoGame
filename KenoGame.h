#ifndef KENOGAME_H
#define KENOGAME_H

#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

//*****************************************************
// KenoGame Class Declaration
//*****************************************************
class KenoGame
{
    public:
        KenoGame();
        void displayMainMenu();
        void displayGameScreen();
        string formatCurrency(string);
        void displayWinnings();
        void displayGameMenu();
        void startNewGame();
        void displayPayoutTables();
        void displayHighScores();
        void quitProgram();
        void eraseMenuFromScreen();

        void createGameBoard();
        void resetUserPicksArray();
        void resetComputerPicksArray();

        void clearInputStream();
        void setPreviousCursorPosition();
        COORD getPreviousCursorPosition();
        void resetCursorPosition();
        void setCursorCoordinate(int, int);
        void promptUserForBet();
        void promptUserForNumPicks();
        void promptUserForSelections();
        void pickComputerNumbers();
        void shuffleComputerPicksArray();
        int checkForMatch(int);
        void checkForWin(int, int);

        bool checkUserSelection(int);
        void addSelectionToArray(int);
        void colorSelectionOnBoard(int, int);

        enum CURSOR_Y_POSITIONS {BOARD = 0, WINNINGS = 9, CHOICE = 11, SELECTIONS = 13, COMPUTER_PICKS = 15};


    private:
        const int BOARD_COLUMNS;
        const int BOARD_ROWS;

        HANDLE screen;
        COORD pos;
        COORD previousPos;
        static bool firstGame;

        int m_winnings;         // Total winnings - update each game
        int m_bet;              // Bet amount for current game
        int m_numPicks;         // Number of picks for current game
        int m_matches;
        int userPicksArray[80];             // Array for tracking user picks
        int computerPicksArray[80];         // Array for tracking random computer picks
        int payoutTable[11][10] =  {{0,0,0,0,0,0,0,0,0,0},
                                    {3,0,0,0,0,0,0,0,0,0},
                                    {0,15,2,2,0,0,0,0,0,0},
                                    {0,0,46,5,3,3,1,0,0,0},
                                    {0,0,0,91,12,4,2,2,1,0},
                                    {0,0,0,0,810,70,21,12,6,5},
                                    {0,0,0,0,0,1600,400,98,44,24},
                                    {0,0,0,0,0,0,7000,1652,335,142},
                                    {0,0,0,0,0,0,0,10000,4700,1000},
                                    {0,0,0,0,0,0,0,0,10000,4500},
                                    {0,0,0,0,0,0,0,0,0,10000}};
        ifstream inputFile;
        ofstream outputFile;

};

#endif // KENOGAME_H
