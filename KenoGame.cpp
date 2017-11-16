#include "KenoGame.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

//*****************************************************
// Keno Game Class Implementation
//*****************************************************
bool KenoGame::firstGame = true;

KenoGame::KenoGame() : BOARD_COLUMNS(5), BOARD_ROWS(4), m_winnings(10000), m_numPicks(0), previousPos({0,0}), m_matches(0)
{
    // Get HANDLE for screen
    screen = GetStdHandle(STD_OUTPUT_HANDLE);

    // Initialize arrays to 0
    resetUserPicksArray();
    resetComputerPicksArray();

    // If first game, display main menu immediately
    if (firstGame){
        displayMainMenu();
    }

}   // End constructor

void KenoGame::displayMainMenu(){
    // Program Title
    cout << string(75, '*') << endl << endl;
    cout << "\t\t\t\tKENO" << endl << endl;
    cout << string(75, '*') << endl << endl;

    // Instructions
    cout << "Keno is a lottery-type game where the computer will randomly choose 20" << endl;
    cout << "numbers between 1 and 80 for each game. Before the game begins, you decide" << endl;
    cout << "how many numbers you'd like to play (between 1 and 10), and then choose" << endl;
    cout << "your numbers. If you match a certain number of your choices, you win. See" << endl;
    cout << "the winning and payout tables in the game for more information. Good luck!" << endl << endl;
    cout << string(75, '*') << endl << endl;

    displayHighScores();
    cout << "\nPress <Enter> to play: ";
    cin.get();

    firstGame = false;      // Set firstGame flag so instructions don't appear again

    system("CLS");      // Clear screen

    // Display game screen w/ game board
    displayGameScreen();

}   // End displayMainMenu function

void KenoGame::displayGameScreen(){
    // Set up Game Board
    createGameBoard();

    // Display Winnings
    displayWinnings();

    // Display Game Menu
    displayGameMenu();

}   // End displayGameScreen function

string KenoGame::formatCurrency(string str){
    string wkgString = "$";
    for (int i = 0; i < str.length(); i++){
        if ((str.length() - i) % 3 == 0 && i != 0){
            wkgString += ",";
        }
        wkgString += str[i];
    }
    return wkgString;

}   // End formatCurrency function

void KenoGame::createGameBoard(){
    // Draw game board
    pos = {0,BOARD};     // Start Position
    for (int i = 0; i < (BOARD_ROWS * 2) + 1; i++){
        for (int j = 0; j < BOARD_COLUMNS; j++){
            if (i % 2 == 0){
                cout << "+----";
            } else {
                int number = (BOARD_COLUMNS * ((i - 1)/2)) + j + 1;
                if (i == 1 && j < 9){
                    cout << "| 0" << number << " ";            // Print numbers 1 to 9
                } else {
                    cout << "| " << number << " ";             // Print numbers 10 through 80
                }
            }
        }   // End columns for loop

        if (i % 2 == 0){
            cout << "+" << endl;        // Final character and endline for border rows
        } else {
            cout << "|" << endl;        // Final character and endline for number rows
        }

    }   // End rows for loop

}   // End createGameBoard function

void KenoGame::displayWinnings(){
    setCursorCoordinate(0, WINNINGS);
    resetCursorPosition();
    cout << "Current winnings:  " << formatCurrency(to_string(m_winnings));

}   // End displayWinnings function

void KenoGame::displayGameMenu(){
    eraseMenuFromScreen();
    setCursorCoordinate(0, CHOICE);
    resetCursorPosition();
    cout << "Start New Game <N>" << endl;
    cout << "View Payout Table <P>" << endl;
    cout << "Show High Scores <H>" << endl;
    cout << "Quit Program <Q>" << endl << endl;

    cout << "Please select an option and press <Enter>: ";

    char ch = cin.get();
    ch = toupper(ch);

    while (cin.fail() || (ch != 'N' && ch != 'P' && ch != 'H' && ch != 'Q')){
        clearInputStream();
        setCursorCoordinate(0, CHOICE + 5);
        resetCursorPosition();
        cout << "Invalid entry. Please select an option and press <Enter>: ";
        ch = cin.get();
        ch = toupper(ch);
    }

    clearInputStream();     // Clear input stream to avoid errors with future prompts

    switch(ch){
        case 'N':
            startNewGame();
            break;
        case 'P':
            displayPayoutTables();
            cout << "Press <Enter> to return to game menu: " << endl;
            cin.get();

            system("CLS");
            displayGameScreen();
            break;
        case 'H':
            displayHighScores();
            cout << "\nPress <Enter> to return to game menu:" << endl;
            cin.get();
            displayGameMenu();
            break;
        case 'Q':
            quitProgram();
            break;
    }

}   // End displayGameMenu function

