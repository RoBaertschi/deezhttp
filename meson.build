project('deezhttp', 'c', version: '0.1', default_options: ['warning_level=3'])

srcs = [
  'src/deezhttp.c',
  'src/header.c',
  'src/error.c',
  'src/buffer.c',
  'src/parser.c',
]

libuv_dep = dependency('libuv')
sdl2_net_dep = dependency('SDL2_net', version: '>=2.2.0')
sdl2_dep = dependency('sdl2')

if get_option('buildtype') == 'debug'
  add_project_arguments('-DDEBUG_ENABLE', language: 'c')
endif

exe = executable(
  'deezhttp',
  srcs,
  dependencies: [libuv_dep, sdl2_net_dep, sdl2_dep],
  install: true,
)

test('basic', exe)
