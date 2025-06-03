# Proyecto de Secuencias de Luces

Este proyecto implementa un sistema de control de acceso con contraseña y varias secuencias de luces en lenguaje C, diseñado para ser ejecutado en una Raspberry Pi (o sistema Linux compatible). También incluye compatibilidad para macOS y una compatibilidad básica para compilar y ejecutar en Windows, aunque con funcionalidad limitada en la interacción del terminal.

## Contenido

- `proyecto.c`: El código fuente principal del proyecto.

## Cómo Compilar

El proyecto se puede compilar usando GCC (o Clang, que es el compilador por defecto en macOS, compatible con GCC flags).

**En Raspberry Pi (Linux) o macOS:**

```bash
gcc proyecto.c -o proyecto -Wall -Wextra -pedantic -std=c99
```
(En macOS, `gcc` might be an alias for Clang, which is fine).
Los flags `-Wall`, `-Wextra`, `-pedantic`, y `-std=c99` son recomendados para habilitar advertencias útiles y usar el estándar C99.

**En Windows (usando MinGW u otro GCC):**

```bash
gcc proyecto.c -o proyecto.exe
```
(O ajusta el nombre del ejecutable según tu sistema). Ten en cuenta que en Windows, las funciones de control de terminal (`<termios.h>`) no estarán activas, por lo que la entrada de contraseña no mostrará `*` y el control de velocidad/salida de secuencia no será interactivo (requerirá Enter o Ctrl+C).

## Cómo Ejecutar

Después de compilar, ejecuta el programa desde la terminal.

**En Raspberry Pi (Linux) o macOS:**

```bash
./proyecto
```

**En Windows:**

```bash
./proyecto.exe
```
(O ajusta el nombre del ejecutable).

## Uso del Programa

1.  Al iniciar, se te pedirá una contraseña de 5 dígitos. La contraseña por defecto es `12345`.
    -   En Linux y macOS, los dígitos se mostrarán como `*` al escribirlos.
    -   En Windows, los dígitos se mostrarán directamente.
    -   Tienes 3 intentos para ingresar la contraseña correcta.

2.  Si la contraseña es correcta, se mostrará un menú con las opciones de secuencias de luces.
3.  Selecciona una opción ingresando el número correspondiente y presionando Enter.

**Durante la ejecución de una secuencia (en Linux o macOS):**

-   Presiona `q` para regresar al menú principal.
-   Usa las **flechas arriba/abajo** para ajustar la velocidad de la secuencia. La velocidad se conserva entre secuencias.

**Durante la ejecución de una secuencia (en Windows):**

-   El control interactivo de velocidad y salida no está disponible. Deberás usar `Ctrl+C` en la terminal para detener la secuencia y regresar al menú (o salir del programa si estás en el menú).

## Próximos Pasos

- Implementar las secuencias de luces propias del grupo.
- Conectar el código con los pines GPIO de la Raspberry Pi para controlar luces físicas.
- Refinar la interacción del terminal si es necesario (especialmente en Windows si se busca una experiencia más interactiva).
- Abordar el TODO sobre cómo manejar el estado de las luces al salir de una secuencia (por ejemplo, apagarlas todas).