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
          // START_MPK
          // Clear AD and WD on Tag 0
          // https://github.com/riscv/riscv-asm-manual/blob/master/riscv-asm.md#pseudoinstructions-for-accessing-control-and-status-registers
          // csrrci x0, csr, imm
          // csrrs rd, csr, x0
          // BuildMI(MBB, MI, DL, XII->get(RISCV::CSRRS), RISCV::X0)
          //     .addImm(0xC00)
          //     .addReg(RISCV::X0);

          // // csrrci x0, csr, imm
          // BuildMI(MBB, MI, DL, XII->get(RISCV::CSRRS), RISCV::X0)
          //     .addImm(0xC00)
          //     .addReg(RISCV::X0);
          // END_MPK
          // save ra to shadow : sw ra, 0(t6)
          BuildMI(MBB, MI, DL, XII->get(RISCV::SD), RISCV::X1)
              .addReg(RISCV::X31)
              .addImm(0);
          // save ra to shadow : sw ra, 0(t6)
          BuildMI(MBB, MI, DL, XII->get(RISCV::SD), RISCV::X1)
              .addReg(RISCV::X31)
              .addImm(0);

          // START_MPK
          // Set AD and WD on Tag 0
          // csrrsi x0, csr, imm
          // BuildMI(MBB, MI, DL, XII->get(RISCV::CSRRS), RISCV::X0)
          //     .addImm(0xC00)
          //     .addReg(RISCV::X0);
          // // csrrsi x0, csr, imm
          // BuildMI(MBB, MI, DL, XII->get(RISCV::CSRRS), RISCV::X0)
          //     .addImm(0xC00)
          //     .addReg(RISCV::X0);
          // END_MPK
          is_instrumented = true;
        }
      }
      // add load instruction before every memory access instructions - Jinjae
      if (MI.mayLoadOrStore()) {
        // load shadow to ra : lw ra, 0(t6)
        BuildMI(MBB, MI, DL, XII->get(RISCV::LD), RISCV::X1)
            .addReg(RISCV::X31)
            .addImm(0);
      }

      if (MI.isReturn() && MF.getName() != "main") {
        // START_MPK
        // Clear AD on Tag 0
        // csrrci x0, csr, imm
        // BuildMI(MBB, MI, DL, XII->get(RISCV::CSRRS), RISCV::X0)
        //     .addImm(0xC00)
        //     .addReg(RISCV::X0);
        // END_MPK

        // save ra to shadow : sw ra, 0(t6)
        BuildMI(MBB, MI, DL, XII->get(RISCV::SD), RISCV::X1)
            .addReg(RISCV::X31)
            .addImm(0);
        // load shadow to ra : lw ra, 0(t6)
        BuildMI(MBB, MI, DL, XII->get(RISCV::LD), RISCV::X1)
            .addReg(RISCV::X31)
            .addImm(0);

        // START_MPK
        // Set AD on Tag 0
        // csrrsi x0, csr, imm
        // BuildMI(MBB, MI, DL, XII->get(RISCV::CSRRS), RISCV::X0)
        //     .addImm(0xC00)
        //     .addReg(RISCV::X0);
        // END_MPK

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