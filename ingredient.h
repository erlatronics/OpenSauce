//
// Created by erlan on 2021-11-09.
//

#ifndef OPENSAUCE_INGREDIENT_H
#define OPENSAUCE_INGREDIENT_H
#define INGREDIENT_NAME_MAX_CHARACTERS 20

#include "unit.h"
#include <wchar.h>

//Ingredient struct holding information about a specific item
typedef struct {
    wchar_t name[INGREDIENT_NAME_MAX_CHARACTERS];
    int id;
    Unit unit;
}Ingredient;

//Item struct holds information about an item item.
typedef struct {
    int id;
    float amount;
    Unit unit;
}Item;

typedef struct {
    Ingredient* ingredients;
    int numIngredients;
}IngredientList;

Item createItem(int id, float amount, Unit unit);
void addIngredient(IngredientList* list, wchar_t* name, Unit unit);
void changeItemUnit(Item* i, Unit desiredUnit);

IngredientList loadIngredients(wchar_t* fileName);
int saveIngredients(wchar_t* fileName, IngredientList list);
Ingredient getIngredientByID(IngredientList list, int id);

#endif //OPENSAUCE_INGREDIENT_H
