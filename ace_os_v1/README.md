# ace_os_v1

Small bare-metal STM32F746 CMake project with lab-style targets.

## Build

```powershell
cmake -S . -B build -G "MinGW Makefiles" "-DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake"
cmake --build build
```

If you have Ninja installed, this also works:

```powershell
cmake -S . -B build -G Ninja "-DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake"
cmake --build build
```

Outputs are generated in `build`:

- `lab0.elf`, `lab0.hex`, `lab0.bin`
- `lab1.elf`, `lab1.hex`, `lab1.bin`

## ITM printf

`printf` is routed through `_write()` to ITM stimulus port 0. To see output, enable SWO/ITM in your debugger and read port 0.

Expected `lab0` output:

```text
hello from lab0 over ITM
```

## Add Another Lab

Create `lab2/main.c`, then add this line to the root `CMakeLists.txt`:

```cmake
add_ace_lab(lab2)
```
