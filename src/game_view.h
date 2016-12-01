#include "recognize_image.h"
#include "gobj.h"
#include "digit.h"
#include "select_bar.h"

class GameView : public AppView
{
public:
   GameView(App& app): AppView(app),
   cnt(0), bgc(0), stepIdx(0),
   select_bar(NULL),
   menu(20,  20, 100, 45,   "New",  !flip_y),
   scan(120, 20, 200, 45,   "Scan", !flip_y),
   hint(220, 20, 300, 45,   "Hint", !flip_y),
   undo(320, 20, 400, 45,   "Undo", !flip_y),
   solveB(420, 20, 500, 45,   "Solve", !flip_y)
   {
      menu.background_color(yellow);
      scan.background_color(yellow);
      hint.background_color(yellow);
      undo.background_color(yellow);
      solveB.background_color(yellow);
      add_ctrl(menu);
      add_ctrl(scan);
      add_ctrl(hint);
      add_ctrl(undo);
      add_ctrl(solveB);
   }

   void enter()
   {
      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();
      unsigned limit = 0;


      //pixfmt_type pf(app.rbuf_img(0));
      //imgRec = new ImageRecognizer(app.rbuf_img(0).width(), app.rbuf_img(0).height(), pf);

      sudoku = generate(9);
      on_resize(w, h);

      //std::cout << "recognized\n\n" << sudoku.str() << std::endl;
      //std::cout << "solved\n\n" << solve(sudoku, steps, limit).str() << std::endl;
      stepIdx = 0;
      app.wait_mode(false);
   }

   void on_mouse_button_up(int x, int y, unsigned flags)
   {
      if (select_bar)
      {
         int value = select_bar->getSelected();
         gobjs.erase(std::remove(gobjs.begin(), gobjs.end(), select_bar), gobjs.end());
         delete select_bar;
         select_bar = NULL;
         move(selected_x, selected_y, value);
      }
      if (m_ctrls.on_mouse_button_up(x, y))
      {
         app.on_ctrl_change();
         app.force_redraw();
      }
   }

   void on_mouse_button_down(int x, int y, unsigned flags)
   {
      if (pixToCol(x) >= 0 and pixToRow(y) >= 0)
      {
         selected_x = pixToCol(x);
         selected_y = pixToRow(y);
         if (select_bar)
         {
            gobjs.erase(std::remove(gobjs.begin(), gobjs.end(), select_bar), gobjs.end());
            delete select_bar;
            select_bar = NULL;
         }
         select_bar = new gSelect_bar(120, 60, sudoku.getPossible(selected_x, selected_y), x, y);
         gobjs.push_back(select_bar);
      }

      if (m_ctrls.on_mouse_button_down(x, y))
      {
         app.on_ctrl_change();
         app.force_redraw();
      }
   }

   void on_mouse_move(int x, int y, unsigned flags)
   {
      if (select_bar && (flags & agg::mouse_left))
      {
         select_bar->scroll(x, y);
      }

      if (m_ctrls.on_mouse_move(x, y, (flags & agg::mouse_left) != 0))
      {
         app.on_ctrl_change();
         app.force_redraw();
         return;
      }
   }

   virtual void on_resize(int sx, int sy)
   {
      tl_x = 20;
      tl_y = sy-20;

      br_x = sx-20;
      br_y = 200;

      xsize = (br_x - tl_x)/(sudoku.getDim()?:9);
      ysize = (tl_y - br_y)/(sudoku.getDim()?:9);

      // re-create all elements
      for (int i = 0; i < gobjs.size(); i++)
      {
         delete gobjs[i];
      }
      gobjs.resize(0);

      for (int j = 0; j < sudoku.getDim(); j++)
      {
         Digits d = sudoku.getRow(j);
         int i = 0;
         for (auto it = d.begin(); it != d.end(); it++)
         {
            if (*it)
            {
               gobjs.push_back(new gDigit(
                        tl_x+i*xsize+0.3*xsize,
                        br_y+j*ysize+0.3*ysize,
                        *it));
            }
            i++;
         }
      }
   }

   void on_key(int x, int y, unsigned key, unsigned flags)
   {
      if (key == 27 || key == 0x4000010e /* Android BACK key*/)
      {
         undo.status(true);
         on_ctrl_change();
      }
   }

   void on_ctrl_change()
   {
      if (menu.status())
      {
         menu.status(false);
         app.changeView("game");
      }
      if (undo.status())
      {
         undo.status(false);
         undoMove();
      }
      if (hint.status())
      {
         hint.status(false);
         if (errors.size() or not sudoku.valid())
         {
            return;
         }
         steps.clear();
         try {
            unsigned int limit = 5000;
            solve(sudoku, steps, limit);
            move(steps[0].x, steps[0].y, steps[0].value);
         } catch (const NoSolution& e)
         {
            std::cout << "No Solution!" << std::endl;
         } catch (const MoveLimit& e)
         {
            std::cout << "No Solution (Move Limit)!" << std::endl;
         }
      }
      if (solveB.status())
      {
         if (errors.size() or not sudoku.valid())
         {
            solveB.status(false);
            return;
         }
         steps.clear();
         try {
            unsigned int limit = 5000;
            solve(sudoku, steps, limit);
            stepIdx = 0;
         } catch (const NoSolution& e)
         {
            solveB.status(false);
            std::cout << "No Solution!" << std::endl;
         } catch (const MoveLimit& e)
         {
            solveB.status(false);
            std::cout << "No Solution (Move Limit)!" << std::endl;
         }
      }
   }

