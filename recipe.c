//
// Created by erlan on 2021-11-10.
//
#include "recipe.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

Recipe createRecipe(char* name){
    Recipe newRecipe;
    strcpy(newRecipe.name,name);
    newRecipe.name[RECIPE_NAME_MAX_CHARACTERS-1] = '\0';
    newRecipe.ingredients = NULL;
    newRecipe.description = NULL;
    newRecipe.descriptionLength = 0;
    return newRecipe;
}

void setRecipeDescription(Recipe* rec, char* desc){
    int descLength = strlen(desc);
    if(rec->description != NULL){
        free(rec->description);
    }
    rec->description = calloc(descLength + 1,sizeof(char));
    strcpy(rec->description,desc);
    rec->descriptionLength = descLength;

}

void addIngredientRecipe(Recipe* rec, Ingredient ing){
    //Find end of ingredient list
    ListItem* curListElement = rec->ingredients;
    ListItem *newIngredient = malloc(sizeof(ListItem));
    //If list is not empty
    if(curListElement != NULL) {
        while (curListElement->nextIngredient != NULL) {
            curListElement = curListElement->nextIngredient;
        }
        //Add item to the end of the list
        newIngredient->prevIngredient = curListElement;
        curListElement->nextIngredient = newIngredient;
        newIngredient->nextIngredient = NULL;

    }
    //If list is empty
    else {
        //Add item as first item on list
        rec->ingredients = newIngredient;
        newIngredient->prevIngredient = NULL;
        newIngredient->nextIngredient = NULL;
    }
    newIngredient->ingredient = ing;
}

void removeIngredientRecipe(Recipe* rec, ListItem* item){
    if(item != NULL){
        if(item->prevIngredient == NULL) {
            if(item->nextIngredient != NULL){
                item->nextIngredient->prevIngredient = NULL;
                rec->ingredients = item->nextIngredient;
            } else{
                rec->ingredients = NULL;
            }
            free(item);
        }
        else if(item->nextIngredient == NULL) {
            item->prevIngredient->nextIngredient = NULL;
            free(item);
        }
        else{
            item->prevIngredient->nextIngredient = item->nextIngredient;
            item->nextIngredient->prevIngredient = item->prevIngredient;
            free(item);
        }
    } else{
        printf("\n!!!Failed at removing item!!!\n");
    }
}
