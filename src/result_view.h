class ResultView : public View
{
public:
   ResultView(App& application): app(application),
   newGame (20,  20, 120, 40,   "New Game",  !flip_y)
   {
      newGame.background_color(red);
      add_ctrl(newGame);
      bestSec[0] = 0;
      bestSec[1] = 0;
      bestSec[2] = 0;
      ERROR_PRINT("RESULT VIEW open_rw !!!!!");
      resultsf = app.open_rw_file("traffar.se", "24gamea", "results");
      if (resultsf)
         fread(bestSec, sizeof(int), 3, resultsf);
   }

   void enter()
   {
      img_scale = 0.9;
      i = 0;
      imgIdx = 9 + rand()%2;

      app.wait_mode(false);

      if (!bestSec[0] || app.gameSec < bestSec[0])
      {
         bestSec[2] = bestSec[1];
         bestSec[1] = bestSec[0];
         bestSec[0] = app.gameSec;
      }
      else if (!bestSec[1] || app.gameSec < bestSec[1])
      {
         bestSec[2] = bestSec[1];
         bestSec[1] = app.gameSec;
      }
      else if (!bestSec[2] || app.gameSec < bestSec[2])
      {
         bestSec[2] = app.gameSec;
      }

      if (resultsf)
      {
         rewind(resultsf);
         fwrite(bestSec, sizeof(int), 3, resultsf);
         fflush(resultsf);
      }

      if (app.sound)
      {
         app.play_sound(0, 1000);
      }
   }

   void on_idle()
   {
      const int MAX_FPS = 14;
      int loop_time = app.elapsed_time();
      if (loop_time < 1000/MAX_FPS)
      {
         usleep(1000*(1000/MAX_FPS-loop_time));
      }
      app.start_timer();
      update();
      app.force_redraw();
   }


   void on_draw()
   {
      pixfmt_type   pf(app.rbuf_window());
      pixfmt_pre    pixf_pre(app.rbuf_window());
      agg::renderer_base<pixfmt_type> rbase(pf);
      renderer_base_pre rb_pre(pixf_pre);
      agg::rasterizer_scanline_aa<> ras;
      agg::scanline_u8 sl;
      ras.reset();
      rbase.clear(lgray);

      double scale = app.rbuf_window().width()/380.0;
      static agg::trans_affine shape_mtx; shape_mtx.reset();
      shape_mtx *= agg::trans_affine_scaling(scale);
      newGame.transform(shape_mtx);
      agg::render_ctrl(ras, sl, rbase, newGame);

      //pixfmt_type img_pixf(app.rbuf_img(imgIdx));
      //rbase.copy_from(app.rbuf_img(imgIdx), 0, app.rbuf_window().width()-app.rbuf_img(imgIdx).width(), 0);
      

      int x1 = app.rbuf_window().width()-app.rbuf_img(imgIdx).width();
      int x2 = app.rbuf_window().width();
      int y1 = 0;
      int y2 = app.rbuf_img(imgIdx).height();

      static agg::trans_affine img_mtx; img_mtx.reset();
      img_mtx *= agg::trans_affine_scaling(img_scale);
      img_mtx *= agg::trans_affine_translation(x1, y1);
      img_mtx.invert();

      typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
      typedef agg::image_accessor_clone<pixfmt_type> img_accessor_type;
      interpolator_type interpolator(img_mtx);
      pixfmt_type pixf_img(app.rbuf_img(imgIdx));
      img_accessor_type ia(pixf_img);
      typedef agg::span_image_filter_rgba_2x2<img_accessor_type,
              interpolator_type> span_gen_type;
      agg::image_filter<agg::image_filter_kaiser> filter;
      span_gen_type sg(ia, interpolator, filter);
      agg::span_allocator<color_type> sa;
      ras.move_to_d(x1,y1);
      ras.line_to_d(x2,y1);
      ras.line_to_d(x2,y2);
      ras.line_to_d(x1,y2);
      agg::render_scanlines_aa(ras, sl, rbase, sa, sg);
      ras.reset();



      char string[200];
      sprintf(string, "Congratulations!\n\nYou finished the 24 game in %ds\n\n"
            "Top Times:\n\n%ds\n\n%ds\n\n%ds", app.gameSec,
            bestSec[0],
            bestSec[1],
            bestSec[2]);

      app.draw_text(40, 200*scale, 10*scale, string);
   }

   void on_ctrl_change()
   {
      if (newGame.status())
      {
         newGame.status(false);
         app.changeView("game");
         app.t = time(0);
         app.scores = 0;
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

private:
    App& app;
    agg::button_ctrl<agg::rgba8> newGame;
    int bestSec[3];
    FILE* resultsf;
    double img_scale;
    int i;
    int imgIdx;

    void update()
    {
       img_scale += 0.001;
       i++;
       if (i > 100)
          app.wait_mode(true);

    }
};
