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

TEST(sudoku_test, test_sub) {
   vector<int> rows = {
      1, 2,
      0, 1
   };
   Sudoku s(rows);

   vector<int> sub0 = {1};
   vector<int> sub1 = {2};
   vector<int> sub2 = {0};
   vector<int> sub3 = {1};

   ASSERT_EQ(sub0, s.getSub(0,0));
   ASSERT_EQ(sub1, s.getSub(1,0));
   ASSERT_EQ(sub2, s.getSub(0,1));
   ASSERT_EQ(sub3, s.getSub(1,1));
};

TEST(sudoku_test, test_poss) {
   vector<int> rows = {
      1, 2,
      0, 1
   };
   Sudoku s(rows);

   vector<int> poss0 = {};
   vector<int> poss1 = {};
   vector<int> poss2 = {2};
   vector<int> poss3 = {};

   ASSERT_EQ(poss0, s.getPossible(0,0));
   ASSERT_EQ(poss1, s.getPossible(1,0));
   ASSERT_EQ(poss2, s.getPossible(0,1));
   ASSERT_EQ(poss3, s.getPossible(1,1));
};

TEST(sudoku_test, test_getMoves) {
   vector<int> rows = {
      1, 2,
      0, 1
   };
   Sudoku s(rows);

   vector<Move> moves;
   moves.push_back(Move(2, 0, 1));

   ASSERT_EQ(moves, s.getPossiveMoves());
};

TEST(sudoku_test, test_solved) {
   vector<int> rows1 = {
      1, 2,
      2, 1
   };
   Sudoku s1(rows1);

   vector<int> rows2 = {
      1, 2,
      0, 1,
   };
   Sudoku s2(rows2);

   ASSERT_TRUE(s1.solved());
   ASSERT_FALSE(s2.solved());
};

TEST(sudoku_test, test_solve) {
   vector<Move> steps;
   unsigned limit = 0;
   vector<int> rows = {
      0, 0,
      0, 0
   };
   Sudoku s(rows);

   ASSERT_NO_THROW(solve(s, steps, limit));

   Sudoku s_solved = solve(s, steps, limit);

   vector<int> row0 = {1,2};
   vector<int> row1 = {2,1};

   ASSERT_EQ(row0, s_solved.getRow(0));
   ASSERT_EQ(row1, s_solved.getRow(1));
};

TEST(sudoku_test, test_solve2) {
   vector<Move> steps;
   unsigned limit = 0;
   vector<int> rows = {
      1, 2,  3, 0,
      4, 0,  0, 2,

      0, 0,  0, 0,
      0, 0,  0, 0,
   };
   Sudoku s(rows);

   ASSERT_NO_THROW(solve(s, steps, limit));
   Sudoku s_solved = solve(s, steps, limit);

   vector<int> row0 = {1,2,3,4};
   vector<int> row1 = {4,3,1,2};
   ASSERT_EQ(row0, s_solved.getRow(0));
   ASSERT_EQ(row1, s_solved.getRow(1));
   std::cout << s_solved.str() << std::endl;
};

TEST(sudoku_test, solve_9x9) {
   vector<Move> steps;
   unsigned limit = 0;
   vector<int> rows = {
      0, 0, 0, 0, 0, 4, 6, 0, 0,
      0, 0, 0, 7, 0, 0, 5, 0, 0,
      0, 0, 2, 0, 0, 0, 0, 7, 8,
      0, 0, 0, 3, 5, 0, 0, 0, 4,
      2, 0, 0, 0, 9, 6, 0, 0, 0,
      0, 1, 3, 0, 0, 2, 0, 6, 0,
      9, 0, 0, 4, 0, 0, 1, 0, 0,
      1, 0, 0, 9, 0, 0, 0, 0, 0,
      6, 5, 4, 0, 3, 0, 0, 0, 0,
   };
   Sudoku s(rows);
   Sudoku s_solved = solve(s, steps, limit);
   std::cout << s_solved.str() << std::endl;
};