void KenoGame::startNewGame(){
    // Erase game menu from bottom half of screen
    eraseMenuFromScreen();

    // Prompt user for bet
    promptUserForBet();

    // Prompt user for number of picks
    promptUserForNumPicks();

    // Prompt user for selections
    promptUserForSelections();

    // Computer selects 20 random numbers
    pickComputerNumbers();

    // Check for win
    checkForWin(m_numPicks, m_matches);

}   // End startNewGame function

void KenoGame::displayPayoutTables(){
    system("CLS");
    // Table Heading
    cout << "To calculate winnings, multiply bet by payout shown below: " << endl << endl;
    // Column Headings
    cout << setw(8) << right << "PICKS" << setw(12) << right << "CATCHES" << setw(13) << right << "PAYOUT " <<
            setw(16) << right << "PICKS" << setw(12) << right << "CATCHES" << setw(13) << right << "PAYOUT " << endl;
    cout << setw(8) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" <<
            setw(16) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;

    // Table Data
    cout << setw(8) << right << "10 " << setw(12) << right << "10  " << setw(13) << right << "10000 " <<
            setw(16) << right << "6  " << setw(12) << right << "6   " << setw(13) << right << "1600 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "9  " << setw(13) << right << "4500 " <<
            setw(16) << right << "   " << setw(12) << right << "5   " << setw(13) << right << "70 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "8  " << setw(13) << right << "1000 " <<
            setw(16) << right << "   " << setw(12) << right << "4   " << setw(13) << right << "4 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "7  " << setw(13) << right << "142 " <<
            setw(16) << right << "   " << setw(12) << right << "3   " << setw(13) << right << "3 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "6  " << setw(13) << right << "24 " <<
            setw(16) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "5  " << setw(13) << right << "5 " <<
            setw(16) << right << "5  " << setw(12) << right << "5   " << setw(13) << right << "810 " << endl;
    cout << setw(8) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" <<
            setw(16) << right << "   " << setw(12) << right << "4   " << setw(13) << right << "12 " << endl;
    cout << setw(8) << right << "9  " << setw(12) << right << "9  " << setw(13) << right << "10000 " <<
            setw(16) << right << "   " << setw(12) << right << "3   " << setw(13) << right << "3 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "8  " << setw(13) << right << "4700 " <<
            setw(16) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "7  " << setw(13) << right << "335 " <<
            setw(16) << right << "4  " << setw(12) << right << "4   " << setw(13) << right << "91 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "6  " << setw(13) << right << "44 " <<
            setw(16) << right << "   " << setw(12) << right << "3   " << setw(13) << right << "5 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "5  " << setw(13) << right << "6 " <<
            setw(16) << right << "   " << setw(12) << right << "2   " << setw(13) << right << "2 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "4  " << setw(13) << right << "1 " <<
            setw(16) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;
    cout << setw(8) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" <<
            setw(16) << right << "3  " << setw(12) << right << "3   " << setw(13) << right << "46 " << endl;
    cout << setw(8) << right << "8  " << setw(12) << right << "8  " << setw(13) << right << "10000 " <<
            setw(16) << right << "   " << setw(12) << right << "2   " << setw(13) << right << "2 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "7  " << setw(13) << right << "1652 " <<
            setw(16) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "6  " << setw(13) << right << "98 " <<
            setw(16) << right << "2  " << setw(12) << right << "2   " << setw(13) << right << "15 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "5  " << setw(13) << right << "12 " <<
            setw(16) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "4  " << setw(13) << right << "2 " <<
            setw(16) << right << "1  " << setw(12) << right << "1   " << setw(13) << right << "3 " << endl;
    cout << setw(8) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" <<
            setw(16) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;
    cout << setw(8) << right << "7  " << setw(12) << right << "7  " << setw(13) << right << "7000 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "6  " << setw(13) << right << "400 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "5  " << setw(13) << right << "21 " << endl;
    cout << setw(8) << right << "   " << setw(12) << right << "4  " << setw(13) << right << "2 " << endl;
    cout << setw(8) << right << "-----" << setw(12) << right << "-------" << setw(13) << right << "--------" << endl;

}   // End displayPayoutTables function

