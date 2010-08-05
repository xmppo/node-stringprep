import os

def backtick(cmd):
  return os.popen(cmd).read().strip()

srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  conf.env['CXXFLAGS_ICU'] = backtick('icu-config --cxxflags').split(' ')
  conf.env['LINKFLAGS_ICU'] = backtick('icu-config --ldflags-libsonly').split(' ')

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'node-stringprep'
  obj.source = 'node-stringprep.cc'
  obj.uselib = 'ICU' 

