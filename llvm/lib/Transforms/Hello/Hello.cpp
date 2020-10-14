//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <utility>
#include <iostream>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "hello"

STATISTIC(HelloCounter, "Counts number of functions greeted");

pair <string, int> func[10];
namespace {
// Hello - The first implementation, without getAnalysisUsage.
struct Hello : public FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  Hello() : FunctionPass(ID) {}
  int idx = 0;
  bool runOnFunction(Function &F) override {
    ++HelloCounter;
    errs() << "Hello: ";
    errs().write_escaped(F.getName()) << '\n';
    errs() << F.arg_size() << '\n';
    string a = F.getName().str();
    func[idx++] = make_pair(a, F.arg_size());
    for (auto &B : F) {
      // errs() << "Basic block:\n";
      // B.print(llvm::errs());

      for (auto &I : B) {
        Instruction *currInstrPtr = &I;
        // is this a call instruction?
        if (CallBase *CI = dyn_cast<CallBase>(currInstrPtr)) {
          Function *CalledF = CI->getCalledFunction();
          if (CalledF != NULL) {
            errs() << CalledF->getName() << "\n";
          } else {
            Type *t = CI->getCalledOperand()->getType();
            errs() << "Waw\n";
            Value *v = CI->getCalledOperand();
            Value *sv = v->stripPointerCasts();
            StringRef fname = sv->getName();
            errs() << fname << "\n";

            FunctionType *ft =
                cast<FunctionType>(cast<PointerType>(t)->getElementType());
          }

          errs() << "Instruction: \n";
          I.print(llvm::errs());
          errs() << "\nOperanNum: ";
          errs() << I.getNumOperands() << '\n';
          errs() << I.GetElementPtr << '\n';
          // Function * calledFunc =
          // dyn_cast<Function>(CI->getCalledFunction()->getNumOperands());
          errs() << "\nFuncname: ";
          for(int i=0;i<idx;i++) {
            if(func[i].first.compare(CI->getArgOperand(0)->getName().str()) == 0){
               errs() << func[i].first << func[i].second << '\n';
            }
          }
          /*if (CI->isIndirectCall()) {
            errs() << CI->getArgOperand(0)->getName() << '\n';
            Function *calledFunc =
                dyn_cast<Function>(CI->getArgOperand(0)->stripPointerCasts()->);
            errs() << calledFunc->getName() << '\n';
            errs() << "\n";
          }*/
          /*
          errs() << "Opcode: \n";
          errs() << I.getOpcode();
          errs() << "\n";
          */
        }
      }
    }
    return false;
  }
};
} // namespace

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass");

namespace {
// Hello2 - The second implementation with getAnalysisUsage implemented.
struct Hello2 : public FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  Hello2() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    ++HelloCounter;
    errs() << "Hello: ";
    errs().write_escaped(F.getName()) << '\n';
    return false;
  }

  // We don't modify the program, so we preserve all analyses.
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};
} // namespace

char Hello2::ID = 0;
static RegisterPass<Hello2>
    Y("hello2", "Hello World Pass (with getAnalysisUsage implemented)");
