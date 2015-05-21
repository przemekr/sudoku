#!/usr/bin/env python
#encoding: utf-8
VERSION='0.0.1'
APPNAME='empty_agg'

top = '.'

from waflib import Configure, Logs

def options(opt):
        opt.load('compiler_cxx')
        opt.add_option('--double_click', action='store',
              default='0',
              dest='double_click')
        opt.add_option('--platform',
              action='store',
              default='sdl2',
              help='Graphical output to use. [sdl, sdl2, x11, w32]',
              dest='platform')

def configure(conf):
        conf.load('compiler_cxx')
        if conf.options.platform == 'sdl':
           conf.check_cfg(
                 path='sdl-config',
                 args='--cflags --libs', package = '',
                 uselib_store='platform')
           conf.env.PLATFORM_SRC = ['agg-2.5/src/platform/sdl/agg_platform_support.cpp']
        elif conf.options.platform == 'sdl2':
           conf.check_cfg(
                 path='sdl2-config',
                 args='--cflags --libs', package = '',
                 uselib_store='platform')
           conf.env.PLATFORM_SRC = ['agg-2.5/src/platform/sdl2/agg_platform_support.cpp']

        elif conf.options.platform == 'x11':
           conf.env.PLATFORM_SRC = ['agg-2.5/src/platform/X11/agg_platform_support.cpp']
           conf.check_cc(lib='X11', uselib_store='platform',
                 msg = "Checking X11 library")
           conf.check_cc(header='X11/Xlib.h',
                 msg = 'Checking X11 standard headers')

        elif conf.options.platform == 'win':
           conf.env.PLATFORM_SRC = ['agg-2.5/src/platform/win32/agg_platform_support.cpp']
           # not sure if any more libs needed for win32 TODO: test!

        elif conf.options.platform == 'mac':
           conf.env.PLATFORM_SRC = ['agg-2.5/src/platform/mac/agg_platform_support.cpp']
           # not sure if any more libs needed for mac TODO: test!
        else:
           conf.fatal('platform not supported: '+ conf.options.platform)

def build(bld):
        bld.recurse('src')
