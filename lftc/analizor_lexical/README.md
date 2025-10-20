# C++ Language Analyzer

A Python-based lexical analyzer for C++ source code that generates two output files: FIP (Program Internal Form) and TS (Symbol Table).

## Overview

This analyzer processes C++ source code and produces:

- **TS (Tabela Simboluri / Symbol Table)**: Contains information about identifiers (variable names) and constants (integer literals, double literals, boolean values)
- **FIP (Forma Interna a Programului / Program Internal Form)**: Contains the source code's tokens, each associated with its position in the Symbol Table (position index for identifiers and constants, -1 for other tokens)

## Input

- A `.txt` file containing your C++ source code

## Output

- `ts.txt` - Symbol Table file
- `fip.txt` - Program Internal Form file

## How to Run
```bash
python main.py
```

When prompted, enter the name of your input file (e.g., `code`, `example`, no `.txt` needed)

## Example
```bash
$ python main.py
Enter file name: example
```

The analyzer will generate in `ts.txt` and `fip.csv` in the data/ directory.
