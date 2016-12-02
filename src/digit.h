class gDigit: public gobj
{
public:
   gDigit (double x, double y, int in_d): gobj(x, y), d(in_d), cover(0), c(0.8, 0, 0), size(20.0)
   {
      t[0] = '0'+d;
      t[1] = '\0';
   };
   virtual ~gDigit () {};

   int getValue()
   {
      return d;
   }

   void set(double in_size, agg::rgba in_c)
   {
      c = in_c;
      size = in_size;
   }

   void draw(agg::renderer_base<pixfmt_type> rbase)
   {
      double tickness = 1.4;
      agg::rgba color(c, cover);
      agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt_type> > ren(rbase);
      agg::gsv_text txt;
      agg::conv_stroke<agg::gsv_text> txt_stroke(txt);
      txt_stroke.width(size/6*tickness);
      txt_stroke.line_cap(agg::round_cap);
      txt.size(size);
      txt.start_point(tl_x, tl_y);
      txt.text(t);
      ras.add_path(txt_stroke);
      ren.color(color);
      agg::render_scanlines(ras, sl, ren);
   }

   void update(double time)
   {
      cover = std::min(1.0, cover+time/2);
   }
private:
   int d;
   char t[2];
   double cover;
   agg::rgba c;
   double size;
};
