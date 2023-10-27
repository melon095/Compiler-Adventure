use std::{
    io::{BufRead, Write},
    path::Path,
};

use clap::Parser;
use rlox::{
    chunk::{self, Chunk, OpCode},
    disassembler,
};

#[derive(Parser, Debug)]
#[command(author, version, about, long_about = None)]
struct Args {
    /// Dissasemble the Bytecode before running it.
    #[arg(short = 'd')]
    disassemble: bool,

    /// The file to run. If not specified, the REPL will be started.
    #[arg(name = "FILE", required = false)]
    file: Option<String>,
}

fn run_file(args: Args, file_path: &Path, vm: &mut rlox::vm::VM) -> anyhow::Result<()> {
    let file = std::fs::read_to_string(file_path)?;

    let mut chunk = Chunk::new();

    rlox::compiler::compile(file, &mut chunk)?;

    if args.disassemble {
        disassembler::disassemble_chunk(&chunk, "Test Chunk")?;
    }

    vm.interpret(chunk)?;

    return Ok(());
}

fn run_repl(args: Args, vm: &mut rlox::vm::VM) -> anyhow::Result<()> {
    let mut count = 0;
    let stdin = std::io::stdin();

    loop {
        print!("rlox[{}]> ", count);
        std::io::stdout().flush()?;

        let mut line = String::new();

        if stdin.read_line(&mut line)? <= 2 {
            count += 1;
            continue;
        }

        let mut chunk = Chunk::new();

        rlox::compiler::compile(line, &mut chunk)?;

        if args.disassemble {
            disassembler::disassemble_chunk(&chunk, "Test Chunk")?;
        }

        vm.interpret(chunk)?;
        count += 1;
    }
}

fn main() -> anyhow::Result<()> {
    let args = Args::parse();

    let mut vm = rlox::vm::VM::new();

    if let Some(file) = args.file.clone() {
        let path = Path::new(&file);

        run_file(args, path, &mut vm)?;
    } else {
        run_repl(args, &mut vm)?;
    }

    // let mut chunk = Chunk::new();
    // chunk.write_op(OpCode::Constant(1.2_f64), 2);
    // chunk.write_op(OpCode::Constant(2.3_f64), 2);
    // chunk.write_op(OpCode::Constant(1.2_f64), 3);

    // chunk.write_op(OpCode::Addition, 4);
    // chunk.write_op(OpCode::Negate, 5);

    // chunk.write_op(OpCode::Return, 6);

    // chunk.write_op(OpCode::Constant(3.4_f64), 1);
    // chunk.write_op(OpCode::Constant(3.4_f64), 1);
    // chunk.write_op(OpCode::Constant(1.2_f64), 2);

    // chunk.write_op(OpCode::Addition, 2);
    // // 1.2 + 3.4 = 4.6

    // chunk.write_op(OpCode::Subtraction, 3);
    // // 4.6 - 3.4 = 1.2

    // chunk.write_op(OpCode::Negate, 4);
    // // -1.2

    // chunk.write_op(OpCode::Return, 5);

    // disassembler::disassemble_chunk(&chunk, "Test Chunk")?;

    // vm.interpret(chunk)?;

    return Ok(());
}
