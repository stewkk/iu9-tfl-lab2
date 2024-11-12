#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>

#include <learner/learner.hpp>

auto main(int argc, char* argv[]) -> int {
    if (argc == 2) {
        try {
          std::ifstream params(argv[1]);
          params.exceptions(std::ifstream::failbit | std::ifstream::badbit);
          std::int32_t n, m;
          params >> n >> m;
          if (learner::BuildDfaTable(n, m)) {
            std::cerr << "OK" << std::endl;
          } else {
            std::cerr << "FAILED" << std::endl;
          }
          return 0;
        } catch (const std::exception& _) {
            std::cerr << "error reading parameters from parameters.txt\n";
        }
    }

    if (argc == 3) {
        try {
          std::int32_t n = std::stoi(argv[1]);
          std::int32_t m = std::stoi(argv[2]);
          if (learner::BuildDfaTable(n, m)) {
            std::cerr << "OK" << std::endl;
          } else {
            std::cerr << "FAILED" << std::endl;
          }
          return 0;
        } catch (const std::exception& _) {}
    }

    std::cout << "Usage:\nlearner [path to parameters.txt]\nlearner [n] \n[m]\n";
    return 0;
}
