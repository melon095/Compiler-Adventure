project('Glyph', 'cpp',
        version : '0.1',
        default_options : ['warning_level=2', 'cpp_std=c++20'],
        subproject_dir : 'ThirdParty')

add_languages('c', native : false)

cxx = meson.get_compiler('cpp')

sources = [
    'Source/Program.cc',
    'Source/AST/AST.cc',
    'Source/Visitors/IASTVisitor.cc',
    'Source/Visitors/ASTPrinterVisitor.cc',
    'Source/Lexer/Lexer.cc',
    'Source/Lexer/Token.cc',
    'Source/Parser/Parser.cc',
    'Source/Parser/Diagnostics.cc',
    'Source/Bytecode/BytecodeCompilerVisitor.cc',
    'Source/Bytecode/BytecodeCompiler.cc',
    'Source/Bytecode/BytecodeInstruction.cc',
    'Source/Bytecode/BytecodeChunk.cc',
    'Source/Runtime/Value.cc',
    'Source/Runtime/ConstantTable.cc',
]

# llvm_dep = cxx.find_library('LLVM', required: true)

magic_enum_sp = subproject('magic_enum', required : true)
abseil_sp = subproject('abseil-cpp', required : true)

executable('Glyph', sources,
           install : true,
           #   dependencies : [llvm_dep, magic_enum_dep],
           dependencies : [
               magic_enum_sp.get_variable('magic_enum_dep'),
               abseil_sp.get_variable('absl_status_dep'),
               # Required to manually link with Synchronization library :(
               abseil_sp.get_variable('absl_synchronization_dep')
           ],
           link_with : [
               abseil_sp.get_variable('absl_status_lib'),
               abseil_sp.get_variable('absl_synchronization_lib')
           ],
           include_directories : include_directories('Source'),
           cpp_args : [
               '-Wno-gnu-statement-expression',
           ]
)

