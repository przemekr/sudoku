class gSelect_bar: public gobj
{
public:
   gSelect_bar (double x, double y, std::vector<int> d, int in_x, int in_y):
      gobj(x, y), selected(0), init_x(x), init_y(y)
   {
      digits.push_back(new gDigit(x, y, 0));
      for (auto it = d.begin(); it != d.end(); it++)
      {
         x += 50;
         digits.push_back(new gDigit(x, y, *it));
      }
      scroll(init_x, init_y);
   };
   ~gSelect_bar ()
   {
      for (auto it = digits.begin(); it != digits.end(); it++)
      {
         delete *it;
      }
   }

   int getSelected()
   {
      if (not digits.size())
         return 0;

      return digits[selected]->getValue();
   }

   void scroll(int x, int y)
   {
      if (not digits.size())
         return;

      digits[selected]->set(20, red);
      selected = (x - init_x)/10% digits.size();
      digits[selected]->set(23, green);
   }

   void draw(agg::renderer_base<pixfmt_type> rbase)
   {
      for (auto it = digits.begin(); it != digits.end(); it++)
      {
         (*it)->draw(rbase);
      }
   }

   void update(double time)
   {
      for (auto it = digits.begin(); it != digits.end(); it++)
      {
         (*it)->update(time);
      }
   }
private:
   std::vector<gDigit*> digits;
   int init_x;
   int init_y;
   int selected;
};
