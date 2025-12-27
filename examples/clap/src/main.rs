// We're going to implement the `head` command, and maybe the `tail` command too as a seperate
// subcommand. I don't know if I want to test subcommands. Probably will try to do that for the
// argument parsers that support subcommands.

use clap::{Parser, Subcommand};

#[derive(Parser)]
#[derive(Debug)]
#[command(version = "0.6.7", about = "Get first X or last X lines from a file", long_about = None)]
#[command(name = "head_tail")]
#[command(propagate_version = true)]
struct Args {
    files: Vec<String>,
    
    #[arg(short = 'c', long = "bytes")]
    bytes: Option<usize>,

    #[arg(short = 'n', long = "lines", default_value_t = 10)]
    lines: usize,

    #[arg(short = 'v', long)]
    verbose: bool,

    #[arg(short = 'q', long = "quiet")]
    quiet: bool,

    #[arg(short = 'z', long = "zero-terminated")]
    zero: bool,
}

fn main() {
    let args = Args::parse();

    println!("peepee poopoo {:?}", args);
}