void KenoGame::displayHighScores(){
    if (!firstGame){
        // Erase game menu from bottom half of screen
        eraseMenuFromScreen();

        // Set cursor position
        setCursorCoordinate(0, CHOICE);
        resetCursorPosition();
    }

    // Display profile options
    cout << "\tAll-Time High Scores" << endl;
    cout << "\t--------------------" << endl;
    inputFile.open("HighScores.txt");
    string name = "";
    string money = "";

    if (inputFile){
        int counter = 1;
        while (inputFile >> name){
            cout << setw(5) << right << counter << ".   " << setw(10) << left << name << "     ";
            inputFile >> money;
            cout << setw(12) << right << formatCurrency(money) << endl;
            counter++;
        }
    }
    inputFile.close();

}   // End displayHighScores function

void KenoGame::quitProgram(){
    cout << "Thank you for playing. Goodbye." << endl;
    exit(2);

}   // End quitProgram function

void KenoGame::eraseMenuFromScreen(){
    setCursorCoordinate(0, CHOICE);
    resetCursorPosition();
    for (int i = 0; i < 10; i++){
        cout << string(70, ' ') << endl;
    }

    resetCursorPosition();

}   // End eraseMenuFromScreen function

void KenoGame::promptUserForBet(){
    setCursorCoordinate(0, CHOICE);         // Set cursor position to number of picks position
    resetCursorPosition();

    cout << "Enter the amount you would like to bet: ";
    cin >> m_bet;

    while (cin.fail() || m_bet < 0 || m_bet > m_winnings){
        clearInputStream();
        resetCursorPosition();
        cout << "Invalid input. Please enter a value between 0 and " << m_winnings << ": ";
        cin >> m_bet;
    }

    m_winnings -= m_bet;
    displayWinnings();
    cout << "    Bet Amount: " << formatCurrency(to_string(m_bet));


}   // End promptUserForBet function

void KenoGame::promptUserForNumPicks(){
    setCursorCoordinate(0, CHOICE);         // Set cursor position to number of picks position
    resetCursorPosition();

    cout << "Enter your number of picks (between 1 and 10): ";
    cin >> m_numPicks;

    while (cin.fail() || m_numPicks < 1 || m_numPicks > 10){
        clearInputStream();
        resetCursorPosition();
        cout << "Invalid input. Please enter a value between 1 and 10: ";
        cin >> m_numPicks;
    }

    cout << endl;       // Skip line after valid number entered

}   // End promptUserForNumPicks function

void KenoGame::promptUserForSelections(){
    setCursorCoordinate(0, SELECTIONS);          // Set cursor position to position for selections
    resetCursorPosition();

    int pickCount = 1;
    int selection = 0;

    for (pickCount; pickCount <= m_numPicks; pickCount++){
        cout << "Please enter Selection #" << pickCount << ": ";
        cin >> selection;

        if (!cin.fail()){
            checkUserSelection(selection);
        }

        while (cin.fail() || !checkUserSelection(selection)){
            clearInputStream();
            resetCursorPosition();
            cout << "Invalid selection. Please make another choice for Selection #" << pickCount << ": ";
            cin >> selection;

            if (!cin.fail()){
                checkUserSelection(selection);
            }
        }

        addSelectionToArray(selection);
        colorSelectionOnBoard(selection, 6);        // Color user selections YELLOW
        resetCursorPosition();
    }

}   // End promptUserForSelections function

void KenoGame::clearInputStream(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

}   // End clearInputStream function

void KenoGame::setPreviousCursorPosition(){
    int x = 0;
    int y = 0;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        x = csbi.dwCursorPosition.X;
        y = csbi.dwCursorPosition.Y;
    }

    previousPos = {x, y};            // Save current cursor position to temp

}   // End setPreviousCursorPosition

