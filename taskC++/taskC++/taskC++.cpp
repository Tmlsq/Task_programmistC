#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

// Чтение лабиринта из файла
vector<string> readMaze(const string& filename) {
    ifstream file(filename);
    vector<string> maze;
    string line;

    if (!file.is_open()) {
        cout << "Error: can't open the file " << filename << endl;
        return maze;
    }

    while (getline(file, line)) {
        if (!line.empty()) {
            maze.push_back(line);
        }
    }

    file.close();
    return maze;
}

// Поиск координат старта и финиша
bool findPoints(const vector<string>& maze,
    int& start_row, int& start_col, //координаты старта
    int& finish_row, int& finish_col) { //координаты финиша
    int rows = maze.size();
    int cols = maze[0].size();

    start_row = start_col = -1;
    finish_row = finish_col = -1;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (maze[r][c] == 'S') {
                start_row = r;
                start_col = c;
            }
            if (maze[r][c] == 'F') {
                finish_row = r;
                finish_col = c;
            }
        }
    }

    return (start_row != -1 && finish_row != -1);
}

// BFS (поиск пути)
bool bfs(vector<string>& maze,
    int start_row, int start_col,
    int finish_row, int finish_col) {

    int rows = maze.size();
    int cols = maze[0].size();

    //вектор для хранения родителя каждой клетки (-1 означает, что клетка не посещена)
    vector<int> parent(rows * cols, -1);
    queue<int> q;

    //преобразование координат в одномерне индексы
    int start_index = start_row * cols + start_col;
    int finish_index = finish_row * cols + finish_col;

    parent[start_index] = start_index;
    q.push(start_index);

    //изменения строки (по направлениям)
    int delta_row[4] = {1, -1, 0, 0};

    //изменения столбца (по направлениям)
    int delta_cols[4] = {0, 0, 1, -1};

    bool path_found = false;

    while (!q.empty() && !path_found) {
        int current = q.front();
        q.pop();

        //преобразование одномерных индексов в координаты
        int cur_row = current / cols;
        int cur_col = current % cols;

        //цикл, перебирающий 4 возможных направления движения 
        for (int i = 0; i < 4; i++) {
            int new_row = cur_row + delta_row[i];
            int new_col = cur_col + delta_cols[i];

            //проверка граничных условий и то, что клетка не стена ('x')
            if (new_row >= 0 && new_row < rows &&
                new_col >= 0 && new_col < cols &&
                maze[new_row][new_col] != 'x') {

                int new_index = new_row * cols + new_col;

                if (parent[new_index] == -1) {
                    parent[new_index] = current;

                    if (new_index == finish_index) {
                        path_found = true;
                        break;
                    }
                    q.push(new_index);
                }
            }
        }
    }

    // Восстановление пути, начинающееся от финиша к старту
    if (path_found) {
        int current = finish_index;

        while (current != start_index) {
            int r = current / cols;
            int c = current % cols;

            if (maze[r][c] != 'S' && maze[r][c] != 'F') {
                maze[r][c] = '*';
            }
            current = parent[current];
        }
    }

    return path_found;
}

// Вывод лабиринта
void printMaze(const vector<string>& maze) {
    for (const auto& row : maze) {
        cout << row << endl;
    }
}

int main() {
    vector<string> maze = readMaze("input.txt");

    if (maze.empty()) {
        cout << "Error: maze's empty" << endl;
        return 0;
    }

    int start_row, start_col;
    int finish_row, finish_col;

    if (!findPoints(maze, start_row, start_col, finish_row, finish_col)) {
        cout << "Error: no start (S) or finish (F) found" << endl;
        return 0;
    }

    if (!bfs(maze, start_row, start_col, finish_row, finish_col)) {
        cout << "Path not found" << endl;
        return 0;
    }

    printMaze(maze);
    return 0;
}