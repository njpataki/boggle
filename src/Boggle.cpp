/*
 * File: Boggle.cpp
 * ----------------
 * Name: Nicholas Josef Pataki
 */

#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;
const int MINIMUM_WORD_LENGTH = 4;
const int STANDARD = 4;
const int BIG_BOGGLE = 5;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
void goodbye();
void setUpBoard(Grid<char> &boggleGrid);
void createNonrandomBoard(int n, Grid<char> &boggleGrid);
void createRandomBoard(int n, Grid<char> &boggleGrid);
void fillBoggleGrid(int n, string letters, Grid<char> &boggleGrid);
void humansTurn(Set<string> &playersWordList, Lexicon &english, Grid<char> &boggleGrid);
void testWordConditions(string &playersWord, Lexicon &english, Set<string> playersWordList);
void highlightPlayersWord(Grid<char> &booleanGrid);
void computersTurn(Set<string> &computersWordList, Set<string> &playersWordList,
                   Lexicon &english, Grid <char> &boggleGrid);
void findBoggleWords(string word, int currentRow, int currentCol,
                     Grid<char> &boggleGrid, Grid<bool> &booleanGrid,
                     Set<string> &computersWordList, Set<string> &playersWordList,
                     Lexicon &english);
void printPlayersWords(Set<string> &wordList);
int chooseBoardSize();
bool evaluatePlayersWord(string &playersWord, Grid<char> &boggleGrid);
bool testPlayersWordExists(string word, int currentRow, int currentCol,
                           Grid<char> &boggleGrid, Grid<char> &booleanGrid);
Vector<string> shuffleCubes(int n);
string nonRandomLetters(int n);
string createRandomLetters(Vector<string> vec );
string getPlayersWord();


/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    Grid<char> boggleGrid;
    Lexicon english("EnglishWords.dat");
    Set<string> playersWordList;
    Set<string> computersWordList;
    welcome();
    do {
        cout << endl;
        cout << "Do you want instructions? ";
        if(getYesOrNo())  giveInstructions();
        setUpBoard(boggleGrid);
        humansTurn(playersWordList, english, boggleGrid);
        computersTurn(computersWordList, playersWordList, english, boggleGrid);
        cout << endl;
        cout << "Do you want to play again? ";
       } while(getYesOrNo());
    goodbye();
    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

/*
 * Function: void setUpBoard();
 * Usage: setUpBoard();
 * -----------------
 * This function acts as a wrapper function for a number of other functions that will prompt the
 * user for his/her preference on board size and utilizing random letters or user-provided letters.
 *
 */

void setUpBoard(Grid<char> &boggleGrid) {
    int n = chooseBoardSize();
    boggleGrid.resize(n,n);
    cout << endl;
    cout << "I'll give you a chance to set up the board to your specification, ";
    cout << "which makes it easier to confirm the boggle program is working correctly." << endl;
    cout << endl;
    cout << "Do you want to force the board configuration? ";
    if(getYesOrNo()) {
        createNonrandomBoard(n, boggleGrid);
    } else {
        createRandomBoard(n, boggleGrid);
    }
    cout << endl;
    cout << "Ok, take all the time you want and find all the words you can!" << endl;
    cout << "Signal that you're finished by entering an empty line." << endl;
}

/*
 * Function: int chooseBoardSize()
 * Usage: int n = chooseBoardSize();
 * -----------------
 * This function asks the user if they would like to play on a 4x4
 * or 5x5 board. Since the board is a square it returns the the size
 * of one side as an integer.
 */

int chooseBoardSize() {
    cout << endl;
    cout << "You can choose standard Boggle (4x4 grid) or Big Boggle (5x5)." << endl;
    cout << endl;
    cout << "Would you like Big Boggle? ";
    if(getYesOrNo()) { return BIG_BOGGLE;} else { return STANDARD;}
}

