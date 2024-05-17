# VMLang
A custom virtual machine language + assembler as a CLI tool (vmcl)

### Build instructions
To build, `git clone https://github.com/zach-pike/VMLang`

Then, cd into the folder

Then, `cmake -B./build .`

Finally, `cmake --build ./build/ --target vmcl --config Release`

Once that finishes, check the `build` folder, the executable `vmcl` there can be used in place, or you may move it somewhere else.

### How to use

There are a few examples in `src/test/testfiles` for unit testing.

### Instruction Reference

### 0 : No Operation (`nop`)
Does absolutely nothing

No Arguments needed

### 1 : Halt (`halt`)
Halts the VM and quits execution

No Arguments needed

### 2 : Stack Push (`push`)
Adds a value to the stack

Usage: `push <Immediate | Register>`

### 3 : Stack Pop (`pop`)
Removes a value from the stack

Usage: `pop <Register | None>`

### 4 : Add (`add`)
Adds 2 values from arguments and pops the result to the stack

Usage: `add <Immediate | Register> <Immediate | Register>`

### 5 : Add Stack (`addStack`)
Takes the last 2 values off the stack and adds them, then add it to the stack

No Arguments needed

### 6 : Subtract (`sub`)
Subtracts 2 values from arguments and pops the result to the stack

Usage: `sub <Immediate | Register> <Immediate | Register>`

### 7 : Subtract Stack (`subStack`)
Takes the last 2 values off the stack and subtract them, then add it to the stack

### 8 : Dump State (`dump`)
This instructions makes the VM print the state of the stack and registers

No Arguments needed

### 9 : Move (`move`)
Moves data from one place to another

Usage: `move <Immediate | Register> <Register>`

### 10 : Jump (`jump`)
Move the instruction pointer to a memory address

Usage: `jump <Immediate(u64) | Register(u64)>`

### 11 : Store Byte (`storeByte`)
storeByte takes a memory address and stores a value at it

Usage: `storeByte <Immediate(u64) | Register(u64)> <Immediate(u8) | Register(u8)> `

### 12 : Load Byte (`loadByte`)
Loads a byte at a memory address into a reguster

`loadByte <Immediate(u64) | Register(u64)> <Register>`

### 13 : Compare Equal (`compareEq`)
Compares 2 values then pushes a 1 onto the stack if the items are equal and puts a 0 if not equal

Usage: `compareEq <Immediate | Register> <Immediate | Register>`

### 14 : Compare Equal Stack (`compareEqStack`)
Pops 2 values off the stack and compares them, if they are equal, pushes a 1, else a 0

No Arguments needed

### 15 : Jump If True (`jumpIfEq`)
Pops a value off the stack, if the value is a truthy value, jump to a memory location

Usage: `jumpIfEq <Immediate(u64) | Register(u64)>`

### 16 : Jump If False (`jumpIfNEq`)
Pops a value off the stack, if the value is a falsy value, jump to a memory location

Usage: `jumpIfNEq <Immediate(u64) | Register(u64)>`
