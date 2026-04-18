#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <stack>

using namespace std;

template<class A, class B>
ostream& operator<<(ostream& o, const pair<A, B>& p) {
    return o << '(' << p.first << ", " << p.second << ')';
}
template<class A, class B, class C>
ostream& operator<<(ostream& o, const tuple<A, B, C>& t) {
    return o << '(' << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ')';
}
template<class T>
auto operator<<(ostream& o, const T& x) -> decltype(x.end(), o) {
    o << '{';
    int i = 0;
    for (const auto& e : x) {
        o << (", ") + 2 * !i++ << e;
    }
    return o << '}';
}
auto operator<<(ostream& o, const string& s) -> decltype(o) {
    for (const auto& e : s) {
        o << e;
    }
    return o;
}
#define red "\x1b[38;2;255;75;75m"
#define blue "\x1b[38;2;75;75;255m"
#define green "\x1b[38;2;75;255;75m"
#define gray "\x1b[38;2;140;140;140m"
#define cyan "\x1b[38;2;75;255;255m"
#define yellow "\x1b[38;2;255;255;75m"
#define violet "\x1b[38;2;190;100;255m"
#define orange "\x1b[38;2;255;180;50m"
#define reset "\x1b[0m"
#define bold "\x1b[1m"
#define underline "\x1b[4m"
#define italic "\x1b[3m"
#define strikethrough "\x1b[9m"
#define invert "\x1b[7m"
//#define DEBUG
#ifdef DEBUG
#define fastio()
#define deb(x...) cerr << "[" #x "]: ", [](auto... $) {((cerr << $ << "; "), ...); }(x), cerr << reset << "\n"
#define debug(c, x...) cerr << "[" #x "]: " << c, [](auto... $) {((cerr << $ << "; "), ...); }(x), cerr << reset << "\n"
#else
#define fastio() ios_base::sync_with_stdio(0); cin.tie(0);
#define deb(...)
#define debug(...)
#endif
#ifdef LOCAL
#define asrt(x) if (!(x)) { cerr << red << bold << "Assertion failed: " << reset << italic << #x << reset << " in line " << __LINE__ << endl; exit(1); }
#else
#define asrt(x)
#endif
#define pii pair<int,int>
#define inf 1'000'000'000
#define llinf 2'000'000'000'000'000'000
#define mod 998'244'353
#define mod7 1'000'000'007
#define mod9 1'000'000'009
#define eps 0.00000000001
#define all(x) x.begin(),x.end()
#define rev(x) x.rbegin(),x.rend()
#define decrease(x) x,vector<x>,greater<x>
#define bits(x) ((x==0)?0:32-__builtin_clz(x))
#define arrsize(x) (sizeof(x)/sizeof(x[0]))
#define p0 '.'
#define p1 'X'
#define p2 'O'
#define int long long
typedef long long ll;
typedef long double ld;
int popcount(int x) {
    int scr = 0;
    while (x > 0) {
        scr += x & 1;
        x >>= 1;
    }
    return scr;
}

int random(int a, int b) {
    return rand() % (b - a + 1) + a;
}

class Board {
    public:
    static const int width = 7, height = 6;

    // private:
    string state[height];
    int current_height[width];
    stack <int> moves;

    bool valid_move (int i, int j) {
        return 0 <= i && i < height && 0 <= j && j < width;
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
            for (char c: state[i]) {
                if (c == p1) cout << red;
                if (c == p2) cout << green;
                cout << c << " " << reset;
            }
            cout << "\n";
        }
    }

    bool can_move(int x) {
        if (x < 0 || width <= x) return false;
        return state[0][x] == p0;
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
        current_height[last]++;
        my_turn = !my_turn;
    }

    bool draw() {
        bool b = true;
        for (int i = 0; i < width; i++) b &= !can_move(i);
        return b;
    }

    int win() {
        if (moves.size() < 7) return 0;
        for (int i = 0; i < height; i++) 
         for (int j = 0; j < width; j++)
         if (row(i, j) || column(i, j) || left_diagonal(i, j) || right_diagonal(i, j))
         return (state[i][j] == p1)? 1: -1;
        return 0;
    }
};

namespace bot {
    int max_depth;
    int accuracy;

    vector <int> move_priority;
    void create_move_priority(int width) {
        move_priority.resize(width);
        iota(all(move_priority), 0);
        sort(all(move_priority), [&](int a, int b) {return abs(a - width / 2) < abs(b - width / 2);});
        debug(blue, move_priority);
    }

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

    int go_wide(Board& pos, int depth, int father_val) {
        int w = pos.win();
        if (w != 0) return w * inf;
        if (pos.draw()) return 0;
        int scr;
        if (depth == max_depth) {
            scr = 0;
            for (int i = 0; i < accuracy; i++) scr += go_deep(pos);
            return scr;
        }
        scr = pos.my_turn? -inf: inf;
        for (int i: move_priority) 
         if (pos.can_move(i)) {
            Board temp = pos;
            temp.move(i);
            if (pos.my_turn) scr = max(scr, go_wide(temp, depth + 1, scr));
            else             scr = min(scr, go_wide(temp, depth + 1, scr));
            temp.undo_move();
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

signed main() {
    bot::max_depth = 5;
    bot::accuracy = 15;
    // srand(chrono::high_resolution_clock::now().time_since_epoch().count());
    Board my_board;
    system("clear");
    bot::create_move_priority(my_board.width);

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
            debug(orange, my_board.can_move(bot_move.second));
            debug(orange, my_board.current_height[bot_move.second]);
            my_board.move(bot_move.second);
            debug(orange, my_board.current_height[bot_move.second]);
            // system("clear");
            cout << "1 2 3 4 5 6 7\n";
            my_board.print();
            cout << "The bot moved " << bot_move.second + 1 << "\n";
            // if (bot_move.first > 2 * bot::accuracy) cout << "The bot is certain it will win\n";
            // else if (bot_move.first < -2 * bot::accuracy) cout << "The bot is is certain you can win\n";
            // else cout << "The bot is " << (bot_move.first + bot::accuracy + 1) * 50 / (bot::accuracy + 1) << "% certain it can win.\n";
        }
        if (my_board.win() || my_board.draw()) break;
        debug(green, my_board.win());

        string input;
        int my_move;
        cout << "Your move is: ";
        while (true) {
            getline(cin, input);
            my_move = atoi(input.c_str());
            if (1 <= my_move && my_move <= 7)
             if (my_board.can_move(my_move - 1))
             break;
            cout << "Invalid move. Type a number between 1 and 7, and then press enter.\n";
        }
        debug(orange, my_board.can_move(my_move - 1));
        my_board.move(my_move - 1);
        system("clear");
        cout << "1 2 3 4 5 6 7\n";
        my_board.print();
        if (my_board.win() || my_board.draw()) break;
        debug(yellow, my_board.win());
    }

    return 0;
}