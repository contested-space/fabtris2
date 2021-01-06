project(
  'fabtris2',
  'c',
  default_options : ['c_std=gnu11']
)

# Handle mathlib portability
c_compiler = meson.get_compiler('c')
m_dep = c_compiler.find_library('m', required : false)

sources = [
  'src/fabtrimino.c',
  'src/fabtris2.c',
  'src/grid.c',
  'src/hold.c',
  'src/matrix.c',
  'src/next.c',
  'src/square.c',
]

sdldeps = [
  dependency('SDL2'),
  dependency('SDL2_image'),
]

warning_cflags = [
  '-Wall',
  '-Wextra',
  '-Wundef',
  '-Wcast-align',
  '-Wwrite-strings',
  '-Wunreachable-code',
  '-Wformat=2',
  '-Wswitch-enum',
  '-Winit-self',
  '-Wno-implicit-fallthrough',
]

supported_warning_cflags = c_compiler.get_supported_arguments(warning_cflags)
add_global_arguments(supported_warning_cflags, language : 'c')

executable(
  'fabtris2',
  sources,
  dependencies: [sdldeps, m_dep]
)
