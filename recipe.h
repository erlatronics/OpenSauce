//
// Created by erlan on 2021-11-10.
//

#ifndef OPENSAUCE_RECIPE_H
#define OPENSAUCE_RECIPE_H

#define RECIPE_NAME_MAX_CHARACTERS 60

#include <stdio.h>
#include "ingredient.h"

typedef struct ListItem{
    Item item;
    struct ListItem* nextItem;
    struct ListItem* prevItem;
}ListItem;

typedef struct {
    char name[RECIPE_NAME_MAX_CHARACTERS];
    char* description;
    ListItem * items;
}Recipe;


Recipe createRecipe(char* name);
void setRecipeDescription(Recipe* rec, char* desc);
void addItemRecipe(Recipe* rec, Item ing);
void removeItemRecipe(Recipe* rec, ListItem* item);
void printRecipe(Recipe recipe, IngredientList list);
Recipe loadRecipeFromFile(char* fileName);
void saveRecipeToFile(char* fileName,Recipe recipe);

#endif //OPENSAUCE_RECIPE_H