/*
 * Function: void createNonrandomBoard(int n, Grid<char> &boggleGrid)
 * Usage: createNonrandomBoard(n, boggleGrid);
 * -----------------
 * This function creates a boggle board on the console and places letters given by the user onto the board.
 * If the board size is 5x5 then the first 5 letters are the cubes starting with the top row
 * from left to right, the next 5 letters are the second row, and so on.
 */

void createNonrandomBoard(int n, Grid<char> &boggleGrid) {
    string letters = toUpperCase(nonRandomLetters(n));
    drawBoard(n,n);
    fillBoggleGrid(n, letters, boggleGrid);
}

/*
 * Function: string nonRandomLetters(int n)
 * Usage: string letters = forcedBoardLetters(n);
 * -----------------
 * This function asks user for letters to fill the boggle board. If the user does not enter enough
 * letters then s/he is prompted for at least the right amount, which will be n^2.
 */

string nonRandomLetters(int n) {
    cout << endl;
    int numLetters = n*n;
    cout << endl;
    cout << "Enter a " << numLetters << "-character string to identify which letters you want on the cubes. ";
    cout << "The first " << n << " letters are the cubes on the top row from left to right, the next ";
    cout <<  n << " letters are the second row, and so on." << endl;
    cout << "Enter the string: ";
    string letters = getLine();
    while(letters.size() < numLetters) {
        cout << "Please enter at least " << numLetters << "characters!" << endl;
        cout << "Try again: ";
        letters = getLine();
    }
    string boggleLetters = letters.substr(0,numLetters);
    return boggleLetters;
}

/*
 * Function: void fillBoggleGrid(int n, string letters, Grid<char> &boggleGrid)
 * Usage: fillBoggleGrid(int n, letters, boggleGrid);
 * -----------------
 * This function takes in a string array of characters and places each leter on the boggle board
 * on the console in order starting at the top and moving by row from left to right.
 * The argument n represents the length of one row (column).
 *
 * This function also takes in an object of the Grid class that is called by reference and places
 * each of those characters into the grid. This grid will exactly match the Boggle board on the console.
 */

void fillBoggleGrid(int n, string letters, Grid<char> &boggleGrid) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i==0) {
                labelCube(i,j,letters[j]);
                boggleGrid.set(i,j,letters[j]);
            } else {
                labelCube(i,j,letters[i*n+j]);
                boggleGrid.set(i,j,letters[i*n+j]);
            }
        }
    }
}

/*
 * Function: void createRandomBoard(int n, Grid<char> &boggleGrid)
 * Usage: createRandomBoard(n, boggleGrid);
 * -----------------
 * This function creates a boggle board on the console and randomly draws letters for that board.
 * First the n*n cubes are shaken to randomly order them on the board and then a letter from each side
 * is randomly chosen to land face up. If the board size is 5x5 then the first 5 letters are the cubes
 * starting with the top row from left to right, the next 5 letters are the second row, and so on.
 */

void createRandomBoard(int n, Grid<char> &boggleGrid) {    
    Vector<string> vec = shuffleCubes(n);
    string letters = createRandomLetters(vec);
    drawBoard(n,n);
    fillBoggleGrid(n, letters, boggleGrid);
}

/*
 * Function: Vector<string> rearrangeCubes(int n)
 * Usage: Vector<string> vec = rearrangeCubes(n);
 * -----------------
 * This function utilizes the argument 'n' to decide whether to use standard boggle cubes (n=4)
 * or big boggle cubes (n=5). Each cube is stored as a string array within each element of the vector
 * and then the vector is randomly "shaken" to rearrange the position of each cube.
 */

Vector<string> shuffleCubes(int n) {
    Vector<string> vec(n*n);
    if(n==4) {
        for(int i = 0; i < n*n ; i++) {
            vec[i] = STANDARD_CUBES[i];
        }
    } else {
        for(int i = 0; i < n*n ; i++) {
            vec[i] = BIG_BOGGLE_CUBES[i];
        }
    }
    int randomIndex;
    for(int i = 0; i < vec.size(); i++) {
        string temp = vec[i];
        randomIndex = randomInteger(i,vec.size()-1);
        vec[i] = vec[randomIndex];
        vec[randomIndex] = temp;
    }
    return vec;
}

