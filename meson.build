project('hurricane', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'optimization=3'])

sdl2_dep = dependency('sdl2',
  required : true)

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
# xlib_dep = dependency('X11')

deps = [
  # xlib_dep,
  m_dep,
  sdl2_dep
]

hc_files = [
  'anim',
  'clock',
  'engine',
  'geometry',
  'input',
  'log',
  'object',
  'shared',
  'util/list',
  'util/mat',
  'util/quat',
  'util/vec',
  'renderer/SDL',
  'renderer/video',
  'renderer/console',
  # 'renderer/xlib',
  'loader/obj',
  'loader/stl'
]

hc_sources = []
foreach hc_file : hc_files
  hc_sources += 'hurricane/src/' + hc_file + '.c'
endforeach

hc_inc = include_directories('hurricane/include', is_system : true)
hc_lib = library('hurricane', hc_sources,
  dependencies : deps,
  include_directories : [hc_inc])

executable('main',
           'main.c',
           link_with : hc_lib,
           include_directories : hc_inc)
