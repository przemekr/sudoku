#include "interactive_polygon.h"
#include "agg_trans_bilinear.h"
class RecognizeView : public AppView
{
public:
   RecognizeView(App& app):
      AppView(app),
      m_quad(4, 13.0),
      corrner_moved(true),
      done(20,  20, 100, 45,   "Done",  !flip_y)
   {
      done.background_color(yellow);
      add_ctrl(done);
   }

   void enter()
   {
      app.wait_mode(false);
#ifndef __ANDROID__
      app.load_img(0, "sudoku.jpg");
#endif
      int w = app.rbuf_img(0).width();
      int h = app.rbuf_img(0).height();
      double scale = std::max(START_W, START_H)*1.0/std::max(w, h);
      app.scale_img(0, scale*w, scale*h, 1);
      if (w > h)
      {
         app.rotate_img(1, -agg::pi/2, 0);
      } else {
         app.copy_img_to_img(0, 1);
      } 

      w = app.rbuf_img(0).width();
      h = app.rbuf_img(0).height();

      //find center of the image
      int cx = w/2;
      int cy = h/2;

      //cut a square
      int shorter = std::min(cx,cy);
      int xoff = cx - shorter;
      int yoff = cy - shorter;

      pixfmt_type pf(app.rbuf_img(0));
      ImageRecognizer imgRec(xoff, yoff, xoff+2*shorter, yoff+2*shorter, pf);
      imgRec.find_corner();
      m_quad.xn(0) = imgRec.tl_x;
      m_quad.yn(0) = imgRec.br_y;
      m_quad.xn(1) = imgRec.br_x;
      m_quad.yn(1) = imgRec.br_y;
      m_quad.xn(2) = imgRec.br_x;
      m_quad.yn(2) = imgRec.tl_y;
      m_quad.xn(3) = imgRec.tl_x;
      m_quad.yn(3) = imgRec.tl_y;

      w = app.rbuf_window().width();
      h = app.rbuf_window().height();
      t_x1 = 20;
      t_x2 = w-20;
      t_y2 = h-100;
      t_y1 = t_y2-(t_x2-t_x1);
      force_redraw();
      corrner_moved = true;
   }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if (flags & agg::mouse_left)
        {
            if (m_quad.on_mouse_move(x, y))
            {
                force_redraw();
            }
        }
    }


    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
        if (m_quad.on_mouse_button_up(x, y))
        {
            force_redraw();
            corrner_moved = true;
        }
        if (m_ctrls.on_mouse_button_up(x, y))
        {
           app.on_ctrl_change();
           app.force_redraw();
        }
    }


   void on_mouse_button_down(int x, int y, unsigned flags)
   {
      int w = app.rbuf_window().width();
      int h = app.rbuf_window().height();

      if (flags & agg::mouse_left)
      {
         if (m_quad.on_mouse_button_down(x, y))
         {
            force_redraw();
         }
      }
      if (m_ctrls.on_mouse_button_down(x, y))
      {
         app.on_ctrl_change();
         app.force_redraw();
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

      agg::rect_i rec(0, 0, w, h);
      pixfmt_type pf2(app.rbuf_img(0));
      rbase.blend_from(pf2, &rec, 0, 0, 100);

      agg::trans_bilinear tr(t_x1, t_y1, t_x2, t_y2, m_quad.polygon());
      if (tr.is_valid() and corrner_moved)
      {
         typedef agg::image_accessor_clone<pixfmt_type> img_accessor_type;
         agg::image_filter_bilinear filter_kernel;
         agg::image_filter_lut filter(filter_kernel, false);
         agg::span_allocator<color_type> sa;
         typedef agg::span_interpolator_linear<agg::trans_bilinear> interpolator_type;
         interpolator_type interpolator(tr);
         typedef agg::span_image_filter_rgba_2x2<img_accessor_type, interpolator_type> span_gen_type;
         pixfmt_type pixf_img(app.rbuf_img(0));
         img_accessor_type ia(pixf_img);
         span_gen_type sg(ia, interpolator, filter);

         int lw = t_x2-t_x1;
         int lh = t_y2-t_y1;
         //app.create_img(3, lw, lh);

         app.create_img(3, w, h);
         pixfmt_type pfImg(app.rbuf_img(3));
         agg::renderer_base<pixfmt_type> rbaseImg(pfImg);

         // Prepare the polygon to rasterize. Here we need to fill
         // the destination (transformed) polygon.
         ras.clip_box(0, 0, width(), height());
         ras.reset();
         ras.move_to_d(t_x1, t_y1);
         ras.line_to_d(t_x2, t_y1);
         ras.line_to_d(t_x2, t_y2);
         ras.line_to_d(t_x1, t_y2);
         agg::render_scanlines_aa(ras, sl, rbaseImg, sa, sg);

         ImageRecognizer imgRec(t_x1, t_y1, t_x2, t_y2, pfImg);
         imgRec.recognize_digits();
         printf("%s\n", imgRec.sudoku.str().c_str());

         app.sudoku = imgRec.sudoku;
         corrner_moved = false;
      }

      //--------------------------
      // Render the "quad" tool and controls
      ras.reset();
      ras.add_path(m_quad);
      agg::render_scanlines_aa_solid(ras, sl, rbase, 
            agg::rgba(0.9, 0.3, 0.0, 0.9));
      agg::render_ctrl(ras, sl, rbase, done);
   }

   void on_ctrl_change()
   {
      if (done.status())
      {
         done.status(false);
         app.changeView("game");
      }
   }

private:
   int br_x;
   int br_y;
   int tl_x;
   int tl_y;
   double  t_x1;
   double  t_y1;
   double  t_x2;
   double  t_y2;
   agg::interactive_polygon   m_quad;
   bool corrner_moved;
   agg::button_ctrl<agg::rgba8> done;
};
