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
          std::int32_t height, width;
          std::int64_t seed;
          params >> height >> width >> seed;
          if (learner::BuildDfaTable(height, width, seed)) {
            std::cerr << "OK" << std::endl;
            return 0;
          }

          std::cerr << "FAILED" << std::endl;
          return 1;
        } catch (const std::exception& _) {
            std::cerr << "error reading parameters from parameters.txt\n";
            return 1;
        }
    }

    if (argc == 4) {
        try {
          std::int32_t height = std::stoi(argv[1]);
          std::int32_t width = std::stoi(argv[2]);
          std::int64_t seed = std::stoll(argv[3]);
          if (learner::BuildDfaTable(height, width, seed)) {
            std::cerr << "OK" << std::endl;
            return 0;
          }

          std::cerr << "FAILED" << std::endl;
          return 1;
        } catch (const std::exception& e) {
          std::cerr << e.what() << std::endl;
          return 1;
        }
    }

    std::cout << "Usage:\nlearner [path to parameters.txt]\nlearner [height] [width] [seed]\n";
    return 1;
}
