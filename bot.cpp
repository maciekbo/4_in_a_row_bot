#include <iostream>
#include <random>
#define inf 1'000'000'000
#define p0 '.'
#define p1 'X'
#define p2 'O'

using namespace std;

int random(int a, int b) {
    return rand() % (b - a + 1) + a;
}

class Board {
    string arr[6];
    bool row(int i, int j) {
        if (size - 3 <= j) return false;
        return arr[i][j] != p0 && arr[i][j] == arr[i][j + 1] && arr[i][j + 1] == arr[i][j + 2] && arr[i][j + 2] == arr[i][j + 3];
    }
    bool column(int i, int j) {
        if (3 <= i) return false;
        return arr[i][j] != p0 && arr[i][j] == arr[i + 1][j] && arr[i + 1][j] == arr[i + 2][j] && arr[i + 2][j] == arr[i + 3][j];
    }
    bool right_diagonal(int i, int j) {
        if (size <= j + 3 || 3 <= i) return false;
        return arr[i][j] != p0 && arr[i][j] == arr[i + 1][j + 1] && arr[i + 1][j + 1] == arr[i + 2][j + 2] && arr[i + 2][j + 2] == arr[i + 3][j + 3];
    }
    bool left_diagonal(int i, int j) {
        if (j < 3 || 3 <= i) return false;
        return arr[i][j] != p0 && arr[i][j] == arr[i + 1][j - 1] && arr[i + 1][j - 1] == arr[i + 2][j - 2] && arr[i + 2][j - 2] == arr[i + 3][j - 3];
    }
    public:
    bool my_turn;
    int size;
    Board(int set_size, bool bot_start = true) {
        size = set_size;
        for (int i = 0; i < 6; i++)
         for (int j = 0; j < size; j++)
         arr[i] += p0;
        my_turn = bot_start;
    }
    void print() {
        for (int i = 0; i < 6; i++) {
            for (auto c: arr[i]) printf("%c ", c);
            printf("\n");
        }
    }
    bool can_move(int x) {
        if (x < 0 || size <= x) return false;
        return arr[0][x] == p0;
    }
    void move(int x) {
        if (!can_move(x)) return;
        int height = 0;
        while (arr[++height][x] == p0 && height != 5);
        if (arr[height][x] != p0) height--;
        arr[height][x] = my_turn? p1: p2;
        my_turn = !my_turn;
    }
    void undo_move(int x) {
        int height = -1;
        while (arr[++height][x] == p0 && height < 5);
        arr[height][x] = p0;
        my_turn = !my_turn;
    }
    bool draw() {
        bool b = false;
        for (int i = 0; i < size; i++) b |= can_move(i);
        return !b;
    }
    int win() {
        for (int i = 0; i < 6; i++)
         for (int j = 0; j < 7; j++)
         if (row(i, j) || column(i, j) || left_diagonal(i, j) || right_diagonal(i, j))
         return (arr[i][j] == p1)? 1: -1;
        return 0;
    }
};

namespace bot {
    int max_depth;
    int accuracy;
    int go_deep(Board pos) {
        int w = pos.win();
        while (w == 0) {
            int x = rand() % pos.size;
            while (!pos.can_move(x)) x = (x + 1) % pos.size;
            pos.move(x);
            w = pos.win();
            if (pos.draw()) break;
        }
        return w;
    }
    int go_wide(Board pos, int depth, int father_val) {
        int w = pos.win();
        if (w != 0) return w * accuracy;
        if (pos.draw()) return 0;
        int scr;
        if (depth == max_depth) {
            scr = 0;
            for (int i = 0; i < accuracy; i++) scr += go_deep(pos);
            return scr;
        }
        scr = pos.my_turn? -inf: inf;
        for (int i = 0; i < pos.size; i++) 
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
    int chose_move(Board pos) {
        pair <int, int> move = {0, 0};
        // if (pos.my_turn) move = {}
        for (int i = 0; i < pos.size; i++) 
         if (pos.can_move(i)) {
            Board temp = pos;
            temp.move(i);
            move = max(move, {go_wide(temp, 1, -inf), i});
        }
        return move.second;
    }
}

int main() {
    bot::max_depth = 5;
    bot::accuracy = 10;
    Board my_board(7);
    while (!(my_board.win() != 0 || my_board.draw())) {
        my_board.move(bot::chose_move(my_board));
        system("clear");
        printf("1 2 3 4 5 6 7\n");
        my_board.print();
        if (my_board.win()) break;
        int my_move;
        scanf("%d", &my_move);
        my_board.move(my_move - 1);
        system("clear");
        printf("1 2 3 4 5 6 7\n");
        my_board.print();
    }
}