COORD KenoGame::getPreviousCursorPosition(){
    return previousPos;

}   // End getPreviousCursorPosition

void KenoGame::resetCursorPosition(){
    SetConsoleCursorPosition(screen, pos);
    cout << string(70, ' ') << "\r";

}   // End resetCursorPosition function

void KenoGame::setCursorCoordinate(int x, int y){
    pos = {x, y};

}   // End setCursorCoordinate

void KenoGame::resetUserPicksArray(){
    for (int i = 0; i < (BOARD_COLUMNS * BOARD_ROWS); i++){
        userPicksArray[i] = 0;
    }

}   // End resetUserPicksArray function

void KenoGame::resetComputerPicksArray(){
    for (int i = 0; i < (BOARD_COLUMNS * BOARD_ROWS); i++){
        computerPicksArray[i] = i + 1;
    }

}   // End resetComputerPicksArray function

bool KenoGame::checkUserSelection(int num){
    if (num < 1 || num > (BOARD_COLUMNS * BOARD_ROWS)){
        return false;
    } else {
        if (userPicksArray[num - 1] == 1){
            return false;
        } else {
            return true;
        }
    }

}   // End checkUserSelection();

void KenoGame::addSelectionToArray(int num){
    userPicksArray[num-1] = 1;

}   // End addSelectionToArray function

void KenoGame::colorSelectionOnBoard(int num, int color){
    // Set print color
    SetConsoleTextAttribute(screen, color);

    setPreviousCursorPosition();

    // Calculate cursor position
    pos = {(((num - 1) % BOARD_COLUMNS) * 5) + 2, (((num - 1) / BOARD_COLUMNS) * 2) + 1};
    SetConsoleCursorPosition(screen, pos);

    // Format text if number is less than 10
    if (num < 10){
        cout << "0" << num;
    } else {
        cout << num;
    }

    // Reset previous values
    SetConsoleTextAttribute(screen, 7);     // Reset color to WHITE
    setCursorCoordinate(getPreviousCursorPosition().X, getPreviousCursorPosition().Y);    // Reset previous cursor position from temp
    resetCursorPosition();

}   // End colorSelectionOnBoard

void KenoGame::pickComputerNumbers(){
    shuffleComputerPicksArray();
    cout << "Numbers Selected: ";
    for (int i = 0; i < 20; i++){
        colorSelectionOnBoard(computerPicksArray[i], checkForMatch(computerPicksArray[i]));
        cout << computerPicksArray[i] << " ";
        //cout << "Random number #" << (i + 1) << ": " << computerPicksArray[i] << endl;    // DEBUGGING
        Sleep(1200);
    }

}   // End pickComputerNumbers function

void KenoGame::shuffleComputerPicksArray(){
    // Seed random number
    unsigned seed;
    seed = time(0);
    srand(seed);

    for (int i = 0; i < (BOARD_COLUMNS * BOARD_ROWS) - 1; i++){
        int random = 0;
        do {
            random = rand() % (BOARD_COLUMNS * BOARD_ROWS);
            //cout << "Working random number chosen: " << random << "    i: " << i << endl;     // DEBUGGING
        } while (random <= i);      // Continue picking random value until it is greater than current index (i)

        //cout << "Random number chosen: " << random << endl;       // DEBUGGING

        // Swap value at random index with value at current index
        int temp = computerPicksArray[random];
        computerPicksArray[random] = computerPicksArray[i];
        computerPicksArray[i] = temp;

    }

}   // End shuffleComputerPicksArray function

int KenoGame::checkForMatch(int num){
    if (userPicksArray[num - 1] == 1){
        m_matches++;          // Increment match variable if match found
        return 10;          // Return value for BRIGHT GREEN if it matches a user selection
    } else {
        return 12;          // Return value for BRIGHT RED if it does no match a user selection
    }

}   // End checkForMatch function

void KenoGame::checkForWin(int picks, int matches){
    int win = 0;
    cout << "\nPayout table value: " << payoutTable[matches][picks-1];
    win = payoutTable[matches][picks-1] * m_bet;
    if (win > 0){
        cout << "\nYou won " << formatCurrency(to_string(win)) << "!";
        m_winnings += win;
        displayWinnings();
    }

}   // End checkForWin function
