#include "recognize_image.h"
#include "gobj.h"
#include "digit.h"
#include "select_bar.h"

class GameView : public AppView
{
public:
   GameView(App& app): AppView(app),
   cnt(0), bgc(0), stepIdx(0),
   select_bar(20, 700),
   new_game(20,  20, 100, 45,   "New",  !flip_y),
   scan(120, 20, 200, 45,   "Scan", !flip_y),
   hint(220, 20, 300, 45,   "Hint", !flip_y),
   undo(320, 20, 400, 45,   "Undo", !flip_y),
   solveB(420, 20, 500, 45, "Solve", !flip_y)
   {
      new_game.background_color(yellow);
      scan.background_color(yellow);
      hint.background_color(yellow);
      undo.background_color(yellow);
      solveB.background_color(yellow);
      add_ctrl(new_game);
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

      sudoku = app.sudoku;
      on_resize(w, h);

      stepIdx = 0;
      app.wait_mode(false);

      errors.clear();
      for (int y = 0; y < sudoku.getDim(); y++)
         for (int x = 0; x < sudoku.getDim(); x++)
         {
            if (sudoku.isEmpty(x,y) and sudoku.getPossible(x,y).size() == 0)
               errors.push_back(std::pair<int,int>(x,y));
         }
   }

   void on_mouse_button_up(int x, int y, unsigned flags)
   {
      if (select_bar.is_active())
      {
         int value = select_bar.getSelected();
         select_bar.clear();
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
         select_bar.set(x, y, sudoku.getPossible(selected_x, selected_y));
      }

      if (m_ctrls.on_mouse_button_down(x, y))
      {
         app.on_ctrl_change();
         app.force_redraw();
      }
   }

   void on_mouse_move(int x, int y, unsigned flags)
   {
      if (select_bar.is_active() && (flags & agg::mouse_left))
      {
         select_bar.scroll(x, y);
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
      tl_y = sy-100;
      br_x = sx-20;
      br_y = tl_y-(br_x-tl_x);
      select_bar.tl_y = sy-80;

      xsize = (br_x - tl_x)/(double)(sudoku.getDim()?:9);
      ysize = (tl_y - br_y)/(double)(sudoku.getDim()?:9);

      // re-create all elements
      for (int i = 0; i < digits.size(); i++)
      {
         delete digits[i];
      }
      digits.resize(0);

      for (int j = 0; j < sudoku.getDim(); j++)
      {
         Digits d = sudoku.getRow(j);
         int i = 0;
         for (auto it = d.begin(); it != d.end(); it++)
         {
            if (*it)
            {
               digits.push_back(new gDigit(
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
      if (new_game.status())
      {
         new_game.status(false);
         moves.clear();
         steps.clear();
         app.sudoku = generate(9, 0.6);
         app.changeView("game");
      }
      if (undo.status())
      {
         undo.status(false);
         undoMove();
      }
      if (scan.status() && strcmp(scan.label(), "Scan") == 0)
      {
         scan.label("Done");
#ifdef ANDROID
         __android_log_print(ANDROID_LOG_INFO, "SUDOKU", "Start Scan");
         JNIEnv *env = Android_JNI_GetEnv();
         __android_log_print(ANDROID_LOG_INFO, "SUDOKU", "Got env %p", env);
         jclass cls = env->FindClass("com/traffar/sudoku/activity");
         __android_log_print(ANDROID_LOG_INFO, "SUDOKU", "Got cls %p", cls);
         jmethodID take_photo = env->GetStaticMethodID(cls, "take_photo", "()V");
         __android_log_print(ANDROID_LOG_INFO, "SUDOKU", "Got method %p", take_photo);
         env->CallStaticVoidMethod(cls, take_photo);
         __android_log_print(ANDROID_LOG_INFO, "SUDOKU", "Done call static method");
#else
         app.changeView("recognize");
#endif

      }
      if (not scan.status() && strcmp(scan.label(), "Done") == 0)
      {
         scan.label("Scan");
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
      rbase.copy_from(app.rbuf_img(2), 0, 0, 0);

      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();

      if (scan.status())
      {
         agg::rect_i rec(tl_x, br_y, br_x, tl_y);
         pixfmt_type pf2(app.rbuf_img(3));
         rbase.blend_from(pf2, &rec, 0, 0, 150);
      } else {
         rbase.blend_bar(tl_x, tl_y, br_x, br_y, white, 200);
      }


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

      for (auto it = digits.begin(); it != digits.end(); it++)
      {
         (*it)->draw(rbase);
      }
      select_bar.draw(rbase);

      agg::render_ctrl(ras, sl, rbase, new_game);
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
         digits.push_back(new gDigit(
                  colToPixI(x),
                  rowToPixI(y),
                  value));
         moves.push_back(m);
      } else if (scan.status())
      {
         for (auto it = digits.begin(); it != digits.end(); it++)
         {
            if ((*it)->tl_x == colToPixI(x) and (*it)->tl_y == rowToPixI(y))
            {
               delete *it;
               digits.erase(it);
               break;
            }
         }
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
      for(auto it = digits.begin(); it != digits.end(); it++)
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
      select_bar.update(elapsed_time);
   }
   agg::button_ctrl<agg::rgba8> new_game;
   agg::button_ctrl<agg::rgba8> scan;
   agg::button_ctrl<agg::rgba8> hint;
   agg::button_ctrl<agg::rgba8> undo;
   agg::button_ctrl<agg::rgba8> solveB;
   Sudoku sudoku;
   std::vector<Move> steps;
   std::vector<Move> moves;
   std::vector<gobj*> digits;
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
   gSelect_bar select_bar;
   int selected_x;
   int selected_y;
};
