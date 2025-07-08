#include <array>
#include <iostream>
#include <fstream>
#include <optional>

using namespace std;

using Board = array<array<int, 9>, 9>;
using Mask  = array<int, 9>;

Board load_board() {
    return {{
        {{5, 3, 0, 0, 7, 0, 0, 0, 0}},
        {{6, 0, 0, 1, 9, 5, 0, 0, 0}},
        {{0, 9, 8, 0, 0, 0, 0, 6, 0}},
        {{8, 0, 0, 0, 6, 0, 0, 0, 3}},
        {{4, 0, 0, 8, 0, 3, 0, 0, 1}},
        {{7, 0, 0, 0, 2, 0, 0, 0, 6}},
        {{0, 6, 0, 0, 0, 0, 2, 8, 0}},
        {{0, 0, 0, 4, 1, 9, 0, 0, 5}},
        {{0, 0, 0, 0, 8, 0, 0, 7, 9}}
    }};
}

void print_board(const Board& b) {
    for (int r = 0; r < 9; ++r) {
        if (r && r % 3 == 0) cout << "---------------------\n";
        for (int c = 0; c < 9; ++c) {
            if (c && c % 3 == 0) cout << " |";
            cout << ' ' << (b[r][c] ? char('0' + b[r][c]) : '.');
        }
        cout << '\n';
    }
}

optional<pair<int, int>> find_empty(const Board& b) {
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (b[r][c] == 0)
                return make_pair(r, c);
    return nullopt;
}

bool solve(Board& b, Mask& rows, Mask& cols, Mask& boxes) {
    auto empty = find_empty(b);
    if (!empty) return true;

    int r = empty->first, c = empty->second;
    int box = (r / 3) * 3 + (c / 3);
    int illegal = rows[r] | cols[c] | boxes[box];

    for (int num = 1; num <= 9; ++num) {
        int bit = 1 << num;
        if (illegal & bit) continue;

        b[r][c] = num;
        rows[r] |= bit;
        cols[c] |= bit;
        boxes[box] |= bit;

        if (solve(b, rows, cols, boxes)) return true;

        b[r][c] = 0;
        rows[r] ^= bit;
        cols[c] ^= bit;
        boxes[box] ^= bit;
    }
    return false;
}

void prepare_masks(const Board& b, Mask& rows, Mask& cols, Mask& boxes) {
    rows.fill(0);
    cols.fill(0);
    boxes.fill(0);
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c) {
            int n = b[r][c];
            if (!n) continue;
            int bit = 1 << n;
            rows[r] |= bit;
            cols[c] |= bit;
            boxes[(r / 3) * 3 + (c / 3)] |= bit;
        }
}

int main() {
    Board board;
    ifstream fin("input.txt");
    if (!fin) {
        cout << "❌ Cannot open input.txt\n";
        return 1;
    }

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            fin >> board[r][c];
    fin.close();

    cout << "Initial puzzle:\n";
    print_board(board);

    Mask rows, cols, boxes;
    prepare_masks(board, rows, cols, boxes);

    cout << "\nSolving...\n\n";
    if (solve(board, rows, cols, boxes)) {
        cout << "Solution:\n";
        print_board(board);

        ofstream fout("solved.txt");
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c)
                fout << board[r][c] << " ";
            fout << "\n";
        }
        fout.close();
        cout << "\n Solution written to solved.txt\n";
    } else {
        cout << "No solution exists.\n";
    }
    return 0;
}

