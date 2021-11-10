cmake_minimum_required(VERSION 3.17)
project(CookingAssistant C)

set(CMAKE_C_STANDARD 99)

add_executable(CookingAssistant main.c unit.h unit.c ingredient.h ingredient.c recipe.h recipe.c)