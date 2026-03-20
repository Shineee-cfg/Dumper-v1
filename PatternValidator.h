#pragma once
#include <string>
#include <vector>
#include <cstdint>

/**
 * @class PatternValidator
 * @brief Busca firmas de bytes en memoria y resuelve los offsets dinámicos.
 */
class PatternValidator {
private:
    /**
     * @brief Convierte un patrón en formato string a bytes y una máscara.
     */
    static void ParsePattern(const std::string& pattern, std::vector<uint8_t>& bytes, std::string& mask);

public:
    /**
     * @brief Busca un patrón dentro de un buffer de memoria local.
     * @param buffer El bloque de memoria donde buscar.
     * @param pattern El patrón estilo IDA (ej. "8B 0D ? ? ? ? 48 8B 01").
     * @return El índice dentro del buffer donde comienza el patrón, o -1 si no se encuentra.
     */
    static intptr_t FindPattern(const std::vector<uint8_t>& buffer, const std::string& pattern);

    /**
     * @brief Calcula la dirección final a partir de una instrucción relativa (RIP-relative).
     * @param instructionAddress Dirección donde se encontró el patrón en memoria.
     * @param instructionOffset Offset dentro de la instrucción donde están los bytes relativos.
     * @param instructionSize Tamaño total de la instrucción.
     * @param relativeValue El valor de 32 bits relativo leído en el offset.
     * @return La dirección de memoria absoluta resuelta.
     */
    static uintptr_t ResolveRelativeAddress(uintptr_t instructionAddress, int instructionOffset, int instructionSize, int32_t relativeValue);
};