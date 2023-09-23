#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

class Sudoku {
private:
	string name;
	int dimentions;
	vector<string> rules;
public:
	Sudoku() {
		name = "Sudoku Game";
		dimentions = 9;
		rules = {
			"You have to fill in the blank squares with numbers 1 - 9",
		 	"You can use a number only once along a collunm, row and 3 x 3 squares",
		 	"You cannot change the values that are fixed"
		};
	}
	friend void display_rules(Sudoku);
};

void display_rules(Sudoku sudoku) {
	system("cls");
	for(int i = 1; i <= sudoku.rules.size(); i++) {
		cout << i << ". " << sudoku.rules[i - 1] << endl;
	}
	cout << endl;
}

class SudokuHard : public Sudoku {
private:
	vector<vector<int>> sud;
	vector<vector<pair<int, int>>> sm_sqr;
	vector<pair<int, int>> hard_pos;
	bool pos_error = false;
	bool limit_error = false;
public:
	SudokuHard() {
		this->sud = {
			{1,0,0,5,9,0,4,0,0},
			{4,0,0,0,0,0,0,0,6},
			{0,0,0,7,0,0,0,1,2},
			{0,0,5,0,7,0,6,0,9},
			{8,0,1,0,5,6,0,0,0},
			{0,2,6,3,0,9,0,0,0},
			{0,0,0,0,0,0,8,6,0},
			{0,0,0,0,0,0,0,4,3},
			{0,5,4,0,8,0,0,0,0}
		};

		this->sm_sqr = {
			{{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}},
			{{0,3},{0,4},{0,5},{1,3},{1,4},{1,5},{2,3},{2,4},{2,5}},
			{{0,6},{0,7},{0,8},{1,6},{1,7},{1,8},{2,6},{2,7},{2,8}},
			{{3,0},{3,1},{3,2},{4,0},{4,1},{4,2},{5,0},{5,1},{5,2}},
			{{3,3},{3,4},{3,5},{4,3},{4,4},{4,5},{5,3},{5,4},{5,5}},
			{{3,6},{3,7},{3,8},{4,6},{4,7},{4,8},{5,6},{5,7},{5,8}},
			{{6,0},{6,1},{6,2},{7,0},{7,1},{7,2},{8,0},{8,1},{8,2}},
			{{6,3},{6,4},{6,5},{7,3},{7,4},{7,5},{8,3},{8,4},{8,5}},
			{{6,6},{6,7},{6,8},{7,6},{7,7},{7,8},{8,6},{8,7},{8,8}}
		};

		make_hard_pos();
	}

	friend void display_rules(Sudoku);

	void show_sudoku(){
		system("cls");
		if (pos_error) {
			cout << "Change in that position cannot be made. Try again" << endl;
			pos_error = false;
		}

		if (limit_error) {
			cout << "input values must be between 1 - 9 and position values must be between 0 - 8" << endl;
			limit_error = false;
		}

		int count1 = 0, count2 = 0;
		for(int i = 0; i < 9; i++){
			if(count2 % 3 == 0) {
				cout << endl;
				for (int a = 1; a <= 22; a++) cout << "_";
				count2 = 0;
			}

			count2++;
			cout << endl;

			for(int j = 0; j < 9; j++) {
				if (count1 % 3 == 0) {
					cout << "|";
					count1 = 0;
				}

				if (sud[i][j] == 0) {
					cout << "  ";
				} else {
					for (auto it : hard_pos) {
						if (it.first == i && it.second == j) {
							SetConsoleTextAttribute(h, 14);
							break;
						}
					}
					if (check_correct(sud[i][j], i, j)) SetConsoleTextAttribute(h, 12);
					cout << sud[i][j] << " ";
					SetConsoleTextAttribute(h, 7);
				}

				count1++;
			}

			cout << "|";
			// cout << endl;
		}
		cout << endl;
		for (int i = 1; i <= 22; i++) {
			cout << "_";
		}

		cout << endl;
	}

	void make_hard_pos() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if(sud[i][j] != 0) hard_pos.push_back(make_pair(i, j));
			}
		}
	}

	void change_value() {
		int x, y, val;
		cout << endl << "Enter position (x, y): ";
		cin >> x >> y;
		cout << "Enter the value: ";
		cin >> val;

		if (!(x >= 0 && x <= 8) && !(y >= 0 && y <= 8) && !(val >= 0 && val <= 0)) {
			limit_error = true;
			return;
		}

		for (auto it1 : hard_pos) {
			if (y == it1.first && x == it1.second) {
				pos_error = true;
				return;
			}
		}

		sud[y][x] = val;
	}

	bool check_correct(int n, int y, int x) {
		int count = 0;
		bool flag = false;

		for (int i = 0; i < 9; i++) {
			if (sud[i][x] == n && i != y) return true;
		}

		for (int i = 0; i < 9; i++) {
			if (sud[y][i] == n && i != x) return true;
		}

		for (auto it1 : sm_sqr) {
			for (auto it2 : it1) {
				if (it2.first == y && it2.second == x) {
					flag = true;
					break;
				}
			}
			if (flag) break;
			count++;
		}

		for (auto it : sm_sqr[count]) {
			if (sud[it.first][it.second] == sud[y][x] && it.first != y && it.second != x)
				return true;
		}

		return false;
	}

	bool check_complete() {
		int sum = 0;
		for (auto it1 : sud) {
			for (auto it2 : it1) {
				sum += it2;
			}
			if (sum != 45) return false;
			sum = 0;
		}

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				sum += sud[j][i];
			}
			if (sum != 45) return false;
			sum = 0;
		}

		for (auto it1 : sm_sqr) {
			for (auto it2 : it1) {
				sum += sud[it2.first][it2.second];
			}
			if (sum != 45) return false;
			sum = 0;
		}
		return true;
	}

	void reset() {
		this->sud = {
			{1,0,0,5,9,0,4,0,0},
			{4,0,0,0,0,0,0,0,6},
			{0,0,0,7,0,0,0,1,2},
			{0,0,5,0,7,0,6,0,9},
			{8,0,1,0,5,6,0,0,0},
			{0,2,6,3,0,9,0,0,0},
			{0,0,0,0,0,0,8,6,0},
			{0,0,0,0,0,0,0,4,3},
			{0,5,4,0,8,0,0,0,0}
		};
	}
};

int main() {
	SudokuHard sudoku;
	char ch;

	display_rules(sudoku);

	cout << "Do you want to start the game?(y/n): ";
	cin >> ch;
	
	while(ch == 'y') {
		while(!sudoku.check_complete()) {
			sudoku.show_sudoku();
			sudoku.change_value();
		}

		sudoku.show_sudoku();

		cout << endl << "Congratulation you have successfully solved the sudoku puzzle." << endl;

		cout << "Do you want to play again?(y/n): ";
		cin >> ch;
		sudoku.reset();
	}

	return 0;
}

// 1: Blue
// 2: Green
// 3: Cyan
// 4: Red
// 5: Purple
// 6: Yellow (Dark)
// 7: Default White
// 8: Gray/Grey
// 9: Bright Blue
// 10: Bright Green
// 11: Bright Cyan
// 12: Bright Red
// 13: Pink/Magenta
// 14: Yellow 
// 15: Bright White
// ...... After 15 are background color