//=============================================================================
// FILE:
//    RAP.cpp
//
// DESCRIPTION:
//    GRRR
//
// License: MIT
//=============================================================================

#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/AliasAnalysis.h>

#include <llvm/Support/Debug.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/ADT/Statistic.h>

#include <llvm/Support/Regex.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Analysis/LoopInfo.h>

#include <llvm/Transforms/Utils/Local.h>
#include <llvm/Transforms/Scalar.h>

#include <llvm/IR/InstIterator.h>
#include <llvm/IR/DebugInfoMetadata.h>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/CFG.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace llvm;

//-----------------------------------------------------------------------------
// RAP implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace {

// This method implements what the pass does
void visitor(Function &F) {
    return;
}

// Legacy PM implementation
struct LegacyRAP : public FunctionPass {
    static char ID;
    LegacyRAP() : FunctionPass(ID) {}
    // Main entry point - the name conveys what unit of IR this is to be run on.
    bool runOnFunction(Function &F) override {
        visitor(F);
        // Does modify the input unit of IR, hence 'true'
        return true;
    }
};
} // namespace


//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
// The address of this variable is used to uniquely identify the pass. The
// actual value doesn't matter.
char LegacyRAP::ID = 0;

// This is the core interface for pass plugins. It guarantees that 'opt' will
// recognize the pass when added to the pass pipeline on the command
// line, i.e.  via '--rap'
static RegisterPass<LegacyRAP>
X("rap", "RAP Pass",
    false, // This pass does modify the CFG => false
    false // This pass is not a pure analysis pass => false
);
