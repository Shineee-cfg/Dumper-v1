#pragma once
#include <string>
#include <vector>
#include <utility>

/**
 * @class OffsetDumper
 * @brief Gestiona el guardado de los offsets encontrados en un archivo físico.
 */
class OffsetDumper {
private:
    std::vector<std::pair<std::string, uintptr_t>> offsets;

public:
    /**
     * @brief Añade un offset a la lista en memoria para ser dumpeado.
     * @param name Nombre de la variable u offset (ej. "dwLocalPlayer").
     * @param value El valor del offset relativo al módulo.
     */
    void AddOffset(const std::string& name, uintptr_t value);

    /**
     * @brief Genera un archivo de cabecera (.hpp) con los offsets.
     * @param filename Ruta y nombre del archivo de salida (ej. "offsets.hpp").
     * @param namespaceName Nombre del namespace contenedor.
     * @return true si el archivo se escribió correctamente.
     */
    bool DumpToHeader(const std::string& filename, const std::string& namespaceName) const;
};