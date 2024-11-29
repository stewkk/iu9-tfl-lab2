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
          std::int32_t height, width, seed;
          params >> height >> width >> seed;
          if (learner::BuildDfaTable(height, width, seed)) {
            std::cerr << "OK" << std::endl;
          } else {
            std::cerr << "FAILED" << std::endl;
          }
          return 0;
        } catch (const std::exception& _) {
            std::cerr << "error reading parameters from parameters.txt\n";
        }
    }

    if (argc == 4) {
        try {
          std::int32_t height = std::stoi(argv[1]);
          std::int32_t width = std::stoi(argv[2]);
          std::int32_t seed = std::stoi(argv[3]);
          if (learner::BuildDfaTable(height, width, seed)) {
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
