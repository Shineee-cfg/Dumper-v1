#include "OffsetDumper.h"
#include <fstream>
#include <iostream>
#include <iomanip>

void OffsetDumper::AddOffset(const std::string& name, uintptr_t value) {
    offsets.push_back({ name, value });
}

bool OffsetDumper::DumpToHeader(const std::string& filename, const std::string& namespaceName) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "// Archivo generado automáticamente por el Dumper Dinámico\n";
    file << "#pragma once\n";
    file << "#include <cstdint>\n\n";
    file << "namespace " << namespaceName << " {\n";

    for (const auto& offset : offsets) {
        file << "    constexpr std::ptrdiff_t " << offset.first
            << " = 0x" << std::hex << std::uppercase << offset.second << ";\n";
    }

    file << "}\n";
    file.close();
    return true;
}