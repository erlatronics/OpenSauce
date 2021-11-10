//
// Created by erlan on 2021-11-10.
//

#ifndef OPENSAUCE_RECIPE_H
#define OPENSAUCE_RECIPE_H

#define RECIPE_NAME_MAX_CHARACTERS 30

#include "ingredient.h"

typedef struct ingredientListElement{
    Ingredient ingredient;
    struct ingredientListElement* nextIngredient;
    struct ingredientListElement* prevIngredient;
}ListItem;

typedef struct recipe{
    char name[RECIPE_NAME_MAX_CHARACTERS];
    char* description;
    int descriptionLength;
    ListItem * ingredients;
}Recipe;


Recipe createRecipe(char* name);
void setRecipeDescription(Recipe* rec, char* desc);
void addIngredientRecipe(Recipe* rec, Ingredient ing);
void removeIngredientRecipe(Recipe* rec, ListItem* item);


#endif //OPENSAUCE_RECIPE_H
