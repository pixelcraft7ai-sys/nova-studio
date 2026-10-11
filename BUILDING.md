# Building NOVA Studio

## Requirements
- C++17 compiler (GCC 11+, Clang 14+, MSVC 2022)
- CMake ≥ 3.21, Ninja (recommended)
- Qt 6 (≥ 6.4; Core, Widgets, Test). Versions actually exercised are listed in `docs/dependencies.md`.
- Optional: clang-format (style), cppcheck (static analysis)

### Ubuntu 24.04
```bash
sudo apt-get install -y cmake ninja-build g++ qt6-base-dev libgl1-mesa-dev
```

## Build and test
```bash
cmake --preset dev          # Debug, warnings as errors
cmake --build --preset dev
ctest --preset dev          # headless: sets QT_QPA_PLATFORM=offscreen itself
./build/dev/bin/nova-studio # run the app
```
Other presets: `ci` (Release), `asan` (AddressSanitizer + UBSan).
If Qt is not in a default location: `cmake --preset dev -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64`.

## Useful flags
`nova-studio --version | --help | --smoke-test | --config-dir <dir> | --log-level <level>`

## Code style / static analysis
```bash
find src tests \( -name '*.cpp' -o -name '*.hpp' \) | xargs clang-format --dry-run -Werror
cppcheck --enable=warning,performance,portability --error-exitcode=1 --inline-suppr -q \
  -I src/core/include src/core src/document src/render src/formats src/platform
cmake --preset dev -DNOVA_ENABLE_CLANG_TIDY=ON   # optional, needs clang-tidy
```
