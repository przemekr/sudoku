/*
 * 24-game, a simple arithmetic game.
 * Copyright 2014 Przemyslaw Rzepecki
 * Contact: przemekr@sdfeu.org
 * 
 * This file is part of 24-game.
 * 
 * 24-game is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * 24-game is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * 24-game.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <algorithm> 
#include <stack>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "app_support.h"
#include "timer_view.h"
#include "menu_view.h"


class the_application: public App
{
public:
   the_application(agg::pix_format_e format, bool flip_y) :
      App(format, flip_y)
   {
      timer = new TimerView(*this);
      menu  = new MenuView(*this);
      view = timer;
   }
   virtual void changeView(const char* name) 
   {
      if (strcmp(name, "menu") == 0)
         view = menu;
      if (strcmp(name, "timer") == 0)
         view = timer;
      view->enter();
   };
private:
   TimerView* timer;
   MenuView* menu;
};


int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgra32, flip_y);
    app.caption("AGG Timer");

    if (false
      // || !app.load_img(0, "1.png")
      // || !app.load_img(1, "2.png")
      // || !app.load_img(2, "3.png")
      // || !app.load_img(3, "4.png")
      // || !app.load_img(4, "5.png")
      // || !app.load_img(5, "6.png")
      // || !app.load_img(6, "7.png")
      // || !app.load_img(7, "8.png")
      // || !app.load_img(8, "9.png")
      // || !app.load_img(9, "you_are_a_star.png") 
      // || !app.load_img(10, "you_are_the_best.png")
      // || !app.load_img(11, "background.png")
      // || !app.load_sound(0, "24finish.ogg")
      // || !app.load_sound(1, "applouse.ogg")
      // || !app.load_sound(2, "applouse2.ogg")
      // || !app.load_sound(3, "clicked.ogg"))
       )
    {
        char buf[256];
        sprintf(buf, "There must be files 1%s...9%s\n",
                     app.img_ext(), app.img_ext());
        app.message(buf);
        return 1;
    }

    if (app.init(START_W, START_H, WINDOW_FLAGS))
    {
       try {
          return app.run();
       } catch (...) {
          return 0;
       }
    }
    return 1;
}
