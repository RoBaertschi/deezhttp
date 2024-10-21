project('deezhttp', 'c', version: '0.1', default_options: ['warning_level=3'])

srcs = [
  'src/deezhttp.c',
  'src/header.c',
  'src/error.c'
]

if get_option('buildtype') == 'debug'
  add_project_arguments('-DDEBUG_ENABLE', language: 'c')
endif

exe = executable('deezhttp', srcs, install: true)

test('basic', exe)
