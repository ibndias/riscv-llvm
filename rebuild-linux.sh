#/bin/bash
set -e
#rm -rf build
#mkdir build
pushd build
#cmake -G Ninja -DCMAKE_BUILD_TYPE="Release"  -DLLVM_ENABLE_PROJECTS=clang -DBUILD_SHARED_LIBS=False -DLLVM_USE_SPLIT_DWARF=True  -DCMAKE_INSTALL_PREFIX="/home/derry/riscv/"  -DLLVM_OPTIMIZED_TABLEGEN=True -DLLVM_BUILD_TESTS=False -DLLVM_PARALLEL_LINK_JOBS=False -DDEFAULT_SYSROOT="/home/derry/riscv/riscv64-unknown-elf"  -DLLVM_DEFAULT_TARGET_TRIPLE="riscv64-unknown-elf"  -DLLVM_TARGETS_TO_BUILD="RISCV"  ../llvm
cmake -G Ninja -DCMAKE_BUILD_TYPE="Release"  -DLLVM_ENABLE_PROJECTS=clang -DBUILD_SHARED_LIBS=False -DLLVM_USE_SPLIT_DWARF=True  -DCMAKE_INSTALL_PREFIX="/home/derry/riscv/"  -DLLVM_OPTIMIZED_TABLEGEN=True -DLLVM_BUILD_TESTS=False -DLLVM_PARALLEL_LINK_JOBS=False -DDEFAULT_SYSROOT="/home/derry/riscv/sysroot"  -DLLVM_DEFAULT_TARGET_TRIPLE="riscv64-unknown-linux-gnu"  -DLLVM_TARGETS_TO_BUILD="RISCV"  ../llvm
cmake --build . --target install
popd