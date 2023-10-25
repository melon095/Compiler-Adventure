use crate::chunk::Chunk;

pub fn disassemble_chunk(chunk: &Chunk, name: &str) -> anyhow::Result<()> {
    let mut output = String::new();

    println!("== {} ==", name);

    let mut offset = 0;
    for byte in chunk.bytecode.iter() {
        let r = byte.disassemble(chunk, offset, &mut output);

        if let Ok(new_offset) = r {
            println!("{}", output);
            output.clear();
            offset = new_offset;
        } else if let Err(err) = r {
            eprintln!("Error disassembling chunk: {}", err);
        }
    }

    Ok(())
}
