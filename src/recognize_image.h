#include <ocradlib.h>
#include <iostream>
#include "sudoku.h"

class ImageRecognizer
{
public:
   ImageRecognizer(int w, int h, pixfmt_type& img):
      height(h), width(w),
      tl_x(0), tl_y(0), br_x(0), br_y(0), pf(img)
   {
      find_corner();
      recognize_digits();
   }

   void find_corner()
   {
      unsigned char tl_corrner[] = {
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
         0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
         0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
         0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
         0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
      };

      unsigned char br_corrner[] = {
         0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
         0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
         0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
         0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
         0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      };

      int min_sum = 1 << 30;
      int bx = 0;
      int by = height - 100;

      for (int x = 0; x < 100; x++)
         for (int y = 0; y < 100; y++)
         {
            int sum = 0;
            for (int ax = 0; ax < 7; ax++)
               for (int ay = 0; ay < 7; ay++)
               {
                  unsigned char r = pf.pixel(bx+x+ax, by+y-ay).r;
                  int diff =  std::abs((int)tl_corrner[7*ay+ax] - (int)r);
                  sum += diff;
               }
            if (sum < min_sum)
            {
               tl_x = bx+x+7;
               tl_y = by+y-7;
               min_sum = sum;
            }
         }

      min_sum = 1 << 30;
      bx = width - 107;
      by = 7;
      for (int x = 0; x < 100; x++)
         for (int y = 0; y < 100; y++)
         {
            int sum = 0;
            for (int ax = 0; ax < 7; ax++)
               for (int ay = 0; ay < 7; ay++)
               {
                  unsigned char r = pf.pixel(bx+x+ax, by+y-ay).r;
                  int diff =  std::abs((int)br_corrner[7*ay+ax] - (int)r);
                  sum += diff;
               }
            if (sum < min_sum)
            {
               br_x = bx+x;
               br_y = by+y;
               min_sum = sum;
            }
         }
   }

   void recognize_digits()
   {
      int digits[9][9];

      int tgt_size_x = br_x - tl_x;
      int tgt_size_y = tl_y - br_y;
      int sqxsize = tgt_size_x/9;
      int sqysize = tgt_size_y/9;

      uint8_t* pixels = (uint8_t*)malloc(sqysize*sqxsize*sizeof(char));
      OCRAD_Pixmap pixmap;
      pixmap.height = sqysize - 7;
      pixmap.width  = sqxsize - 7;
      pixmap.mode = OCRAD_greymap;
      pixmap.data = pixels;

      for (int x = 0; x < 9; x++)
         for (int y = 0; y < 9; y++)
         {
            int bx = tl_x + x*sqxsize;
            int by = br_y + y*sqysize;
            struct OCRAD_Descriptor* desc = OCRAD_open();

            for (int ya = 3; ya < sqysize-3; ya++)
            {
               for (int xa = 3; xa < sqxsize-3; xa++)
               {
                  pixels[(sqysize-3-ya)*(sqxsize-7)+xa] = pf.pixel(bx+xa, by+ya).r;
               }
            }
            int ret1 = OCRAD_set_image(desc, &pixmap, false);
            int ret2 = OCRAD_set_threshold(desc, 127);
            int ret3 = OCRAD_set_utf8_format(desc, 0);
            int ret4 = OCRAD_recognize(desc, false);

            digits[y][x] = 0;
            if (OCRAD_result_blocks(desc) != 0)
            {
               const char* result_line = OCRAD_result_line(desc, 0, 0);
               for (int i = 0; i < strlen(result_line); i++)
               {
                  if (result_line[i] >= '0' and result_line[i] <= '9')
                  {
                     digits[y][x] = result_line[i] - '0';
                  }
               }
            }
            OCRAD_close(desc);
         }
      Digits d;
      for (int i = 0; i < 9; i++)
      {
         d.insert(d.end(), digits[i], digits[i]+9);
      }
      sudoku = Sudoku(d);
   }
   int tl_x;
   int tl_y;
   int br_x;
   int br_y;
   int height;
   int width;
   Sudoku sudoku;
   pixfmt_type pf;
};
