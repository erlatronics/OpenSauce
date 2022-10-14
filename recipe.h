//
// Created by erlan on 2021-11-10.
//

#ifndef OPENSAUCE_RECIPE_H
#define OPENSAUCE_RECIPE_H

#define RECIPE_NAME_MAX_CHARACTERS 60

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "ingredient.h"
#include "strproperty.h"

typedef struct ListItem{
    Item item;
    struct ListItem* nextItem;
    struct ListItem* prevItem;
}ListItem;

typedef struct {
    wchar_t name[RECIPE_NAME_MAX_CHARACTERS];
    wchar_t* description;
    ListItem * items;
}Recipe;


Recipe createRecipe(wchar_t* name);
void setRecipeDescription(Recipe* rec, wchar_t* desc);
void addItemRecipe(Recipe* rec, Item ing);
void removeItemRecipe(Recipe* rec, ListItem* item);
void printRecipe(Recipe recipe, IngredientList list);
Recipe loadRecipeFromFile(wchar_t* fileName);
void saveRecipeToFile(wchar_t* fileName,Recipe recipe);

#endif //OPENSAUCE_RECIPE_H
