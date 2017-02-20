class RecAdjustView : public AppView
{
public:
   RecAdjustView(App& app): AppView(app),
   select_bar(NULL),
   done(20,  20, 100, 45,   "Done",  !flip_y)
   {
      done.background_color(yellow);
      add_ctrl(done);
   }

   void enter()
   {
      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();
      on_resize(w, h);
      app.wait_mode(false);
   }

   virtual void on_resize(int sx, int sy)
   {
      tl_x = 20;
      tl_y = sy-20;
      br_x = sx-20;
      br_y = 200;
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
         select_bar = new gSelect_bar(120, 60, app.sudoku.getPossible(selected_x, selected_y), x, y);
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

   void on_ctrl_change()
   {
      if (done.status())
      {
         done.status(false);
         app.changeView("game");
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

      agg::render_ctrl(ras, sl, rbase, done);
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

   void move(int x, int y, int value)
   {
      Move m(value, x, y);
      app.sudoku.move(Move(value, x, y));
      if (value)
      {
         gobjs.push_back(new gDigit(
                  colToPixI(x),
                  rowToPixI(y),
                  value));
      }

      errors.clear();
      for (int y = 0; y < app.sudoku.getDim(); y++)
         for (int x = 0; x < app.sudoku.getDim(); x++)
         {
            if (app.sudoku.isEmpty(x,y) and app.sudoku.getPossible(x,y).size() == 0)
               errors.push_back(std::pair<int,int>(x,y));
         }
   }

   void update(long elapsed_time)
   {
      for(auto it = gobjs.begin(); it != gobjs.end(); it++)
      {
         (*it)->update(1.0*elapsed_time/1000);
      }
   }

   agg::button_ctrl<agg::rgba8> done;
   std::vector<gobj*> gobjs;
   std::vector<std::pair<int,int> > errors;
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
