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
#include <cctype>
#include <string>
#include <stack>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#ifdef ANDROID
#include <jni.h>
#include <android/log.h>
#include <SDL_android.h>
#endif


#include "app_support.h"
#include "game_view.h"
#include "menu_view.h"
#include "load_view.h"
#include "recognize_view.h"


class the_application: public App
{
public:
   the_application(agg::pix_format_e format, bool flip_y):
      App(format, !flip_y)
   {
      game = new GameView(*this);
      menu  = new MenuView(*this);
      recognize = new RecognizeView(*this);
      load = new LoadView(*this);
      view = game;
   }
   virtual void changeView(const char* name) 
   {
      printf("change view %s\n", name);
      if (strcmp(name, "menu") == 0)
         view = menu;
      if (strcmp(name, "game") == 0)
         view = game;
      if (strcmp(name, "recognize") == 0)
         view = recognize;
      if (strcmp(name, "load") == 0)
         view = load;
      view->enter();
   };
private:
   GameView* game;
   MenuView* menu;
   RecognizeView* recognize;
   LoadView* load;
};

static the_application* _app = NULL;

int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgra32, !flip_y);
    app.caption("Sudoku Kuai");
    _app = &app;

    if (app.init(START_W, START_H, WINDOW_FLAGS))
    {
       try {
          app.load_img(5, "leaves.png");
          app.load_img(6, "panda.png");
          app.load_img(7, "sun.png");
          app.sudoku = generate(9, 0.6);
          app.changeView("game");
          return app.run();
       } catch (...) {
          return 0;
       }
    }
    return 1;
}


#ifdef __ANDROID__
#include <jni.h>

extern "C" {
   JNIEXPORT void JNICALL Java_com_traffar_sudoku_activity_onTakePhotoResult(
                                    JNIEnv* env, jclass cls)
   {
      __android_log_print(ANDROID_LOG_INFO, "SUDOKU", "onTakePhotoResult");
      _app->load_img(0, "/storage/sdcard0/DCIM/Camera/sudoku.jpg");
      _app->changeView("recognize");
   }
}
#endif
