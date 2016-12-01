class gobj {
public:
   gobj (double x, double y): tl_x(x), tl_y(y) {};
   virtual ~gobj () {};

   virtual void draw(agg::renderer_base<pixfmt_type> rbase)
   {
      ras.reset();
   }

   virtual void update(double time)
   {
   }

   virtual bool wait_mode()
   {
      return true;
   }

   double tl_x;
   double tl_y;
   agg::rasterizer_scanline_aa<> ras;
   agg::scanline_u8 sl;
};
