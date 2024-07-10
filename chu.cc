#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

// ゲームボードを表示する
void displayBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << endl;
        if (i < 2) cout << "---------" << endl;
    }
    cout << endl;
}

// プレーヤーが勝利したかどうかを確認する
bool checkWin(const vector<vector<char>>& board, char player) {
    // 行と列を確認する
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    // 対角線を確認する
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
    return false;
}

// 勝利の可能性があるかどうかを確認する
bool checkPotentialWin(vector<vector<char>>& board, char player, int& row, int& col) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                if (checkWin(board, player)) {
                    row = i;
                    col = j;
                    board[i][j] = ' ';
                    return true;
                }
                board[i][j] = ' ';
            }
        }
    }
    return false;
}

// コンピュータの動き
void computerMove(vector<vector<char>>& board, int& row, int& col) {
    // コンピュータが勝てるかどうかを確認する
    if (checkPotentialWin(board, 'O', row, col)) {
        board[row][col] = 'O';
        return;
    }
    
    // プレーヤーをブロックする必要があるかどうかを確認する
    if (checkPotentialWin(board, 'X', row, col)) {
        board[row][col] = 'O';
        return;
    }
    
    // ランダムに空いている場所を選ぶ
    vector<pair<int, int>> emptySpots;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                emptySpots.push_back({i, j});
            }
        }
    }
    
    if (!emptySpots.empty()) {
        int randomIndex = rand() % emptySpots.size();
        row = emptySpots[randomIndex].first;
        col = emptySpots[randomIndex].second;
        board[row][col] = 'O';
    }
}

int main() {
    srand(time(0));  // ランダム数生成器を初期化
    vector<vector<char>> board(3, vector<char>(3, ' '));
    char currentPlayer = 'X';
    list<vector<int>> Moved;

    while (true) {
        displayBoard(board);
        
        int row, col;
        if (currentPlayer == 'X') {
            bool validMove = false;
            while (!validMove) {
                cout << "プレーヤーのターン。行と列 (1-3)の番号を入力してください: ";
                cin >> row >> col;
                row -= 1;
                col -= 1;

                if (row < 0 || row > 2 || col < 0 || col > 2) {
                    cout << "入力エラー、1から3の数を二つ入力してください。" << endl;
                } else if (board[row][col] != ' ') {
                    cout << "この位置はもう記号があります。他の位置にしてください。" << endl;
                } else {
                    validMove = true;
                }
            }

            board[row][col] = currentPlayer;
        } else {
            cout << "コンピュータのターン..." << endl;
            computerMove(board, row, col);
        }
        if (checkWin(board, currentPlayer)) {
            displayBoard(board);
            if (currentPlayer == 'X') cout << "プレーヤーの勝ち！" << endl;
            else cout << "コンピュータの勝ち！" << endl;
            return 0;
        }

        Moved.push_back({row, col});

        if (Moved.size() > 6) {
            vector<int> oldestMove = Moved.front();
            Moved.pop_front();
            board[oldestMove[0]][oldestMove[1]] = ' ';
        }

        if (Moved.size() == 9) {
            displayBoard(board);
            cout << "ゲームは引き分けです！" << endl;
            return 0;
        }

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }   
}
