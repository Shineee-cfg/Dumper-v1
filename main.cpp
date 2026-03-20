#include <iostream>
#include "MemoryScanner.h"
#include "PatternValidator.h"
#include "OffsetDumper.h"

int main() {
    MemoryScanner scanner;
    OffsetDumper dumper;

    std::wstring processName = L"target_application.exe";
    std::wstring moduleName = L"client.dll";

    std::wcout << L"[+] Buscando proceso: " << processName << L"...\n";

    if (!scanner.Attach(processName)) {
        std::cerr << "[-] Error: No se pudo adjuntar al proceso. Asegúrate de que esté corriendo.\n";
        return 1;
    }
    std::cout << "[+] Proceso encontrado y adjuntado.\n";

    uintptr_t baseAddress = scanner.GetModuleBase(moduleName);
    if (!baseAddress) {
        std::cerr << "[-] Error: No se encontró el módulo base.\n";
        return 1;
    }
    std::cout << "[+] Base de " << std::string(moduleName.begin(), moduleName.end())
        << " en: 0x" << std::hex << baseAddress << std::dec << "\n";

    // Configuramos el tamaño del módulo a leer (usualmente requerirías parsear los PE headers para el tamaño exacto,
    // aquí leemos un bloque estático grande por simplicidad para el escaneo inicial).
    const size_t MODULE_SIZE = 0x1000000; // 16 MB 
    std::vector<uint8_t> moduleBuffer;

    if (scanner.ReadMemoryBlock(baseAddress, moduleBuffer, MODULE_SIZE)) {

        // --- Ejemplo de actualización dinámica de un offset (ej: LocalPlayer) ---
        // Patrón hipotético: "48 8B 05 ? ? ? ? 48 85 C0 74 ? 48 8B 48 08"
        std::string localPlayerPattern = "48 8B 05 ? ? ? ?";

        intptr_t patternIndex = PatternValidator::FindPattern(moduleBuffer, localPlayerPattern);
        if (patternIndex != -1) {
            uintptr_t instructionAddress = baseAddress + patternIndex;

            // En x64, la instrucción "48 8B 05" lee un offset de 32 bits a partir del byte 3
            int32_t relativeOffset = scanner.Read<int32_t>(instructionAddress + 3);

            // Resolvemos la dirección relativa (El tamaño de la instrucción '48 8B 05 ? ? ? ?' es de 7 bytes)
            uintptr_t absoluteAddress = PatternValidator::ResolveRelativeAddress(instructionAddress, 3, 7, relativeOffset);

            // El offset final respecto a la base del módulo:
            uintptr_t finalOffset = absoluteAddress - baseAddress;

            std::cout << "[+] Offset LocalPlayer encontrado: 0x" << std::hex << finalOffset << std::dec << "\n";
            dumper.AddOffset("dwLocalPlayer", finalOffset);
        }
        else {
            std::cerr << "[-] Patrón de LocalPlayer desactualizado o no encontrado.\n";
        }

        // Puedes añadir más escaneos aquí usando la misma lógica...

    }
    else {
        std::cerr << "[-] Error al intentar leer el bloque de memoria del módulo.\n";
    }

    // Exportar los offsets
    if (dumper.DumpToHeader("cs2_offsets.hpp", "Offsets")) {
        std::cout << "[+] Archivo cs2_offsets.hpp generado correctamente.\n";
    }

    return 0;
}