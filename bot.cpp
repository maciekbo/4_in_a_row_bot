#include <iostream>
#include <random>
#include <chrono>
#include <stack>
#define inf 1'000'000'000
#define p0 '.'
#define p1 'X'
#define p2 'O'

using namespace std;

int random(int a, int b) {
    return rand() % (b - a + 1) + a;
}

class Board {
    public:
    static const int width = 7, height = 6;

    private:
    string state[height];
    int current_height[width];
    stack <int> moves;

    bool valid_move (int i, int j) {
        return 1 <= i && i <= height && 1 <= j && j <= width;
    };
    bool row(int i, int j) {
        if (width - 3 <= j || !valid_move(i, j)) return false;
        if (state[i][j] == p0) return false;
        for (int d: {0, 1, 2})
         if (state[i][j + d] != state[i][j + d + 1])
         return false;
        return true;
    }
    bool column(int i, int j) {
        if (height - 3 <= i || !valid_move(i, j)) return false;
        if (state[i][j] == p0) return false;
        for (int d: {0, 1, 2})
         if (state[i + d][j] != state[i + d + 1][j])
         return false;
        return true;
    }
    bool right_diagonal(int i, int j) {
        if (width - 3 <= j || 3 <= i || !valid_move(i, j)) return false;
        if (state[i][j] == p0) return false;
        for (int d: {0, 1, 2})
         if (state[i + d][j + d] != state[i + d + 1][j + d + 1])
         return false;
        return true;
    }
    bool left_diagonal(int i, int j) {
        if (j < 3 || 3 <= i || !valid_move(i, j)) return false;
        if (state[i][j] == p0) return false;
        for (int d: {0, 1, 2})
         if (state[i + d][j - d] != state[i + d + 1][j - d - 1])
         return false;
        return true;
    }

    public:
    bool my_turn;

    Board(bool bot_start = true) {
        for (int i = 0; i < height; i++)
         for (int j = 0; j < width; j++)
         state[i] += p0;
        for (int i = 0; i < width; i++) current_height[i] = height;
        my_turn = bot_start;
    }

    void print() {
        for (int i = 0; i < height; i++) {
            cout << state[i] << "\n";
        }
    }

    bool can_move(int x) {
        if (x < 0 || width <= x) return false;
        return current_height[x] > 0;
    }

    void move(int x) {
        if (!can_move(x)) return;
        moves.push(x);
        state[current_height[x] - 1][x] = my_turn? p1: p2;
        current_height[x]--;
        my_turn = !my_turn;
    }

    void undo_move() {
        if (moves.empty()) return;
        int last = moves.top();
        moves.pop();
        state[current_height[last]][last] = p0;
        my_turn = !my_turn;
    }

    bool draw() {
        bool b = true;
        for (int i = 0; i < width; i++) b &= !can_move(i);
        return b;
    }

    int win() {
        if (moves.empty()) return 0;
        int i = current_height[moves.top()], j = moves.top();
        for (int d: {0, 1, 2, 3}) {
            if (row(i, j - d) || column(i - d, j) || left_diagonal(i - d, j - d) || right_diagonal(i - d, j + d))
             return (state[i][j] == p1)? 1: -1;
        }
        return 0;
    }
};

namespace bot {
    int max_depth;
    int accuracy;

    int go_deep(Board pos) {
        int w = pos.win();
        int iter = 0;
        while (w == 0) {
            iter++;
            if (iter > 50) return 0;
            int x = rand() % pos.width;
            int iter2 = 0;
            while (!pos.can_move(x)) {
                iter++;
                if (iter > 10) return 0;
                x = (x + 1) % pos.width;
            }
            pos.move(x);
            w = pos.win();
            if (pos.draw()) break;
        }
        return w;
    }

    int go_wide(Board pos, int depth, int father_val) {
        int w = pos.win();
        if (w != 0) return w * accuracy * 10;
        if (pos.draw()) return 0;
        int scr;
        if (depth == max_depth) {
            scr = 0;
            for (int i = 0; i < accuracy; i++) scr += go_deep(pos);
            return scr;
        }
        scr = pos.my_turn? -inf: inf;
        for (int i = 0; i < pos.width; i++) 
         if (pos.can_move(i)) {
            Board temp = pos;
            temp.move(i);
            if (pos.my_turn) scr = max(scr, go_wide(temp, depth + 1, scr));
            else             scr = min(scr, go_wide(temp, depth + 1, scr));
            if (pos.my_turn) {
                if (scr > father_val) return scr;
            } else {
                if (scr < father_val) return scr;
            }
        }
        return scr;
    }

    pair <int, int> chose_move(Board pos) {
        pair <int, int> move = {0, 0};
        // if (pos.my_turn) move = {}
        for (int i = 0; i < pos.width; i++) 
         if (pos.can_move(i)) {
            Board temp = pos;
            temp.move(i);
            move = max(move, {go_wide(temp, 1, -inf), i});
        }
        return move;
    }
}

int main() {
    bot::max_depth = 5;
    bot::accuracy = 15;
    srand(chrono::high_resolution_clock::now().time_since_epoch().count());
    Board my_board;
    system("clear");

    cout << "Do you want to go first? Y/N\n";
    char who_first;
    cin >> who_first;
    if (who_first == 'Y' || who_first == 'y') {
        my_board.my_turn = false;
        system("clear");
        cout << "1 2 3 4 5 6 7\n";
        my_board.print();
    }

    while (!(my_board.win() != 0 || my_board.draw())) {
        if (my_board.my_turn) {
            auto bot_move = bot::chose_move(my_board);
            my_board.move(bot_move.second);
            system("clear");
            cout << "1 2 3 4 5 6 7\n";
            my_board.print();
            cout << "The bot moved " << bot_move.second + 1 << "\n";
            if (bot_move.first > 2 * bot::accuracy) cout << "The bot is certain it will win\n";
            else if (bot_move.first < -2 * bot::accuracy) cout << "The bot is is certain you can win\n";
            else cout << "The bot is " << (bot_move.first + bot::accuracy + 1) * 50 / (bot::accuracy + 1) << " certain it can win.\n";
        }
        if (my_board.win() || my_board.draw()) break;

        string input;
        int my_move;
        cout << "Your move is: ";
        while (true) {
            getline(cin, input);
            my_move = atoi(input.c_str());
            if (1 <= my_move && my_move <= 7) break;
            cout << "Invalid move. Type a number between 1 and 7, and then press enter.\n";
        }
        my_board.move(my_move - 1);
        system("clear");
        cout << "1 2 3 4 5 6 7\n";
        my_board.print();
        if (my_board.win() || my_board.draw()) break;
    }
}