   void on_draw()
   {
      pixfmt_type pf(app.rbuf_window());
      agg::renderer_base<pixfmt_type> rbase(pf);
      agg::rasterizer_scanline_aa<> ras;
      agg::scanline_u8 sl;
      ras.reset();
      rbase.clear(lgray);

      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();

      rbase.blend_bar(tl_x, tl_y, br_x, br_y, white, 255);

      // draw grid
      for (int i = 0; i < 10; i++)
      {
         int hx = tl_x;
         int hy = br_y + i*ysize;
         rbase.blend_bar(hx, hy, hx+9*xsize, hy+(i%3 == 0? 3:1), black, (i%3 == 0? 255:128));

         int vx = tl_x + i*xsize;
         int vy = br_y;
         rbase.blend_bar(vx, vy, vx+(i%3 == 0? 3:1), vy+9*ysize, black, (i%3 == 0? 255:128));
      }

      // errors
      for (auto it = errors.begin(); it != errors.end(); it++)
      {
         rbase.blend_bar(colToPix(it->first), rowToPix(it->second), colToPix(it->first+1), rowToPix(it->second+1), red, 80);
      }

      for (auto it = gobjs.begin(); it != gobjs.end(); it++)
      {
         (*it)->draw(rbase);
      }

      agg::render_ctrl(ras, sl, rbase, menu);
      agg::render_ctrl(ras, sl, rbase, scan);
      agg::render_ctrl(ras, sl, rbase, hint);
      agg::render_ctrl(ras, sl, rbase, undo);
      agg::render_ctrl(ras, sl, rbase, solveB);

   }

   virtual int max_fps() { return 20; }

private:
   double colToPix(int col)
   {
      return tl_x+col*xsize;
   }
   double rowToPix(int row)
   {
      return br_y+row*ysize;
   }
   double colToPixI(int col)
   {
      return colToPix(col)+0.3*xsize;
   }
   double rowToPixI(int row)
   {
      return rowToPix(row)+0.3*ysize;
   }
   int pixToRow(int pix)
   {
      return 
         pix < br_y? -1:
         pix > tl_y? -1:
         (int)(pix - br_y)/ysize;
   }
   int pixToCol(int pix)
   {
      return 
         pix < tl_x? -1:
         pix > br_x? -1:
         (int)(pix - tl_x)/xsize;
   }

   void undoMove()
   {
      if (moves.empty())
         return;
      Move m = moves.back();
      moves.pop_back();
      move(m.x, m.y, 0);

      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();
      on_resize(w, h);

   }

   void move(int x, int y, int value)
   {
      Move m(value, x, y);
      sudoku.move(Move(value, x, y));
      if (value)
      {
         gobjs.push_back(new gDigit(
                  colToPixI(x),
                  rowToPixI(y),
                  value));
         moves.push_back(m);
      }

      errors.clear();
      for (int y = 0; y < sudoku.getDim(); y++)
         for (int x = 0; x < sudoku.getDim(); x++)
         {
            if (sudoku.isEmpty(x,y) and sudoku.getPossible(x,y).size() == 0)
               errors.push_back(std::pair<int,int>(x,y));
         }
   }

   void update(long elapsed_time)
   {
      for(auto it = gobjs.begin(); it != gobjs.end(); it++)
      {
         (*it)->update(1.0*elapsed_time/1000);
      }

      if (solveB.status() and stepIdx < steps.size())
      {
         move(steps[stepIdx].x, steps[stepIdx].y, steps[stepIdx].value);
         stepIdx++;
      }
      if (solveB.status() and stepIdx == steps.size())
      {
         solveB.status(false);
      }
   }
   agg::button_ctrl<agg::rgba8> menu;
   agg::button_ctrl<agg::rgba8> scan;
   agg::button_ctrl<agg::rgba8> hint;
   agg::button_ctrl<agg::rgba8> undo;
   agg::button_ctrl<agg::rgba8> solveB;
   Sudoku sudoku;
   ImageRecognizer* imgRec;
   std::vector<Move> steps;
   std::vector<Move> moves;
   std::vector<gobj*> gobjs;
   std::vector<std::pair<int,int> > errors;
   int stepIdx;
   int cnt;
   int bgc;
   double xsize;
   double ysize;
   int tl_x;
   int tl_y;
   int br_x;
   int br_y;
   gSelect_bar* select_bar;
   int selected_x;
   int selected_y;
};
