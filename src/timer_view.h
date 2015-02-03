
class TimerView : public AppView
{
public:
   TimerView(App& app): AppView(app),
   menu(80,  20, 180, 40,   "Menu", !flip_y)
   {
      menu.background_color(red);
      add_ctrl(menu);
   }

   void on_mouse_button_up(int x, int y, unsigned flags)
   {
      if (m_ctrls.on_mouse_button_up(x, y))
      {
         app.on_ctrl_change();
         app.force_redraw();
      }
   }

   void on_mouse_button_down(int x, int y, unsigned flags)
   {
      if (m_ctrls.on_mouse_button_down(x, y))
      {
         app.on_ctrl_change();
         app.force_redraw();
      }
   }

   void on_mouse_move(int x, int y, unsigned flags)
   {
      if (m_ctrls.on_mouse_move(x, y, (flags & agg::mouse_left) != 0))
      {
         app.on_ctrl_change();
         app.force_redraw();
         return;
      }
   }

   void on_ctrl_change()
   {
      if (menu.status())
      {
         menu.status(false);
         app.changeView("menu");
      }
   }

   void on_multi_gesture(float x, float y,
         float dTheta, float dDist, int numFingers)
   {
   }

   void on_draw()
   {
      pixfmt_type pf(app.rbuf_window());;
      agg::renderer_base<pixfmt_type> rbase(pf);
      agg::rasterizer_scanline_aa<> ras;
      agg::scanline_u8 sl;
      ras.reset();
      rbase.clear(black);

      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();

      agg::render_ctrl(ras, sl, rbase, menu);
   }

private:
   void update(long elapsed_time)
   {
   }
   agg::button_ctrl<agg::rgba8> menu;
};

