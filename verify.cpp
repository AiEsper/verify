#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include <iterator>
#include "xxh3.h"

uint64_t compute_xxh3(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 0;
    }
    XXH3_state_t state;
    XXH3_64bits_reset(&state);
    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        XXH3_64bits_update(&state, buffer, sizeof(buffer));
    }
    if (file.gcount() > 0) {
        XXH3_64bits_update(&state, buffer, file.gcount());
    }
    return XXH3_64bits_digest(&state);
}

void process_directory(const std::string& directory_path) {
    std::vector<std::string> filenames;
    std::ofstream file("D:\\Project\\Test\\hash.txt");

    std::map<int, std::string> hashmap;
    for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
            filenames.push_back(entry.path().string());
        }
    }
    for (const auto& filename : filenames) {
        uint64_t hash = compute_xxh3(filename);
        if (filename.find("hash.txt") != std::string::npos) {
            continue;
        }
        file << hash << "|" << filename << std::endl; 
    }
}

std::map<double, std::string> readInputFile(std::string filename) {
  std::ifstream inputFile(filename);
  std::string line;
  std::map<double, std::string> resultMap;

  while (std::getline(inputFile, line)) {
    std::istringstream ss(line);
    std::string keyString, value;
    std::getline(ss, keyString, '|');
    std::getline(ss, value);
    double key = std::stod(keyString);


    // Store the key-value pair in the map
    resultMap[key] = value;
  }

  return resultMap;
}


int main() {
    std::string directory_path = "D:\\Project\\Test";
    process_directory(directory_path);
    std::map<double, std::string> resultMap = readInputFile("D:\\Project\\Test\\hash.txt");
    for (auto const& [key, value] : resultMap) {
        std::cout << std::hex << key << " -> " << value << std::endl;
    }


    return 0;
}

bool is_file_exist(const std::string& name) { return std::ifstream(name.c_str()).good(); }