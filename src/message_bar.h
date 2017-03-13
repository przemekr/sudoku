class gMsgBar: public gobj
{
public:
   gMsgBar (double x, double y): gobj(x, y), cover(0), c(0.8, 0, 0), size(20.0)
   {
   };

   void operator()(std::string mesage)
   {
      text = mesage;
      cover = 1.0;
   }


   void draw(agg::renderer_base<pixfmt_type> rbase)
   {
      if (not cover)
         return;

      double tickness = 1.4;
      agg::rgba color(c, cover);
      agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt_type> > ren(rbase);
      agg::gsv_text txt;
      agg::conv_stroke<agg::gsv_text> txt_stroke(txt);
      txt_stroke.width(size/6*tickness);
      txt_stroke.line_cap(agg::round_cap);
      txt.size(size);
      txt.start_point(tl_x, tl_y);
      txt.text(text.c_str());
      ras.add_path(txt_stroke);
      ren.color(color);
      agg::render_scanlines(ras, sl, ren);
   }

   void update(double time)
   {
      cover = std::max(0.0, cover-time/6);
   }
private:
   std::string text;
   double cover;
   agg::rgba c;
   double size;
};
