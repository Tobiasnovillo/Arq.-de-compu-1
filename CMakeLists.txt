cmake_minimum_required(VERSION 3.29)
project(Arq__de_compu_1 C) # Nombre de tu proyecto general

set(CMAKE_C_STANDARD 11)

# Agrega el directorio actual para que el compilador encuentre EasyPIO.h
include_directories(.)

# Define el nombre del ejecutable final.
# Luego, lista TODOS los archivos .c que deben ser compilados y enlazados para crear este ejecutable.
add_executable(mi_programa_final
        mini_proyecto.c
        proyecto.c
)