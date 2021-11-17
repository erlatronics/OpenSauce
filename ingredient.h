//
// Created by erlan on 2021-11-09.
//

#ifndef OPENSAUCE_INGREDIENT_H
#define OPENSAUCE_INGREDIENT_H
#define INGREDIENT_NAME_MAX_CHARACTERS 20

#include "unit.h"

//Ingredient struct holding information about a specific ingredient
typedef struct {
    char name[INGREDIENT_NAME_MAX_CHARACTERS];
    int id;
    Unit unit;
}Ingredient;

//Item struct holds information about an ingredient item.
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
void addIngredient(IngredientList* list, char* name, Unit unit);
void changeItemUnit(Item* i, Unit desiredUnit);

IngredientList loadIngredients(char* fileName);
int saveIngredients(char* fileName, IngredientList list);


#endif //OPENSAUCE_INGREDIENT_H
