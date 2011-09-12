#! /usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='tree'

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx')

def build(bld):
    bld.recurse('example')
    
    if bld.cmd == 'test':
        bld.recurse('test')

from waflib.Build import BuildContext

class test_(BuildContext):
    cmd = 'test'

class example_(BuildContext):
    cmd = 'example'

