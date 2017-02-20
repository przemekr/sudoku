#include <ocradlib.h>
#include <iostream>
#include "sudoku.h"

const int AREA = 100;
const int PATL = 9;

int tl_corrner[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

int br_corrner[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


class ImageRecognizer
{
public:
   ImageRecognizer(int x, int y, int w, int h, pixfmt_type& img):
      offx(x), offy(y), height(h), width(w), 
      tl_x(x), tl_y(h), br_x(w), br_y(y), pf(img)
   {
      printf("ImgRec %d.%d %d.%d\n", tl_x, tl_y, br_x, br_y);
   }


   int match_pattern(int* pat, int x, int y, bool debug=false)
   {
      int sum = 0;
      for (int ax = 0; ax < PATL; ax++)
      {
         for (int ay = 0; ay < PATL; ay++)
         {
            unsigned char r = pf.pixel(x+ax, y-ay).r;
            r = r>60?255:0;
            int diff =  std::abs((int)pat[PATL*ay+ax] - (int)r);
            sum += diff;
            if (debug)
            {
               //printf("%02x", pat[PATL*ay+ax]);
               //printf("%03d ", (int)r);
               printf("%03d-%03d=%03d ", pat[PATL*ay+ax], r, diff);
               //printf("%c", r > 60?' ':'#');
            }
         }
         if (debug)
         {
            printf("\n");
         }
      }
      return sum;
   }

   void find_corner()
   {
      int best_result = 1 << 30;
      int bx = offx;
      int by = height-AREA;
      printf("TL: x %d-%d y %d-%d\n", bx, bx+AREA, by, by+AREA);
      for (int x = bx; x < bx+AREA; x++)
         for (int y = by; y < by+AREA; y++)
         {
            int result = match_pattern(tl_corrner, x, y);
            if (result < best_result-100)
            {
               tl_x = x;
               tl_y = y;
               best_result = result;
            }
         }

      best_result = 1 << 30;
      bx = width - (AREA+PATL);
      by = offy + PATL;
      printf("BR: x %d-%d y %d-%d\n", bx, bx+AREA, by, by+AREA);
      for (int x = bx; x < bx+AREA; x++)
         for (int y = by; y < by+AREA; y++)
         {
            int result = match_pattern(br_corrner, x, y);
            if (result < best_result-100)
            {
               br_x = x+PATL;
               br_y = y-PATL;
               best_result = result;
            }
         }
   }

   char filter(char c)
   {
      //if (c == 'D' or c == 'O' or c == 'Q' or c == 'o') return '0';
      if (c == 'I' or c == 'l')  return '1';
      if (c == 'Z' or c == 'z')  return '2';
      if (c == 'A' or c == 'q')  return '4';
      if (c == 'S' or c == 's')  return '5';
      if (c == 'G' or c == 'b')  return '6';
      if (c == 'J' or c == 'T')  return '7';
      if (c == '&' or c == 'B')  return '8';
      if (c == 'g')              return '9';
      return c;
   }

   int get_digit(struct OCRAD_Descriptor* desc)
   {
      if (OCRAD_result_blocks(desc) != 0)
      {
         const char* result_line = OCRAD_result_line(desc, 0, 0);
         for (int i = 0; i < strlen(result_line); i++)
         {
            char c = filter(result_line[i]);
            if (c >= '0' and c <= '9')
            {
               return c - '0';
            }
         }
      }
      return 0;
   }

   void recognize_digits()
   {
      int digits[9][9];

      int tgt_size_x = br_x - tl_x;
      int tgt_size_y = tl_y - br_y;
      int sqxsize = tgt_size_x/9;
      int sqysize = tgt_size_y/9;
      printf("recognize_digits %d.%d %d.%d\n", tl_x,tl_y, br_x,br_y);
      printf("recognize_digits %d.%d %d.%d\n", tgt_size_x,tgt_size_y, sqxsize,sqysize);

      uint8_t* pixels = (uint8_t*)malloc(sqysize*sqxsize*sizeof(char));
      OCRAD_Pixmap pixmap;
      pixmap.height = sqysize - PATL;
      pixmap.width  = sqxsize - PATL;
      pixmap.mode = OCRAD_greymap;
      pixmap.data = pixels;

      for (int x = 0; x < 9; x++)
      {
         for (int y = 0; y < 9; y++)
         {
            int bx = tl_x + x*sqxsize;
            int by = br_y + y*sqysize;
            struct OCRAD_Descriptor* desc = OCRAD_open();

            for (int ya = 3; ya < sqysize-3; ya++)
            {
               for (int xa = 3; xa < sqxsize-3; xa++)
               {
                  printf("%c", pf.pixel(bx+xa, by+ya).r>60? '#':' ');
                  pixels[(sqysize-3-ya)*(sqxsize-PATL)+xa] = pf.pixel(bx+xa, by+ya).r;
               }
               printf("\n");
            }
            int ret1 = OCRAD_set_image(desc, &pixmap, false);
            int ret2 = OCRAD_set_threshold(desc, -1);
            int ret3 = OCRAD_set_utf8_format(desc, 0);
            int ret4 = OCRAD_recognize(desc, false);

            digits[y][x] = get_digit(desc);
            OCRAD_close(desc);
         }
         printf("\n\n");
      }
         
      Digits d;
      for (int i = 0; i < 9; i++)
      {
         d.insert(d.end(), digits[i], digits[i]+9);
      }
      sudoku = Sudoku(d);
   }
   int offx;
   int offy;
   int tl_x;
   int tl_y;
   int br_x;
   int br_y;
   int height;
   int width;
   Sudoku sudoku;
   pixfmt_type pf;
};
