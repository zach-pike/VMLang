# VMLang
A custom virtual machine language + assembler as a CLI tool (vmcl)

### Build instructions
To build, `git clone https://github.com/zach-pike/VMLang`

Then, cd into the folder

Then, `cmake -B./build .`

Finally, `cmake --build ./build/ --target vmcl --config Release`

Once that finishes, check the `build` folder, the executable `vmcl` there can be used in place, or you may move it somewhere else.

### How to use
The CLI is very good, to run the CLI help just run `vmcl` with no args or with the `--help` flag

There are a few examples in `src/test/testfiles` for unit testing.


## Argument Types

### Immediate Values
Immediate values allow you to directly use numbers as arguments to the instruction

The immediate is started with either a `u` or a `s` for `unsigned` and `signed`. That is immediately followed by the bit width (options: [8,16,32,64])

```
main:
push s32(-10)
dump
halt
```
Output below

```
------ REGS ------
A: VMVar { Null }
B: VMVar { Null }
C: VMVar { Null }
D: VMVar { Null }
IP: 19
------ Stack ------
Stack item: VMVar { s32(-10) }
```
---

### Registers
Whenever you need to store a value somewhere but you may for some reason don't wanna use the stack? Use a register

```
main:
move u16(10), %a
halt
```
This code above moves a 10 into the A register. Percent signs are used to represent a register in a argument

There are 5 registers as of now
 - A (`%a`)
 - B (`%b`)
 - C (`%c`)
 - D (`%d`)
 - Instruction Pointer (`%ip`)

## Execution Order
Instructions are exectuted linearly from top to bottom, all programs have atleast 1 label. Execution also doesn't stop when hitting a label. A label is just a marker that can be used to get its final memory location

```
main:
halt
```
Above is the most simple program you could make, the `main:` part is the label definition

## How to get memory address of label

```
main:
push $testing

pop %a
jump %a
nop
nop
nop
nop

testing:
dump
halt
```
When you see the `$`, that is a marker that tells the compiler to fill this in with a `u64` with the address of the label specified, this is useful for `jump` like instructions.

## Instruction Reference

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