/*
 * Function: string createRandomLetters(Vector<string> vec);
 * Usage: string letters = createRandomLetters(vec);
 * -----------------
 * This function takes in a vector of strings where each element holds the
 * letters that belong to each side of a boggle dice. It returns a string of
 * vec.size() length where each character at letter[i] is a randomly drawn letter
 * from the dice represented by vec[i].
 */

string createRandomLetters(Vector<string> vec) {
    int randomDiceIndex;
    string letters = "";
    for(int i = 0; i < vec.size(); i++) {
        randomDiceIndex = randomInteger(0,5);
        letters += (vec[i][randomDiceIndex]);
    }
    return letters;
}


/*
 * Function: string getPlayersWord();
 * Usage: string playersWord = getPlayersWord();
 * -----------------
 * This function retrieves a word from the player
 */

string getPlayersWord() {
    cout << endl;
    cout << "Enter a word: ";
    string playersWord = getLine();
    return playersWord;
}

/*
 * Function: void humansTurn(Set<string> &playersWordList,
 *                           Lexicon &english, Grid<char> &boggleGrid);
 * Usage: umansTurn(pplayersWordList, english, boggleGrid)
 * -----------------
 * This function acts as a wrapper function for a human players turn.
 */


void humansTurn(Set<string> &playersWordList,
                Lexicon &english, Grid<char> &boggleGrid) {
    string playersWord = toUpperCase(getPlayersWord());
    if(playersWord != "") {
        testWordConditions(playersWord, english, playersWordList);
    }
    while(playersWord != "") {
        if(evaluatePlayersWord(playersWord, boggleGrid)) {
            recordWordForPlayer(playersWord, HUMAN);
            playersWordList.add(playersWord);
        }
        playersWord = toUpperCase(getPlayersWord());
        testWordConditions(playersWord, english, playersWordList);
    }
}


/*
 * Function: void testWordConditions(string word);
 * Usage: void testWord(word);
 * -----------------
 * This function takes in a word provided by the player and tests it against three conditions:
 * 1. is the word less than four letters long?
 * 2. is it an actual word in the english language?
 * 3. has it already been used by the player?
 *
 * It ensures that the player enters a word that meets one of the conditions. If the player enters
 * an empty line then the method ends and the computer takes its turn.
 *
 */

void testWordConditions(string &playersWord, Lexicon &english, Set<string> playersWordList) {
    while(playersWord.size() < MINIMUM_WORD_LENGTH && playersWord != "") {
        cout << endl;
        cout << "That word does not meet the minimum word length." << endl;
        cout << "Enter a word: ";
        playersWord = getLine();
        if(playersWord=="") break;
    }
    while(!english.contains(playersWord) && playersWord != "") {
        cout << endl;
        cout << "That is not a word!" << endl;
        cout << "Enter a word: ";
        playersWord = getLine();
        if(playersWord=="") break;
    }
    while(playersWordList.contains(playersWord) && playersWord != "") {
        cout << endl;
        cout << "I'm sorry but you've already played that word." << endl;
        cout << "Enter a word: ";
        playersWord = getLine();
        if(playersWord=="") break;
    }
}


/*
 * Function: bool evaluatePlayersWord(&playersWord, &boggleGrid);
 * Usage: bool evaluatePlayersWord());
 * -----------------
 * This function acts as a wrapper for the recursive function that does the heavy lifting when
 * evaluating whether the players word can actually be formed on the boggle board. It loops through
 * every set of coordinates on the grid to evaluate whether that square contains the first letter
 * of the playersWord. If that first letter is not found the function returns false. If that letter is found
 * the first character of the playersWord is removed and the remainder of the string is sent into the
 * recursive function to do the rest of the work. If no match is found then
 *
 */

