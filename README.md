# MIPS Processor Simulator

## Overview

The MIPS Processor Simulator is a command-line application written in C++ that simulates the execution of MIPS assembly instructions. This project provides features such as instruction fetching, decoding, execution, and memory access. It is designed to help users understand how MIPS architecture works through simulation.

## Features

| Feature              | Description                                                                        |
| -------------------- | ---------------------------------------------------------------------------------- |
| Instruction Fetching | Reads instructions from memory and updates the program counter.                    |
| Instruction Decoding | Decodes the fetched instruction to determine its type and operands.                |
| Execution            | Performs arithmetic, logical, and memory operations based on decoded instructions. |
| Memory Access        | Supports load and store instructions for memory manipulation.                      |
| Register Operations  | Implements register-based calculations and data movement.                          |
| Control Flow         | Supports branch and jump instructions for control flow changes.                    |

## Installation

Follow these steps to install and run the MIPS Processor Simulator:

### Clone the Repository:

```bash
git clone https://github.com/KiritoReborn/MIPS-Processor.git
```

### Navigate to the Directory:

```bash
cd MIPS-Processor
```

### Compile the Program:

```bash
g++ MIPS_processor.cpp -o mips_sim
```

### Run the Simulator:

```bash
./mips_sim
```

## File Structure

| File                | Purpose                                                     |
| ------------------- | ----------------------------------------------------------- |
| MIPS\_processor.cpp | Main source code implementing the MIPS simulator.           |
| factorial.asm       | MIPS assembly program to compute the factorial of a number. |
| fibonacci.asm       | MIPS assembly program to compute Fibonacci numbers.         |
| instructions.txt    | Contains the binary instructions to be executed.            |

## Usage

- The simulator reads binary MIPS instructions from `instructions.txt` and executes them.
- Users can modify `factorial.asm` and `fibonacci.asm`, convert them to binary, and test their execution.

## Future Improvements

- Implement pipelining to simulate multiple instruction executions.
- Support additional MIPS instructions and optimizations.
- Enhance debugging tools for tracking register and memory values.

## Credits

| S.No. | Name                                  | Roll No.  | Email                                                                    |
| ----- | ------------------------------------- | --------- | ------------------------------------------------------------------------ |
| 1     | P. Jaya Raghunandhan Reddy            | BT2024029 | [raghunandhan.p@iiitb.ac.in](mailto\:raghunandhan.p@iiitb.ac.in)         |
| 2     | Venkata Durga Srikar Vellanki         | BT2024081 | [Srikar.Vellanki@iiitb.ac.in](mailto\:Srikar.Vellanki@iiitb.ac.in)       |
| 3     | Kommireddy Dhanush Chennakesava Reddy | BT2024169 | [Dhanush.Kommireddy@iiitb.ac.in](mailto\:Dhanush.Kommireddy@iiitb.ac.in) |

## License

This project is licensed under the MIT License. See the LICENSE file for more details.

## Results

You can add screenshots of the simulation results in this section to showcase the execution process and outputs.

## Contact

For any questions or suggestions, please contact: 

