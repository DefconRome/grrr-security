//=============================================================================
// FILE:
//    RAP.cpp
//
// DESCRIPTION:
//    GRRR
// template inspired from: https://github.com/banach-space/llvm-tutor
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
#include "llvm/IR/IRBuilder.h"

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
// Legacy PM implementation
class LegacyRAP : public ModulePass {

  public:
    static char ID;
    LegacyRAP() : ModulePass(ID) {}

    // This method implements what the pass does
    void visitor(Module &M, Function &F) {

        if (F.getName() == "__rap_init") return;

        GlobalVariable *RapCookie = M.getNamedGlobal("__rap_cookie");
        assert(RapCookie);

        Function* getAddrRetAddr = Intrinsic::getDeclaration(&M, Intrinsic::addressofreturnaddress, {Type::getInt64PtrTy(M.getContext())});
        Function* trapFunc       = Intrinsic::getDeclaration(&M, Intrinsic::trap);

        IRBuilder<> EntryBuilder(&*F.getEntryBlock().getFirstInsertionPt());
        Value *addrOfRetAddr = EntryBuilder.CreateCall(getAddrRetAddr, {}, "addrofretaddr");
        Value *retAddr       = EntryBuilder.CreateLoad(addrOfRetAddr, "retaddr");

        Value *rapKey = EntryBuilder.CreateLoad(RapCookie);
        Value *encRet = EntryBuilder.CreateXor(retAddr, rapKey, "encret");

        std::set<ReturnInst*> returns;
        for(BasicBlock &BB: F) {
            for (Instruction &I: BB) {
                if (ReturnInst *ret = dyn_cast<ReturnInst>(&I)) {
                    returns.insert(ret);
                }
            }
        }

        BasicBlock *errBB = BasicBlock::Create(F.getContext(), "errBB", &F);
        IRBuilder<> errBuilder(errBB);
        errBuilder.CreateCall(trapFunc);
        errBuilder.CreateUnreachable();

        for (ReturnInst *ret : returns) {
            BasicBlock *retBB = ret->getParent();
            Value *retVal = ret->getReturnValue();
            ret->eraseFromParent();

            BasicBlock *okBB  = BasicBlock::Create(F.getContext(), "okBB", &F);
            IRBuilder<> okBuilder(okBB);
            okBuilder.CreateRet(retVal);

            IRBuilder<> retBuilder(retBB);
            Value *addrOfRetAddr = retBuilder.CreateCall(getAddrRetAddr, {}, "check_addrofretaddr");
            Value *newRetAddr       = retBuilder.CreateLoad(addrOfRetAddr, "check_retaddr");

            Value *rapKey = retBuilder.CreateLoad(RapCookie);
            Value *decRet = retBuilder.CreateXor(encRet, rapKey);

            Value *isOk = retBuilder.CreateICmpEQ(decRet, newRetAddr);
            retBuilder.CreateCondBr(isOk, okBB, errBB);
        }
        return;
    }

    // Main entry point - the name conveys what unit of IR this is to be run on.
    bool runOnModule(Module &M) override {

        for (Function &F : M) {
            // Skip if it is just a declaration
            if (F.isDeclaration())
                continue;

            // apply the pass to each function
            visitor(M, F);
        }
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
