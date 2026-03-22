========================================================================
           MANUAL DE ACTUALIZACIÓN: CS2 DUMPER DINÁMICO
========================================================================

ESTADO DE ALERTA: ¿Cómo saber si el juego se actualizó?
Si ejecutas el Dumper.exe con CS2 abierto y la consola muestra:
"[-] Patrón desactualizado o no encontrado" 
o si el Wallhack/ESP empieza a dibujar cosas en lugares incorrectos.

------------------------------------------------------------------------
FASE 1: ACTUALIZAR LAS VARIABLES GLOBALES (dwViewMatrix, dwEntityList, etc.)
------------------------------------------------------------------------
Dado que nuestro dumper usa "Pattern Scanning" para los Globals, 
debemos actualizar las firmas en el código fuente de C++.

PASO 1: Obtener los nuevos patrones.
1. Ve al repositorio oficial de a2x en GitHub (cs2-dumper).
2. Navega a la ruta exacta: src -> analysis -> offsets.rs
3. Abre el archivo y usa Ctrl+F para buscar la variable que falló 
   (Ej: "dwLocalPlayerController").
4. Copia el texto que está dentro de pattern!("...").

PASO 2: Traducir el patrón de Rust a C++ (Estilo IDA).
El patrón de a2x estará en formato continuo y con símbolos raros. 
Debes limpiarlo antes de ponerlo en tu código:
- Regla A: Separa todos los caracteres en pares usando un espacio 
  (Ej: "488B" -> "48 8B").
- Regla B: Reemplaza cualquier bloque como "${'}" o "${}" por 
  nuestros signos de interrogación separados (Ej: "? ? ? ?").

  * Ejemplo Original (Rust): "488b05${'} 4189be"
  * Ejemplo Traducido (C++): "48 8B 05 ? ? ? ? 41 89 BE"

PASO 3: Compilar y verificar.
1. Abre main.cpp en Visual Studio.
2. Reemplaza el std::string del patrón roto por tu nueva traducción.
3. Compila el programa (Ctrl + Shift + B) y ejecútalo.
4. Verifica que la consola arroje un número hexadecimal válido.

------------------------------------------------------------------------
FASE 2: ACTUALIZAR BOTONES Y NETVARS (jump, attack, m_iHealth, etc.)
------------------------------------------------------------------------
Nuestro dumper no escanea botones ni netvars por razones de 
rendimiento y arquitectura. Estos se actualizan manualmente 
consumiendo los resultados del dumper de la comunidad.

PASO 1: Obtener las nuevas direcciones estáticas.
1. Ve al repositorio oficial de a2x en GitHub (cs2-dumper).
2. Navega a la ruta de salidas: output -> cpp (o tu lenguaje de preferencia).
3. Abre el archivo correspondiente:
   - Para saltos/disparos: Abre "buttons.hpp"
   - Para vida/equipo/coordenadas: Abre "client.dll.hpp"
4. Usa Ctrl+F para buscar la variable (Ej: "attack" o "m_iHealth").
5. Copia el número hexadecimal final (Ej: 0x20628F0).

PASO 2: Actualizar el proyecto del Overlay/GUI.
1. Abre tu proyecto de la Interfaz Gráfica en Visual Studio.
2. Pega los nuevos valores hexadecimales en las variables 
   correspondientes de tu lógica.

========================================================================
¡MANTÉN LA CALMA! El 90% de las actualizaciones menores de CS2 NO 
rompen los patrones. Solo las actualizaciones masivas del motor lo harán.
========================================================================
