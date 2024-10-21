project('deezhttp', 'c', version: '0.1', default_options: ['warning_level=3'])

srcs = [
  'src/deezhttp.c',
  'src/lexer.c',
  'src/header.c',
  'src/error.c'
]

exe = executable('deezhttp', srcs, install: true)

test('basic', exe)
