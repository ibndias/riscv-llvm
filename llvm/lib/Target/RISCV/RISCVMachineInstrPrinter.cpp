// derry

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

using namespace llvm;

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
//TODO: INDIRECT CHECK, if it jumps or not
//TODO: Function what can we read inside
//TODO: Switch case ins become what?
bool RISCVMachineInstrPrinter::runOnMachineFunction(MachineFunction &MF) {
  int x = 0;
  int y = 0;
  unsigned int flag;
  for (auto &MBB : MF) {
    /*
    outs() << "Contents of MachineBasicBlock:\n";
    outs() << MBB << "\n";
    const BasicBlock *BB = MBB.getBasicBlock();
    outs() << "Contents of BasicBlock corresponding to MachineBasicBlock:\n";
    outs() << BB << "\n";
    */
    if(y==0 && MF.getName().str().compare("main"))
    {
      
    }
    x = 0;
    for (auto &MI : MBB) {

      MachineInstr *currInstrPtr = &MI;
      // is this a load instruction?
      /*
      if (MBB.hasAddressTaken() && x==0 && !(y == 0 && MF.getName().str().compare("main") == 0)) {
        //printf("indirect child MBB : %d %s\n",MBB.getParent()->getFunctionNumber(), MBB.getParent()->getName());
        flag = 4;//MF.getFunctionNumber();
        
        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;
        
        MachineBasicBlock::iterator MBBI =
          BuildMI(MBB, MI, DL, XII->get(RISCV::SRLI), RISCV::X0)
                .addbti(flag);
      } else if(x==0 && !(y == 0 && MF.getName().str().compare("main")==0)) {
        //printf("direct child MBB : %d %s\n",MBB.getParent()->getFunctionNumber(), MBB.getParent()->getName());
        flag = 4;//MF.getFunctionNumber();
        
        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;
        
        MachineBasicBlock::iterator MBBI =
          BuildMI(MBB, MI, DL, XII->get(RISCV::SRLI), RISCV::X0)
                .addbti(flag);        
      }
      
      if (MI.isIndirectBranch() || MI.isBranch()) {
        //printf("parent MBB : %d %s\n",MF.getFunctionNumber(), MF.getName());
        flag = 4;//MF.getFunctionNumber();
        
        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;

        MachineBasicBlock::iterator MBBI =
          BuildMI(MBB, MI, DL, XII->get(RISCV::SLLI), RISCV::X0)
                .addbti(flag);
      }
      */
      ///*
      if(MI.isCall() && MI.getOperand(0).getGlobal()->getName() == ""){

        printf("indirect jump : %d %s\n",MF.getFunctionNumber(),MF.getName());
        printf("next bb name : %d\n",MI.getOperand(0).getGlobal()->getNumOperands());
        
        flag = MF.getFunctionNumber();

        const TargetInstrInfo *XII =
            MF.getSubtarget().getInstrInfo(); // target instruction info
        DebugLoc DL;

        MachineBasicBlock::iterator MBBI =
          BuildMI(MBB, MI, DL, XII->get(RISCV::SLLI), RISCV::X0)
                .addbti(flag);
      }
      //*/
    x++;  
    }
  y++;
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