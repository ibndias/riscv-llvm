// derry

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include <iostream>
#include <utility>

using namespace llvm;
using namespace std;

#define RISCV_MACHINEINSTR_PRINTER_PASS_NAME                                   \
  "Dummy RISCV machineinstr printer pass"

namespace {

class RISCVMachineInstrPrinter : public MachineFunctionPass {
public:
  static char ID;

  RISCVMachineInstrPrinter() : MachineFunctionPass(ID) {
    initializeRISCVMachineInstrPrinterPass(*PassRegistry::getPassRegistry());
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override {
    return RISCV_MACHINEINSTR_PRINTER_PASS_NAME;
  }
};

char RISCVMachineInstrPrinter::ID = 0;
// TODO: INDIRECT CHECK, if it jumps or not
// TODO: Function what can we read inside
// TODO: Switch case ins become what?
int idx = 0;
pair<string, int> func[10];
bool RISCVMachineInstrPrinter::runOnMachineFunction(MachineFunction &MF) {
  int x = 0;
  bool is_instrumented = false;
  for (auto &MBB : MF) {
    /*
    outs() << "Contents of MachineBasicBlock:\n";
    outs() << MBB << "\n";
    const BasicBlock *BB = MBB.getBasicBlock();
    outs() << "Contents of BasicBlock corresponding to MachineBasicBlock:\n";
    outs() << BB << "\n";
    */

    outs() << "Here at: " << MF.getName() << ", ";
    outs() << "has " << MF.getFunction().arg_size() << " arguments \n";
    func[idx++] = make_pair(MF.getName().str(), MF.getFunction().arg_size());
    outs() << "Make Pair: " << func[idx - 1].first << ":"
           << func[idx - 1].second << " arguments \n";

    for (auto &B : MF.getFunction()) {

      for (auto &I : B) {
        Instruction *currInstrPtr = &I;
        // is this a call instruction?
        if (CallBase *CI = dyn_cast<CallBase>(currInstrPtr)) {
          Function *CalledF = CI->getCalledFunction();
          I.print(llvm::errs());

          /*
          outs() << "\n[IR Iterator] PairList: \n";
          for (int i = 0; i < idx; i++) {

            outs() << "\n[IR Iterator] " << func[i].first << ":"
                   << func[i].second << " \n";
          }
          */
          outs() << "\n[IR Iterator] Func Name: \n";
          for (int i = 0; i < idx; i++) {
            if (func[i].first.compare(CI->getArgOperand(0)->getName().str()) ==
                0) {
              outs() << func[i].first << ":" << func[i].second << '\n';
            }
          }
        }
      }
    }

    for (auto &MI : MBB) {

      MachineInstr *currInstrPtr = &MI;

      // is this a load instruction?
      if (0 && x == 0 && MF.getName() != "main") {
        outs() << "FIRST INST\n";
        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;
        MachineBasicBlock::iterator MBBI =
            BuildMI(MBB, MI, DL, XII->get(RISCV::SRLI), RISCV::X0)
                .addImm(2)
                .addImm(3);
        is_instrumented = true;
      }
      // if (CallInst *CI = isa<CallInst>(currInstrPtr))

      if (MI.isCall()) {
        outs() << "\nFound Call\n";
        /*
        outs() << MI.getNumOperands() << "\n";
        outs() << MI.getOperand(0).getGlobal()->getName() << "\n";
        outs() << MI.getOperand(0).getImm() << "\n";
        outs() << MI.getOperand(0).getSymbolName() << "\n";

        outs() << MI.getNumDebugOperands() << "\n";
        outs() << MI.getNumExplicitOperands() << "\n";
        outs() << MI.getNumImplicitOperands() << "\n";
        outs() << MI.getNumMemOperands() << "\n";
        */
      }

      if (0 && MI.mayStore()) {
        outs() << "Found Store\n";
      }

      if (0 && MI.isCall()) {
        outs() << "XFound Call\n";
        // outs() << MI.getNumOperands() << "\n";
        // outs() << MI.getOperand(0).getSymbolName() << "\n";

        // outs() << MI.getOperand(1) << "\n";

        // outs() << MI.getOpcode() << "\n";
        // outs() << MI.getOperand(0) << "\n";
        // outs() << MI.getOperand(0).getGlobal()->getName() << "\n";
        // outs() << MI.getOperand(0).getGlobal()->getGUID() << "\n";
        // outs() << CI->getCalledFunction->getName() << "\n";

        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;
        /*
        MachineBasicBlock::iterator MBBI = BuildMI(MBB, MI ,DL,
        XII->get(RISCV::SW), RISCV::X1) .addReg(RISCV::X31) .addImm(0);
        */
        MachineBasicBlock::iterator MBBI =
            BuildMI(MBB, MI, DL, XII->get(RISCV::SLLI), RISCV::X0)
                .addImm(2)
                .addImm(3);

        is_instrumented = true;
      }
      x++;
    }
    return is_instrumented;
  }

  return false;
}

} // end of anonymous namespace

INITIALIZE_PASS(RISCVMachineInstrPrinter, "RISCV-machineinstr-printer",
                RISCV_MACHINEINSTR_PRINTER_PASS_NAME,
                true, // is CFG only?
                true  // is analysis?
)

namespace llvm {

FunctionPass *createRISCVMachineInstrPrinterPass() {
  return new RISCVMachineInstrPrinter();
}

} // namespace llvm