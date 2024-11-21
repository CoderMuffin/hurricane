project('hurricane', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

sdl2_dep = dependency('sdl2')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

executable('hurricane',
           'main.c',
           dependencies: [sdl2_dep, m_dep])
