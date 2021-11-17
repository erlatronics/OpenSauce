//
// Created by erlan on 2021-11-10.
//

#ifndef OPENSAUCE_RECIPE_H
#define OPENSAUCE_RECIPE_H

#define RECIPE_NAME_MAX_CHARACTERS 30

#include "ingredient.h"

typedef struct ListItem{
    Item ingredient;
    struct ListItem* nextIngredient;
    struct ListItem* prevIngredient;
}ListItem;

typedef struct {
    char name[RECIPE_NAME_MAX_CHARACTERS];
    char* description;
    ListItem * ingredients;
}Recipe;


Recipe createRecipe(char* name);
void setRecipeDescription(Recipe* rec, char* desc);
void addItemRecipe(Recipe* rec, Item ing);
void removeItemRecipe(Recipe* rec, ListItem* item);


#endif //OPENSAUCE_RECIPE_H
