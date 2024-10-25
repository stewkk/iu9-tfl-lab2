# Сборка

``` sh
conan install conanfile.txt --profile:all conan.profile --build=missing --output-folder build -s build_type=Debug
cmake . -B build/ -D CMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -D CMAKE_BUILD_TYPE=Debug
cmake --build build
build/bin/learner
```