bool evaluatePlayersWord(string &playersWord, Grid<char> &boggleGrid) {
    string word = playersWord;
    Grid<char> booleanGrid(boggleGrid.numRows(), boggleGrid.numCols());
    for(int i = 0; i < boggleGrid.numRows(); i++) {
        for(int j = 0; j < boggleGrid.numCols(); j++) {
            if(boggleGrid[i][j]==word[0]) {
                string newWord = word.substr(1,word.length()-1);
                booleanGrid[i][j] = true;
                if(testPlayersWordExists(newWord, i, j, boggleGrid, booleanGrid)) return true;
                booleanGrid[i][j] = false;
            }
        }
    }
    return false;
}


/*
 * Function: bool testPlayersWordExists(string word, int currentRow, int currentCol,
 *                                      Grid<char> &boggleGrid, Grid<char> &booleanGrid);
 * Usage: testPlayersWordExists(newWord, i, j, boggleGrid, booleanGrid);
 * -----------------
 * This recursive function returns "true" if a word entered by a human player can be found on the
 * current game's boggle board. The function passes:
 *
 * 1.   A word string which has its first character removed if that character is found.
 * 2.   Two integers to store the location of the search match in the wrapper function. These coordinates
 *      identify where the first letter of the players word is found on the boggle board.
 * 3.   The current game's boggle grid. This grid is called by reference
 * 4.   A boolean grid which matches the boggle grid's dimensions. If a square is "true" this means that
 *      the letter on the matching square on the boggle board has already been used. This grid is also
 *      called by reference.
 *
 * The for loops search the 3x3 grid that the matched character from the wrapper function (or from a successful
 * successful recursive call) sits in the middle of. If an adjoining square contains the next letter in the
 * word the recursive call resets that 3x3 grid so that the newly found character is in the middle of the search grid.
 *
 */

bool testPlayersWordExists(string word, int currentRow, int currentCol,
                           Grid<char> &boggleGrid, Grid<char> &booleanGrid) {
    for(int i = currentRow-1; i <= currentRow+1; i++) {
        for(int j = currentCol-1; j <= currentCol+1; j++) {
            if(boggleGrid.inBounds(i,j) && boggleGrid[i][j] == word[0] && booleanGrid[i][j] != true) {
                if(word.length() == 1) {
                    booleanGrid[i][j] = true;
                    highlightPlayersWord(booleanGrid);
                    return true;
                }
                if(word.length() > 1) {
                    booleanGrid[i][j] = true;
                    string newWord = word.substr(1,word.length()-1);
                    if(testPlayersWordExists(newWord, i, j, boggleGrid, booleanGrid)) return true;
                }
            booleanGrid[i][j] = false;
            }
        }
    }
    return false;
}

/*
 * Function: highlightPlayersWord(Grid<char> &booleanGrid)
 * Usage: highlightPlayersWord(booleanGrid);
 * -----------------
 * This function highlights the word the player has entered if it is contained on the board.
 */


void highlightPlayersWord(Grid<char> &booleanGrid) {
    for(int i = 0; i < booleanGrid.numRows(); i++) {
        for(int j = 0; j < booleanGrid.numRows(); j++) {
            if(booleanGrid[i][j]==true) {
                highlightCube(i,j,true);
                pause(10);
                highlightCube(i,j,false);
            }
        }
    }
}



/*
 * Function: void computersTurn(Set<string> &computersWordList, Set<string> &playersWordList,
                   Lexicon &english, Grid <char> &boggleGrid)
 * Usage: computersTurn(computersWordList, playersWordList, english, boggleGrid);
 * -----------------
 * This function acts as a wrapper for the recursive function that does the heavy lifting when
 * evaluating what words the computer can find on the boggle board.
 *
 */

void computersTurn(Set<string> &computersWordList, Set<string> &playersWordList,
                   Lexicon &english, Grid <char> &boggleGrid) {
    Grid<bool> booleanGrid(boggleGrid.numRows(), boggleGrid.numCols());
    for(int i = 0; i < boggleGrid.numRows(); i++) {
        for(int j = 0; j < boggleGrid.numCols(); j++) {
            string word = "";
            word += boggleGrid[i][j];
            booleanGrid[i][j] = true;
            findBoggleWords(word, i, j, boggleGrid, booleanGrid, computersWordList, playersWordList, english);
            booleanGrid[i][j] = false;
        }
    }
    printPlayersWords(computersWordList);
}

