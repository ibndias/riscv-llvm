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

    for (auto &MI : MBB) {
      MachineInstr *currInstrPtr = &MI;

      // outs() << "FIRST INST\n";
      const TargetInstrInfo *XII =
          MF.getSubtarget().getInstrInfo(); // target instruction info
      DebugLoc DL;
      // is this a very first block?
      if (x == 0) {
        if (MF.getName() == "main") {
          // // allocate stack for shadow stack

          // // Copy SP to T6: addi x31, x2, 0
          // MachineBasicBlock::iterator MBBI =
          //     BuildMI(MBB, MI, DL, XII->get(RISCV::ADDI), RISCV::X31)
          //         .addReg(RISCV::X2)
          //         .addImm(0);

          // // Copy Load 0x100 000 (MB) to T5 : lui x30, 0x100
          // BuildMI(MBB, MI, DL, XII->get(RISCV::LUI),
          // RISCV::X30).addImm(0x400);

          // // Decrease SP : sub x2, x2, x30
          // BuildMI(MBB, MI, DL, XII->get(RISCV::SUB), RISCV::X2)
          //     .addReg(RISCV::X2)
          //     .addReg(RISCV::X30);

          // is_instrumented = true;
        } else {
          // allocate stack on shadow : addi t6, t6, -4
          BuildMI(MBB, MI, DL, XII->get(RISCV::ADDI), RISCV::X31)
              .addReg(RISCV::X31)
              .addImm(-16);
          // save ra to shadow : sw ra, 0(t6)
          BuildMI(MBB, MI, DL, XII->get(RISCV::SD), RISCV::X1)
              .addReg(RISCV::X31)
              .addImm(0);
          // tag the shadow : st zero, 0(t6)
          BuildMI(MBB, MI, DL, XII->get(RISCV::ST), RISCV::X0)
              .addReg(RISCV::X31)
              .addImm(0);

          is_instrumented = true;
        }
      }

      if (MI.isReturn() && MF.getName() != "main") {
        // load shadow to ra : lw ra, 0(t6)
        BuildMI(MBB, MI, DL, XII->get(RISCV::LD), RISCV::X1)
            .addReg(RISCV::X31)
            .addImm(0);

        // restore stack on shadow : addi t6, t6, 4
        BuildMI(MBB, MI, DL, XII->get(RISCV::ADDI), RISCV::X31)
            .addReg(RISCV::X31)
            .addImm(16);

        is_instrumented = true;
      }

      x++;
    }
  }

    return is_instrumented;
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