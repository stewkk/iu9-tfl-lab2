# Сборка

``` sh
cmake . -B build/ -D CMAKE_BUILD_TYPE=Debug
cmake --build build
build/bin/learner config/parameters.txt 
build/bin/learner 3 3
build/bin/unittests
```

