class LoadView : public AppView
{
public:
   LoadView(App& app): AppView(app),
      back(20, 20, 100, 45,   "Cancel",  !flip_y)
   {
      back.background_color(yellow);
      add_ctrl(back);
   }

   void enter()
   {
      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();
      int yoff = 80;
      background = 5+rand()%3;

      auto list = app.save_list();
      buttons.clear();
      m_ctrls.m_num_ctrl = 1;

      for (auto i = list.begin(); i != list.end(); i++)
      {
         auto b = new agg::button_ctrl<agg::rgba8>(120, 20+yoff, 380, 50+yoff, i->c_str(), !flip_y);
         b->background_color(yellow);
         buttons.push_back(b);
         yoff += 60;
         add_ctrl(*b);
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
      }
   }

   void on_key(int x, int y, unsigned key, unsigned flags)
   {
      if (key == 27 || key == 0x4000010e /* Android BACK key*/)
      {
         back.status(true);
         on_ctrl_change();
      }
   }

   void on_ctrl_change()
   {
      if (back.status())
      {
         back.status(false);
         app.changeView("game");
      }
      for (auto i = buttons.begin(); i != buttons.end(); i++)
      {
         if ((*i)->status())
         {
            std::cout << "button " << (*i)->label() << std::endl;
            app.save_load((*i)->label());
            app.changeView("game");
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
      rbase.copy_from(app.rbuf_img(background), 0, 0, 0);

      agg::render_ctrl(ras, sl, rbase, back);
      for (auto i = buttons.begin(); i != buttons.end(); i++)
      {
         agg::render_ctrl(ras, sl, rbase, **i);
      }
   }

private:
   agg::button_ctrl<agg::rgba8> back;
   std::vector<agg::button_ctrl<agg::rgba8>* > buttons;
   int background;
};
