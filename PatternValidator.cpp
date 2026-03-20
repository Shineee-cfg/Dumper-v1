#include "PatternValidator.h"
#include <sstream>

void PatternValidator::ParsePattern(const std::string& pattern, std::vector<uint8_t>& bytes, std::string& mask) {
    std::istringstream iss(pattern);
    std::string word;
    while (iss >> word) {
        if (word == "?" || word == "??") {
            bytes.push_back(0x00);
            mask += "?";
        }
        else {
            bytes.push_back((uint8_t)std::strtol(word.c_str(), nullptr, 16));
            mask += "x";
        }
    }
}

intptr_t PatternValidator::FindPattern(const std::vector<uint8_t>& buffer, const std::string& pattern) {
    std::vector<uint8_t> patternBytes;
    std::string mask;
    ParsePattern(pattern, patternBytes, mask);

    size_t patternLen = mask.length();
    size_t bufferSize = buffer.size();

    if (patternLen == 0 || bufferSize < patternLen) return -1;

    for (size_t i = 0; i <= bufferSize - patternLen; ++i) {
        bool found = true;
        for (size_t j = 0; j < patternLen; ++j) {
            if (mask[j] == 'x' && buffer[i + j] != patternBytes[j]) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    return -1;
}

uintptr_t PatternValidator::ResolveRelativeAddress(uintptr_t instructionAddress, int instructionOffset, int instructionSize, int32_t relativeValue) {
    // Para x64: Dirección destino = Dirección de la instrucción + Tamaño de la instrucción + Valor Relativo
    return instructionAddress + instructionSize + relativeValue;
}