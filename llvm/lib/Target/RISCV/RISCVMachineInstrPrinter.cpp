// derry

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/IR/Function.h"
#include <queue> // std::queue

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
bool RISCVMachineInstrPrinter::runOnMachineFunction(MachineFunction &MF) {
  int x = 0;
  int y = 0;
  unsigned int flag;

  queue<int> my_queue;

  for (BasicBlock &BB : MF.getFunction())
    for (Instruction &I : BB) {
      if (auto *Call = dyn_cast<CallBase>(&I)) {
        /*
        errs() << "CallSite(args type): ";
        for (unsigned i = 0; i < Call->getNumArgOperands(); i++) {
          errs() << Call->getArgOperand(i)->getType()->getTypeID() << " ";
        }
        errs() << "\n";
        errs() << "CallSite(args num): " << Call->getNumArgOperands() << '\n';
        errs() << "CallSite(ret type): "
               << Call->getFunctionType()->getReturnType()->getTypeID() << '\n';
               */
        if (Call->isIndirectCall()) {
          // errs() << "CallSiteInd(args num): " << Call->getNumArgOperands()
          //       << '\n';
          my_queue.push(Call->getNumArgOperands());
        }
        /*else {
          errs() << "CallSiteDir(args num): " << Call->getNumArgOperands()
                 << '\n';
          my_queue.push(Call->getNumArgOperands());
        }*/
      }
    }

  for (auto &MBB : MF) {
    x = 0;
    for (auto &MI : MBB) {
      MachineInstr *currInstrPtr = &MI;
      //*
      // For Indirect Jump, each start of function, add checks
      if (y == 0 && x == 0 && (MF.getName().str().compare("main") != 0)) {
        flag = MF.getFunction().arg_size();
        // printf("mf.operandnum : %d\n",flag);

        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;

        MachineBasicBlock::iterator MBBI =
            BuildMI(MBB, MI, DL, XII->get(RISCV::SRLI), RISCV::X0).addbti(flag);
      }
      //*/

      // Bracnh Regulation
      /*
      if (MBB.hasAddressTaken() && x==0 && !(y == 0 &&
      MF.getName().str().compare("main") == 0)) {
        //printf("indirect child MBB : %d
      %s\n",MBB.getParent()->getFunctionNumber(), MBB.getParent()->getName());
        flag = MF.getFunctionNumber();

        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;

        MachineBasicBlock::iterator MBBI =
          BuildMI(MBB, MI, DL, XII->get(RISCV::SRLI), RISCV::X0)
                .addbti(flag);
      } else if(x==0 && !(y == 0 && MF.getName().str().compare("main")==0)) {
        //printf("direct child MBB : %d
      %s\n",MBB.getParent()->getFunctionNumber(), MBB.getParent()->getName());
        flag = MF.getFunctionNumber();

        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;

        MachineBasicBlock::iterator MBBI =
          BuildMI(MBB, MI, DL, XII->get(RISCV::SRLI), RISCV::X0)
                .addbti(flag);
      }

      if (MI.isIndirectBranch() || MI.isBranch()) {
        //printf("parent MBB : %d %s\n",MF.getFunctionNumber(), MF.getName());
        flag = MF.getFunctionNumber();

        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;

        MachineBasicBlock::iterator MBBI =
          BuildMI(MBB, MI, DL, XII->get(RISCV::SLLI), RISCV::X0)
                .addbti(flag);
      }
      */
      ///*
      if (MI.isCall()) {
        if(!MI.getOperand(0).isGlobal()){
          // printf("indirect jump : %d
          // %s\n",MF.getFunctionNumber(),MF.getName()); printf("next bb name
          // : %d\n",MI.getOperand(0).getGlobal()->getNumOperands());
          flag = my_queue.front(); // MF.getFunctionNumber();
          my_queue.pop();

          const TargetInstrInfo *XII =
              MF.getSubtarget().getInstrInfo(); // target instruction info
          DebugLoc DL;

          MachineBasicBlock::iterator MBBI =
              BuildMI(MBB, MI, DL, XII->get(RISCV::SLLI), RISCV::X0)
                  .addbti(flag);
        }
      }
      //*/
      x++;
    }
    y++;
  }
  return false;
} // namespace

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