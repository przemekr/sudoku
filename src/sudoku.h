#ifndef SUDOKU__
#define SUDOKU__

#include <vector>
#include <algorithm>
#include <sstream>
#include <set>
#include <ctime>
#include <math.h>

typedef std::vector<int> Digits;
struct NoSolution {};
struct MoveLimit {};

struct Move
{
   int x;
   int y;
   int value;
   Move(): x(0), y(0), value(0) { }
   Move(int in_v, int in_x, int in_y) :x(in_x), y(in_y), value(in_v) { }
   bool operator==(const Move& o) const
   {
      return x == o.x and y == o.y and value == o.value;
   }
};



class Sudoku
{
public:
   Sudoku ()
   {
      rows.resize(0);
      dim = 0;
   }
   Sudoku(const Sudoku& other):
      rows(other.rows),
      all(other.all),
      dim(other.dim),
      subsize(other.subsize)
   {
   }

   Sudoku (Digits rowdata)
   {
      dim = (int)sqrt((double)rowdata.size());
      subsize = (int)sqrt((double)dim);
      for (int i = 0; i < dim; i++)
      {
         Digits row(i*dim+rowdata.begin(), (i+1)*dim+rowdata.begin());
         rows.push_back(row);
         all.push_back(i);
      }
      all.push_back(dim);
   };
   virtual ~Sudoku () {};

   Digits getRow(int nr)
   {
      return rows[nr];
   }

   Digits getCol(int nr)
   {
      Digits col;
      for (auto i = rows.begin(); i != rows.end(); i++)
      {
         col.push_back((*i)[nr]);
      }
      return col;
   }

   Digits getSub(int x, int y)
   {
      Digits sub;

      for (auto i = rows.begin()+y; i != rows.begin()+y+subsize; i++)
      {
         sub.insert(sub.end(), i->begin()+x, i->begin()+x+subsize);
      }
      return sub;
   }

   std::string str()
   {
      std::stringstream ss;
      std::for_each(rows.rbegin(), rows.rend(), [&](Digits& d) {
            ss << "[";
            std::for_each(d.begin(), d.end(), [&](int a) {ss << " " << a; });
            ss << " ]\n";
            });
      return ss.str();
   }

   bool isEmpty(int x, int y)
   {
      return rows[y][x] == 0;
   }

   Digits getPossible(int x, int y)
   {
      int subsqx = x/subsize*subsize;
      int subsqy = y/subsize*subsize;
      Digits poss;
      Digits tmp;
      Digits taken;
      Digits col = getCol(x);
      Digits row = getRow(y);
      Digits sub = getSub(subsqx, subsqy);

      if (rows[y][x])
         return poss;

      std::sort(sub.begin(), sub.end());
      std::sort(col.begin(), col.end());
      std::sort(row.begin(), row.end());

      std::set_union(col.begin(), col.end(), row.begin(), row.end(), back_inserter(tmp));
      std::set_union(tmp.begin(), tmp.end(), sub.begin(), sub.end(), back_inserter(taken));
      std::set_difference(all.begin(), all.end(), taken.begin(), taken.end(), back_inserter(poss));
      return poss;
   }

   std::vector<Move> getPossiveMoves()
   {
      std::vector<Move> moves;
      int lowest = dim+1;
      for (int y = 0; y < dim; y++)
         for (int x = 0; x < dim; x++)
         {
            Digits a = getPossible(x,y);
            if (a.size() and a.size() < lowest)
            {
               moves.clear();
               lowest = a.size();
               for (auto it = a.begin(); it != a.end(); it++)
               {
                  moves.push_back(Move(*it, x, y));
               }
            }
         }
      return moves;
   }

   bool solved()
   {
      for (int y = 0; y < dim; y++)
         for (int x = 0; x < dim; x++)
            if (rows[y][x] == 0)
               return false;
      return true;
   }

   void move(Move m)
   {
      rows[m.y][m.x] = m.value;
   }

   bool uniq(Digits d)
   {
      std::vector<int> v(d.size()+1, 0);
      for (auto i = d.begin(); i != d.end(); i++)
      {
         v[*i]++;
         if (*i and v[*i] > 1)
            return false;
      }
      return true;
   }

   bool valid()
   {
      for (int i = 0; i < dim; i++)
         if (not uniq(getRow(i)))
            return false;

      for (int i = 0; i < dim; i++)
         if (not uniq(getCol(i)))
            return false;

      for (int i = 0; i < dim; i += subsize)
         for (int j = 0; j < dim; j += subsize)
         {
            if (not uniq(getSub(i, j)))
               return false;
         }
      return true;
   }

   int getDim()
   {
      return dim;
   }

private:
   std::vector<Digits> rows;
   Digits all;
   int dim;
   int subsize;
};

Sudoku solve(Sudoku s, std::vector<Move>& steps, unsigned& moveLimit, int index = 0)
{
   if (s.solved())
      return s;

   auto moves = s.getPossiveMoves();
   for (auto it = moves.begin(); it != moves.end(); it++)
   {
      Sudoku new_s(s);
      new_s.move(*it);
      try {
         steps.resize(std::max(steps.size(), (size_t)index+1));
         steps[index] = *it;
         return solve(new_s, steps, moveLimit, index+1);
      } catch (const NoSolution& e) { }
      if (--moveLimit == 0)
      {
         throw MoveLimit();
      }
   }
   throw NoSolution();
}

Sudoku generate(int dim, double level=0.7)
{
   for (int i = 0; i < 20; i++)
   {
      try {
         std::vector<Move> steps;
         std::srand(unsigned(std::time(0)));
         std::vector<int> rows(dim*dim, 0);
         Sudoku s(rows);
         for (int i = 0; i < dim; i++)
         {
            Digits poss = s.getPossible(i, 0);
            s.move(Move(poss[rand()%poss.size()], i, 0));
         }
         unsigned limit = 1000;
         Sudoku solved = solve(s, steps, limit);
         for (int i = 1; i <= level*dim*dim; i++)
         {
            while (true)
            {
               int x = rand()%dim;
               int y = rand()%dim;
               if (not solved.isEmpty(x, y))
               {
                  solved.move(Move(0, x, y));
                  break;
               }
            }
         }
         return solved;
      } catch (const MoveLimit& e) { }
   }
   return Sudoku(std::vector<int>(dim*dim, 0));
}

#endif /* end of: SUDOKU__ */
