project('Glyph', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'],
  subproject_dir: 'ThirdParty')

add_languages('c', native: false)

cxx = meson.get_compiler('cpp')

sources = [
    'Source/Program.cc',
    'Source/AST/AST.cc',
]

# llvm_dep = cxx.find_library('LLVM', required: true)

magic_enum_sp = subproject('magic_enum', required: true)
magic_enum_dep = magic_enum_sp.get_variable('magic_enum_dep')

executable('Glyph', sources,
  install : true,
#   dependencies : [llvm_dep, magic_enum_dep],
  dependencies : [magic_enum_dep],
  include_directories : include_directories('Source'),
   cpp_args: [
    '-Wno-gnu-statement-expression'
    ]
)