/*
 * Function: findBoggleWords(string word, int i, int j,
                     Grid<char> &boggleGrid, Grid<bool> &booleanGrid,
                     Set<string> &computersWordList, Set<string> &playersWordList
                     Lexicon &english);
 * Usage: findBoggleWords(word, i, j, boggleGrid, booleanGrid,computersWordList);
 * -----------------
 * This recursive function adds a word to the ComputersWordList if that word can be found on the current game's
 * boggle board, if it hasn't been played already, and if it meets the MINIMUM_WORD_LENGTH.
 * The function passes as arguments:
 *
 * 1.   A word string which initially contains the character passed by the wrapper function as it loops through
 *      every character lying face up on the boggle board.
 * 2.   Two integers to store the location of the search match in the wrapper function. These coordinates
 *      identify where the first letter of the current word being built is found on the boggle board.
 * 3.   The current game's boggle grid. This grid is called by reference
 * 4.   A boolean grid which matches the boggle grid's dimensions. If a square is "true" this means that
 *      the letter on the matching square on the boggle board has already been used. This grid is also
 *      called by reference.
 * 5.   A set containing all the words the computer has found, if any. This set is called by reference.
 * 6.   A set containing all the words the human player found in his/her turn. This set is called by reference.
 * 7.   The english lexicon for testing letter combinations as the algorithm searches combinations on the
 *      boggle board. This lexicon is called by reference.
 *
 * The for loops search the 3x3 grid that the last character in the word string sits in the middle of. As the function
 * passes over the neighboring squares it checks if the prefix that the current word string represents is contained in
 * the english lexicon. If it is, that character is appended to the word string and the function is called recursively.
 *
 * The base case is as follows:
 *
 * 1.   If the english lexicon contains the word, the playersWordList does not contain the word, and the computer has
 *      not already found the word, i.e. it is not contained in the computersWordList then the word is added to
 *      the computersWordList.
 * 2.   If the english lexicon does not contain the prefix that the word string currently represents the search path is killed.
 *
 */


void findBoggleWords(string word, int currentRow, int currentCol,
                     Grid<char> &boggleGrid, Grid<bool> &booleanGrid,
                     Set<string> &computersWordList, Set<string> &playersWordList,
                     Lexicon &english) {
    if((word.length() >= MINIMUM_WORD_LENGTH) && !playersWordList.contains(word) &&
            !computersWordList.contains(word) && english.contains(word)) {
        computersWordList.add(word);
    }
    for(int i = currentRow-1; i <= currentRow+1; i++) {
        for(int j = currentCol-1; j <= currentCol+1; j++) {
            if(boggleGrid.inBounds(i,j) && english.containsPrefix(word) && !booleanGrid[i][j]) {
                word += boggleGrid[i][j];
                booleanGrid[i][j] = true;
                findBoggleWords(word, i, j, boggleGrid, booleanGrid, computersWordList, playersWordList, english);
                booleanGrid[i][j] = false;
                word = word.substr(0, word.length() - 1);
            }
        }
    }
}

/*
 * Function: void printPlayersWords(Set<string> &playersWordList);
 * Usage: printPlayersWords(playersWordList);
 * -----------------
 * This function prints all the words contained in the players word list, if any exist,
 * which is fairly unlikely ...
 *
 *
*/

void printPlayersWords(Set<string> &wordList) {
    for (string word : wordList) {
        recordWordForPlayer(word, COMPUTER);
    }
}

/*
 * Function: goodbye
 * Usage: goodbye();
 * --------------------------
 * Say goodbye to the user.
 */

void goodbye() {
    cout << endl;
    cout << "Thank you for playing. I hope the experience ";
    cout << "wasn't too painful!" << endl;
}

