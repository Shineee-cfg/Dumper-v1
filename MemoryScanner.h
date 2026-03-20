#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>

/**
 * @class MemoryScanner
 * @brief Gestiona la conexión con el proceso objetivo y la lectura de su memoria.
 */
class MemoryScanner {
private:
    HANDLE hProcess;
    DWORD processID;

public:
    MemoryScanner();
    ~MemoryScanner();

    /**
     * @brief Adjunta el escáner a un proceso en ejecución por su nombre.
     * @param processName Nombre del ejecutable (ej. "game.exe").
     * @return true si se conectó con éxito, false en caso contrario.
     */
    bool Attach(const std::wstring& processName);

    /**
     * @brief Obtiene la dirección base de un módulo específico dentro del proceso.
     * @param moduleName Nombre del módulo (ej. "client.dll").
     * @return Dirección base del módulo, o 0 si falla.
     */
    uintptr_t GetModuleBase(const std::wstring& moduleName) const;

    /**
     * @brief Lee un bloque de memoria del proceso objetivo.
     * @param address Dirección de memoria donde empezar a leer.
     * @param buffer Vector donde se almacenarán los bytes leídos.
     * @param size Cantidad de bytes a leer.
     * @return true si la lectura fue exitosa.
     */
    bool ReadMemoryBlock(uintptr_t address, std::vector<uint8_t>& buffer, size_t size) const;

    /**
     * @brief Lee un tipo de dato específico de la memoria.
     */
    template <typename T>
    T Read(uintptr_t address) const {
        T value;
        ReadProcessMemory(hProcess, (LPCVOID)address, &value, sizeof(T), nullptr);
        return value;
    }
};