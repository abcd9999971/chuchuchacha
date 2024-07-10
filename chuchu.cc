#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>

const int CELL_SIZE = 100;
const int BOARD_SIZE = 3;

class TicTacToe {
private:
    struct Move {
        int row, col;
        char player;
        int turn;
    };

    std::vector<std::vector<char>> board;
    std::queue<Move> moves;
    char currentPlayer;
    bool gameOver;
    int currentTurn;

    sf::RenderWindow window;
    sf::Font font;

public:
    TicTacToe() : board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' ')), 
                  currentPlayer('X'), gameOver(false), currentTurn(0) {
        window.create(sf::VideoMode(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE), "Tic-Tac-Toe");
        font.loadFromFile("path/to/your/font.ttf");  // 你需要提供一個字體文件
        srand(time(nullptr)); // 初始化隨機數生成器
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            if (currentPlayer == 'O' && !gameOver) {
                computerMove();
            }
            draw();
        }
    }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && !gameOver && currentPlayer == 'X') {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }

    void handleMouseClick(int x, int y) {
        int row = y / CELL_SIZE;
        int col = x / CELL_SIZE;
        if (row < BOARD_SIZE && col < BOARD_SIZE && board[row][col] == ' ') {
            makeMove(row, col);
        }
    }

    void makeMove(int row, int col) {
        board[row][col] = currentPlayer;
        moves.push({row, col, currentPlayer, currentTurn});
        currentTurn++;

        if (moves.size() > 12) { // 如果已經進行了超過12步(3x4),移除最早的一步
            Move oldestMove = moves.front();
            moves.pop();
            board[oldestMove.row][oldestMove.col] = ' ';
        }

        if (checkWin(currentPlayer)) {
            std::cout << "Player " << currentPlayer << " wins!" << std::endl;
            gameOver = true;
        } else if (isBoardFull()) {
            std::cout << "It's a draw!" << std::endl;
            gameOver = true;
        } else {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    void computerMove() {
        std::vector<std::pair<int, int>> emptyCells;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == ' ') {
                    emptyCells.push_back({i, j});
                }
            }
        }
        if (!emptyCells.empty()) {
            int randomIndex = rand() % emptyCells.size();
            int row = emptyCells[randomIndex].first;
            int col = emptyCells[randomIndex].second;
            makeMove(row, col);
        }
    }

    void draw() {
        window.clear(sf::Color::White);

        // Draw grid lines
        for (int i = 1; i < BOARD_SIZE; ++i) {
            sf::RectangleShape line(sf::Vector2f(BOARD_SIZE * CELL_SIZE, 2));
            line.setFillColor(sf::Color::Black);
            line.setPosition(0, i * CELL_SIZE);
            window.draw(line);

            line.setSize(sf::Vector2f(2, BOARD_SIZE * CELL_SIZE));
            line.setPosition(i * CELL_SIZE, 0);
            window.draw(line);
        }

        // Draw X's and O's
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] != ' ') {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(board[i][j]);
                    text.setCharacterSize(80);
                    text.setFillColor(sf::Color::Black);
                    text.setPosition(j * CELL_SIZE + 25, i * CELL_SIZE);
                    window.draw(text);
                }
            }
        }

        window.display();
    }

    bool checkWin(char player) {
        // 檢查行和列
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
            if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
        }
        // 檢查對角線
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
        return false;
    }

    bool isBoardFull() {
        for (const auto& row : board) {
            for (char cell : row) {
                if (cell == ' ') return false;
            }
        }
        return true;
    }
};

int main() {
    TicTacToe game;
    game.run();
    return 0;
}