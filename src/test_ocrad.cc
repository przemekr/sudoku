#include <stdint.h>
#include <gtest/gtest.h>
#include <ocradlib.h>


TEST(test_ocrad, loadfile)
{
      struct OCRAD_Descriptor* desc = OCRAD_open();
      int ret1 = OCRAD_set_image_from_file(desc, "sudoku.pbm", false);
      int ret3 = OCRAD_set_utf8_format(desc, 0);
      int ret4 = OCRAD_recognize(desc, false);

      for (int b = 0; b < OCRAD_result_blocks(desc); b++)
         for (int l = 0; l < OCRAD_result_lines(desc, b); l++)
            printf("b:%d, l:%d %s\n", b,l,OCRAD_result_line(desc, b, l));
      OCRAD_close(desc);
};
