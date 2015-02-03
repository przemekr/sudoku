#include "ctrl/agg_cbox_ctrl.h"

class MenuView : public View
{
public:
   MenuView(App& application): app(application),
   exitApp (30, 20, 130, 40,    "Quit App",  !flip_y),
   exitMenu(30, 50, 130, 70,    "Return  ",  !flip_y),
   newGame (30, 80, 130, 100,   "New Game",  !flip_y),
   sound   (150, 20,            "Sound  ",   !flip_y)
   {
      exitMenu.background_color(red);
      exitApp.background_color(red);
      newGame.background_color(red);
      sound.text_size(15);
      sound.text_color(red);
      sound.text_thickness(CTRL_TEXT_THICKNESS);
      sound.active_color(red);
      sound.inactive_color(red);
      add_ctrl(exitMenu);
      add_ctrl(exitApp);
      add_ctrl(newGame);
      add_ctrl(sound);
   }

   void on_draw()
   {
      double w = app.rbuf_window().width();
      double h = app.rbuf_window().height();

      pixfmt_type   pf(app.rbuf_window());
      pixfmt_pre    pixf_pre(app.rbuf_window());
      agg::renderer_base<pixfmt_type> rbase(pf);
      renderer_base_pre rb_pre(pixf_pre);
      agg::rasterizer_scanline_aa<> ras;
      agg::scanline_u8 sl;
      agg::span_allocator<color_type> sa;
      ras.reset();
      rbase.clear(lgray);

      double scale = app.rbuf_window().width()/380.0;
      static agg::trans_affine shape_mtx; shape_mtx.reset();
      shape_mtx *= agg::trans_affine_scaling(scale);
      shape_mtx *= agg::trans_affine_translation(0, 0);
      exitMenu.transform(shape_mtx);
      exitApp.transform(shape_mtx);
      newGame.transform(shape_mtx);
      sound.transform(shape_mtx);

      agg::render_ctrl(ras, sl, rbase, exitMenu);
      agg::render_ctrl(ras, sl, rbase, exitApp);
      agg::render_ctrl(ras, sl, rbase, newGame);
      agg::render_ctrl(ras, sl, rbase, sound);
   }

   void on_ctrl_change()
   {
      app.sound_on(sound.status());
      if (exitMenu.status())
      {
         exitMenu.status(false);
         app.changeView("game");
      }
      if (newGame.status())
      {
         newGame.status(false);
         app.changeView("timer");
      }
      if (exitApp.status())
      {
         throw 0;
      }
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
         return;
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

private:
    App& app;
    agg::button_ctrl<agg::rgba8> exitMenu;
    agg::button_ctrl<agg::rgba8> exitApp;
    agg::button_ctrl<agg::rgba8> newGame;
    agg::cbox_ctrl<agg::rgba8>   sound;
};
