#include <gtest/gtest.h>
#include "sudoku.h"

using namespace std;

TEST(sudoku_test, test_case) {
   vector<int> rows = {
      1, 2,
      0, 1
   };
   Sudoku s(rows);

   vector<int> row0 = {1,2};
   vector<int> row1 = {0,1};

   ASSERT_EQ(row0, s.getRow(0));
   ASSERT_EQ(row1, s.getRow(1));
};

TEST(sudoku_test, test_col) {
   vector<int> rows = {
      1, 2,
      0, 1
   };
   Sudoku s(rows);

   vector<int> col0 = {1,0};
   vector<int> col1 = {2,1};

   ASSERT_EQ(col0, s.getCol(0));
   ASSERT_EQ(col1, s.getCol(1));
};

TEST(serialize, sudoku_from_str) {
   std::string s = "[ 0 0 0 3 0 7 0 0 0 ]\n"
                   "[ 0 0 1 0 4 0 3 0 0 ]\n"
                   "[ 0 0 0 0 0 0 0 2 0 ]\n"
                   "[ 0 0 0 5 0 1 2 0 0 ]\n"
                   "[ 0 9 0 0 0 0 0 5 0 ]\n"
                   "[ 0 7 0 4 8 2 0 1 0 ]\n"
                   "[ 2 0 0 0 0 6 0 0 4 ]\n"
                   "[ 0 0 5 0 1 0 6 0 0 ]\n"
                   "[ 0 6 0 0 0 0 0 7 0 ]\n";
   Sudoku sudoku(s);
   vector<int> col0 = {0,0,2,0,0,0,0,0,0};
   vector<int> col1 = {6,0,0,7,9,0,0,0,0};
   ASSERT_EQ(col0, sudoku.getCol(0));
   ASSERT_EQ(col1, sudoku.getCol(1));
   ASSERT_EQ(s, sudoku.str());
};

TEST(serialize, move_to_str) {
   std::string s = "1 1 9\n"
                   "2 2 3\n"
                   "2 6 2\n";
   std::vector<Move> list;
   list.push_back(Move(1,1,9));
   list.push_back(Move(2,2,3));
   list.push_back(Move(2,6,2));
   ASSERT_EQ(s, move_list_to_str(list));
};

TEST(serialize, move_from_str) {
   std::string s = "1 1 9\n"
                   "2 2 3\n"
                   "2 6 2\n";
   vector<Move> moves = move_list(s);
   vector<Move> expected;
   expected.push_back(Move(1,1,9));
   expected.push_back(Move(2,2,3));
   expected.push_back(Move(2,6,2));
   ASSERT_EQ(expected, moves);
};

TEST(serialize, both_from_str) {
   vector<int> rows = {
      1, 2,
      0, 1
   };
   Sudoku s(rows);
   vector<Move> list;
   list.push_back(Move(1,1,9));
   list.push_back(Move(2,2,3));
   list.push_back(Move(2,6,2));
   std::string serialized = s.str() + "@" + move_list_to_str(list);

   std::stringstream ss(serialized);
   std::string line;
   getline(ss, line, '@');
   ASSERT_EQ(s.str(), Sudoku(line).str());

   getline(ss, line, '@');
   ASSERT_EQ(list, move_list(line));
};
