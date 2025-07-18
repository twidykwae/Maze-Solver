#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
using namespace std;

const string BLUE_BG = "\033[44m";     
const string YELLOW_BG = "\033[43m";   
const string GREEN_BG = "\033[42m";    
const string RESET = "\033[0m";       

void loadMaze(char maze[10][10], const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Can't open file \"" << filename << "\"!" << endl;
        exit(1);
    }
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            fin >> maze[i][j];
        }
    }
    fin.close();
}

void displayMaze(char maze[10][10]) {
    cout << "Maze:" << endl;
    
    cout << " ";
    for (int j = 0; j < 10; j++) {
        cout << j;
    }
    cout << endl;
    
    for (int i = 0; i < 10; i++) {
        cout << i;
        for (int j = 0; j < 10; j++) {
            cout << maze[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void displayColoredMaze(char maze[10][10], char visited[10][10]) {
    cout << "\033[H\033[J";
    cout << "Finding path:" << endl;
    
    cout << " ";
    for (int j = 0; j < 10; j++) {
        cout << j;
    }
    cout << endl;
    
    for (int i = 0; i < 10; i++) {
        cout << i; 
        for (int j = 0; j < 10; j++) {
            if (visited[i][j] == 'G') {
                cout << GREEN_BG << maze[i][j] << RESET;
            } else if (visited[i][j] == 'Y') {
                cout << YELLOW_BG << maze[i][j] << RESET;
            } else if (visited[i][j] == 'B') {
                cout << BLUE_BG << maze[i][j] << RESET;
            } else {
                cout << maze[i][j];
            }
        }
        cout << endl;
    }
    
    usleep(500000);
}

bool canEscape(int row, int col, char maze[10][10], char visited[10][10], bool &foundExit) {
    // Checking if out of bounds or hitting a wall or obstacle
    if (row < 0 || row >= 10 || col < 0 || col >= 10 || 
        maze[row][col] == '*' || maze[row][col] == '#') {
        return false;
    }
    
    // Checkinf if already visited
    if (visited[row][col] != ' ') {
        return false;
    }
    
    // Checking if at the exit
    if (maze[row][col] == 'E') {
        visited[row][col] = 'G'; 
        displayColoredMaze(maze, visited);
        foundExit = true;
        return true;
    }
    
    // Mark as visited
    visited[row][col] = 'B';
    displayColoredMaze(maze, visited);
    
    bool canExit = false;
    
    //down
    if (canEscape(row + 1, col, maze, visited, foundExit)) {
        canExit = true;
    }
    //right
    else if (canEscape(row, col + 1, maze, visited, foundExit)) {
        canExit = true;
    }
    //up
    else if (canEscape(row - 1, col, maze, visited, foundExit)) {
        canExit = true;
    }
    //left
    else if (canEscape(row, col - 1, maze, visited, foundExit)) {
        canExit = true;
    }
    
    if (canExit && foundExit) {
        visited[row][col] = 'G';
        displayColoredMaze(maze, visited);
        return true;
    } else {
        visited[row][col] = 'Y';
        displayColoredMaze(maze, visited);
        return false;
    }
}

int main() {
    char maze[10][10];
    char visited[10][10]; 
    int row, col;
    bool foundExit = false;
    string filename;
    
    // Ask user for maze filename
    cout << "Enter maze filename: ";
    cin >> filename;
    
    loadMaze(maze, filename);
    displayMaze(maze);
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            visited[i][j] = ' ';
        }
    }
    
    do {
        cout << "Enter starting row (0-9): ";
        cin >> row;
        cout << "Enter starting column (0-9): ";
        cin >> col;
        
        if (row < 0 || row >= 10 || col < 0 || col >= 10 || 
            maze[row][col] == '*' || maze[row][col] == '#') {
            cout << "Invalid starting position. Please try again." << endl;
        }
    } while (row < 0 || row >= 10 || col < 0 || col >= 10 || 
             maze[row][col] == '*' || maze[row][col] == '#');
    
    cout << "Finding escape path..." << endl;
    usleep(1000000); 
    
    if (canEscape(row, col, maze, visited, foundExit)) {
        cout << "\nPath to exit found!" << endl;
    } else {
        cout << "\nNo path to exit available from this starting point." << endl;
    }
    
    return 0;
}
