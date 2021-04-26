# Grrr-security

DefconRome livecoding repository. We will implement a code reuse mitigation inspired by [RAP](https://grsecurity.net/rap_faq) from grsecurity as an LLVM IR pass.

slides [here](https://docs.google.com/presentation/d/1kdVYY9b0xasQYXFR6HV4BLrNa8_wFE_MKZWeRN3MyYI/edit?usp=sharing)

## Build

Run `./install_llvm12.sh` to install the last stable version of LLVM12 if it is not already present in your system.

Fill the pass in `RAP/RAP.cpp` and build it with `LLVM_DIR=<path to llvm12> ./build.sh`. The default path for LLVM12 should be `/usr/lib/llvm-12`, obtain the path running: `llvm-config-12 --prefix`.

Compile a sample program using the `rap` pass with `LLVM_DIR=<path to llvm12> ./test.sh`.

## LLVM resources

[Writing an LLVM pass](https://llvm.org/docs/WritingAnLLVMPass.html)

[Existing LLVM passes](https://llvm.org/docs/Passes.html)

[LLVM IR Language reference](https://llvm.org/docs/LangRef.html)

## Common LLVM functions/routines

### Iterate over Functions in a Module
```c++
for (Function &F : M) {
    [...]
}
```

### Iterate over Basic Blocks in a Function
```c++
for (BasicBlock &BB: F) {
    [...]
}
```

### Iterate over Instructions in a Basic Block
```c++
for (Instruction &I: BB) {
    [...]
}
```

**NOTICE: never change the items on which you are iterating**

### Add instruction INSTR with operands OPERANDS before Instruction I or at the end of Basic Block BB

```c++
IRBuilder<> Builder(I or BB);
Value * V = Builder.CreateINSTR(OPERANDS, ...);

--> you can keep adding instructions
Value * V2 = Builder.CreateINSTR2(OPERANDS2, ...);
Value * V3 = Builder.CreateINSTR3(OPERANDS3, ...);

--> you can combine them
Value * V4 = Builder.CreateINSTR4(V2, V3);
```

### Get basic types
```c++
Type::getInt64Ty(M.getContext());
Type::getInt32Ty(M.getContext());
Type::getInt16Ty(M.getContext());
```

**Types have no signs, operations have**

### Get an existing global variable, or create a new one if it does not exist
```c++
Constant *G = M.getOrInsertGlobal(NAME, TYPE)
```

Globals are constants as they represent an address. 
To access/moddify the value use load or store operations.

```c++
IRBuilder<> Builder(BB);
Value *GlobalValue = Builder.CreateLoad(G);
Builder.CreateStore(NewValue, G);
```
