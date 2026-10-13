//Program Name: Assignment 4 - Sudoku Solver 
//Description: This program solves sudoku puzzles using recursion and backtracking.
//Input: Five text files named puzzle1.txt through puzzle5.txt. Each file contains a 9x9 Sudoku puzzle, using underscores for empty cells.
//Output: The original puzzle, all solutions found, and the total number of solutions, or a message stating that no solution was found.
//Sources/Collaborators: Google Gemini was my genAI I went off of. As well as ChatGPT
//Author: Saylor Statler
//Date Created: October 12, 2026
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class SudokuSolver
{
private:
    char board[9][9];
    int solutionCount;
    //check whether a number can be placed in a cell
    bool isValid(int row, int col, char num)
    {
        //check row and column
        for (int i = 0; i < 9; i++)
        {
            if (board[row][i] == num)
                return false;

            if (board[i][col] == num)
                return false;
        }
        //check 3x3 block
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        for (int r = startRow; r < startRow + 3; r++)
        {
            for (int c = startCol; c < startCol + 3; c++)
            {
                if (board[r][c] == num)
                    return false;
            }
        }

        return true;
    }
    //check whether the original puzzle is valid
    bool initialPuzzleValid()
    {
        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                if (board[r][c] != '_')
                {
                    char num = board[r][c];

                    board[r][c] = '_';
                    bool valid = isValid(r, c, num);
                    board[r][c] = num;

                    if (!valid)
                        return false;
                }
            }
        }

        return true;
    }
    //print the board with clear formatting
    void printBoard() const
    {
        for (int r = 0; r < 9; r++)
        {
            if (r > 0 && r % 3 == 0)
                cout << "------+-------+------\n";

            for (int c = 0; c < 9; c++)
            {
                if (c > 0 && c % 3 == 0)
                    cout << "| ";

                cout << board[r][c] << " ";
            }

            cout << endl;
        }
    }

    //recursively find and print every solution
    void solveRecursive()
    {
        int bestRow = -1;
        int bestCol = -1;
        int fewestCandidates = 10;

        //find the empty cell with the fewest valid candidates
        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                if (board[r][c] == '_')
                {
                    int candidates = 0;

                    for (char num = '1'; num <= '9'; num++)
                    {
                        if (isValid(r, c, num))
                            candidates++;
                    }

                    //no number can fit in this cell
                    if (candidates == 0)
                        return;

                    if (candidates < fewestCandidates)
                    {
                        fewestCandidates = candidates;
                        bestRow = r;
                        bestCol = c;
                    }
                }
            }
        }

        //no empty cells means a solution was found
        if (bestRow == -1)
        {
            solutionCount++;

            cout << "\nSolution #" << solutionCount << ":\n";
            printBoard();
            return;
        }

        //try every valid number in the selected cell
        for (char num = '1'; num <= '9'; num++)
        {
            if (isValid(bestRow, bestCol, num))
            {
                board[bestRow][bestCol] = num;

                solveRecursive();

                //undo the choice and try another number
                board[bestRow][bestCol] = '_';
            }
        }
    }

public:
    //initialize the board
    SudokuSolver()
    {
        solutionCount = 0;

        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
                board[r][c] = '_';
        }
    }

    //load and validate the puzzle file
    bool loadPuzzle(const string& filename)
    {
        ifstream file(filename);

        if (!file.is_open())
        {
            cout << "Error: Cannot open " << filename << endl;
            return false;
        }

        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                if (!(file >> board[r][c]))
                {
                    cout << "Error: Incomplete puzzle file.\n";
                    return false;
                }

                char ch = board[r][c];

                if ((ch < '1' || ch > '9') && ch != '_')
                {
                    cout << "Error: Invalid character in puzzle.\n";
                    return false;
                }
            }
        }

        //reject extra non-whitespace characters
        char extra;

        if (file >> extra)
        {
            cout << "Error: Puzzle contains extra characters.\n";
            return false;
        }

        return true;
    }

    //solve one puzzle
    void solve(const string& filename)
    {
        cout << "\n====================================\n";
        cout << "Puzzle File: " << filename << endl;
        cout << "====================================\n";

        if (!loadPuzzle(filename))
        {
            cout << "Skipping invalid file.\n";
            return;
        }

        cout << "\nOriginal Puzzle:\n";
        printBoard();

        if (!initialPuzzleValid())
        {
            cout << "\nInvalid puzzle.\n";
            cout << "No solution found.\n";
            return;
        }

        solutionCount = 0;
        solveRecursive();

        if (solutionCount == 0)
            cout << "\nNo solution found.\n";
        else
            cout << "\nTotal solutions found: "
                 << solutionCount << endl;
    }
};

int main()
{
    string puzzleFiles[5] = {
        "puzzle1.txt",
        "puzzle2.txt",
        "puzzle3.txt",
        "puzzle4.txt",
        "puzzle5.txt"
    };

    for (int i = 0; i < 5; i++)
    {
        SudokuSolver solver;
        solver.solve(puzzleFiles[i]);
    }

    return 0;
}
