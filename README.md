# boggle

This program is written in c++. My original code is contained in the 'src' folder within the Boggle.cpp file. All the support files necessary for a clean execution have been included. To execute Boggle.cpp:

1. Download Qt Creator (a free platform) or use any other platform you prefer.
2. Right click on 'RandomWriter.pro' file and direct the cursor to Qt Creator or your preferred platform.
3. Find RandomWriter.cpp (located within the 'src' folder) and execute the code.

The program runs a game of boggle where human and computer meet head-to-head! It is a Stanford CS106B classic. 
The main focus of the assignemnt is on designing and implementing recursive algorithms. At the heart of the program 
are two recursive functions that find words on the board, one for the human player and another for the computer. 

See the testPLayersWordExists method for a recursive backtracking algorithm that "fails fast": as soon as it can be established that a word can't be started from a position on the board the search moves to the next position. This method is "wrapped" in the evaluatePlayersWord method.

See the findBoggleWord method for a recursive algorithm that traverses the entire boggle board and performs an exhaustive search to find all undiscovered words.


Human Player's word meets the following conditions: 

1. It is at least four letters long.
2. It is contained in the English lexicon.
3. It has not already been included in the player’s word list (even if there is an alternate path on the board to form the same word, the word is counted at most once).
4. It can be formed on the board (i.e., it is composed of adjoining letters and each cube is used at most once).





