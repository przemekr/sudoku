#!/usr/bin/env python
#encoding: utf-8
VERSION='0.0.1'
APPNAME='empty_agg'

top = '.'

from waflib import Configure, Logs

def options(opt):
        opt.load('compiler_cxx')
        opt.add_option('--platform',
              action='store',
              default='sdl2',
              help='Graphical output to use. [sdl, sdl2, x11, w32]',
              dest='platform')

def configure(conf):
        conf.load('compiler_cxx')
        conf.check_cfg(
             path='sdl2-config',
             args='--cflags --libs', package = '',
             uselib_store='platform')
        #conf.check_cc(header='ocradlib', lib='ocrad', uselib_store='ocrad')
        conf.env.PLATFORM_SRC = ['agg-2.5/src/platform/sdl2/agg_platform_support.cpp']
        conf.env.append_value('CXXFLAGS', '-std=c++11')

def build(bld):
        bld.recurse('src')