TEST(sudoku_test, no_solution_9x9) {
   vector<Move> steps;
   unsigned limit = 0;
   vector<int> rows = {
      1, 0, 0, 0, 0, 4, 6, 0, 0,
      0, 0, 0, 7, 0, 0, 5, 0, 0,
      0, 0, 2, 0, 0, 0, 0, 7, 8,
      0, 0, 0, 3, 5, 0, 0, 0, 4,
      2, 0, 0, 0, 9, 6, 0, 0, 0,
      0, 1, 3, 0, 0, 2, 0, 6, 0,
      9, 0, 0, 4, 0, 0, 1, 0, 0,
      1, 0, 0, 9, 0, 0, 0, 0, 0,
      6, 5, 4, 0, 3, 0, 0, 0, 0,
   };
   Sudoku s(rows);
   ASSERT_THROW(solve(s, steps, limit), NoSolution);
};

TEST(sudoku_test, empty_9x9) {
   vector<Move> steps;
   unsigned limit = 0;
   vector<int> rows = {
      1, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0,
   };
   Sudoku s(rows);
   Sudoku s_solved = solve(s, steps, limit);
   std::cout << s_solved.str() << std::endl;
};


TEST(sudoku_test, test_valid) {

   vector<int> rows1 = {
      2, 0, 0, 0,
      0, 0, 2, 3,
      0, 1, 1, 0,
      0, 0, 0, 4,
   };
   vector<int> rows2 = {
      0, 0, 1, 0,
      1, 0, 2, 0,
      0, 0, 4, 0,
      0, 0, 3, 4,
   };
   vector<int> rows3 = {
      0, 3, 0, 1,
      0, 2, 0, 0,
      2, 0, 3, 1,
      0, 0, 0, 4,
   };
   vector<int> rows4 = {
      1, 4, 2, 0,
      3, 2, 1, 0,
      0, 0, 3, 1,
      0, 0, 4, 0,
   };
   Sudoku s1(rows1);
   Sudoku s2(rows2);
   Sudoku s3(rows3);
   Sudoku s4(rows4);

   ASSERT_FALSE(s1.valid());
   ASSERT_FALSE(s2.valid());
   ASSERT_FALSE(s3.valid());
   ASSERT_TRUE(s4.valid());


   vector<Move> steps;
   unsigned limit = 0;
   vector<int> rows = {
      0, 0, 0, 0, 0, 4, 6, 0, 0,
      0, 0, 0, 7, 0, 0, 5, 0, 0,
      0, 0, 2, 0, 0, 0, 0, 7, 8,
      0, 0, 0, 3, 5, 0, 0, 0, 4,
      2, 0, 0, 0, 9, 6, 0, 0, 0,
      0, 1, 3, 0, 0, 2, 0, 6, 0,
      9, 0, 0, 4, 0, 0, 1, 0, 0,
      1, 0, 0, 9, 0, 0, 0, 0, 0,
      6, 5, 4, 0, 3, 0, 0, 0, 0,
   };
   Sudoku s(rows);
   Sudoku s_solved = solve(s, steps, limit);
   ASSERT_TRUE(s.valid());
   ASSERT_TRUE(s_solved.valid());
};


TEST(sudoku_test, generate) {
   Sudoku s2 = generate(2);
   Sudoku s4 = generate(4);
   Sudoku s6 = generate(6);
   Sudoku s9 = generate(9);
   vector<Move> steps;
   unsigned limit2 = 0;
   unsigned limit4 = 0;
   unsigned limit6 = 0;
   unsigned limit9 = 0;

   std::cout << "generated\n" << s2.str() << std::endl;
   std::cout << "solved\n"    << solve(s2, steps, limit2).str() << std::endl;
   
   std::cout << "generated\n" << s4.str() << std::endl;
   std::cout << "solved\n"    << solve(s4, steps, limit4).str() << std::endl;

   std::cout << "generated\n" << s6.str() << std::endl;
   std::cout << "solved\n"    << solve(s6, steps, limit6).str() << std::endl;

   std::cout << "generated\n" << s9.str() << std::endl;
   std::cout << "solved\n"    << solve(s9, steps, limit9).str() << std::endl;
};
