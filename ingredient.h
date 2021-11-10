//
// Created by erlan on 2021-11-09.
//

#ifndef OPENSAUCE_INGREDIENT_H
#define OPENSAUCE_INGREDIENT_H
#define INGREDIENT_NAME_MAX_CHARACTERS 20

#include "unit.h"

typedef struct ingredient{
    char name[INGREDIENT_NAME_MAX_CHARACTERS];
    float amount;
    Unit unit;
}Ingredient;

Ingredient createIngredient(char* name, float amount, Unit unit);
void changeIngredientUnit(Ingredient* i, Unit desiredUnit);

#endif //OPENSAUCE_INGREDIENT_H
