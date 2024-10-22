project('deezhttp', 'c', version: '0.1', default_options: ['warning_level=3'])

cmake = import('cmake')

srcs = [
  'src/deezhttp.c',
  'src/header.c',
  'src/error.c'
]

libuv_subproj = cmake.subproject('libuv', cmake_options: ['-DCMAKE_BUILD_TYPE=Release'])
libuv_dep = libuv_subproj.dependency('uv')

if get_option('buildtype') == 'debug'
  add_project_arguments('-DDEBUG_ENABLE', language: 'c')
endif

exe = executable('deezhttp', srcs, dependencies: [libuv_dep], install: true)

test('basic', exe)
