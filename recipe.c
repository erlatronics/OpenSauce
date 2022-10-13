//
// Created by erlan on 2021-11-10.
//
#include "recipe.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "strproperty.h"
Recipe createRecipe(char* name){
    Recipe newRecipe;
    strncpy_s(newRecipe.name,RECIPE_NAME_MAX_CHARACTERS,name,RECIPE_NAME_MAX_CHARACTERS);
    newRecipe.name[RECIPE_NAME_MAX_CHARACTERS-1] = '\0';
    newRecipe.items = NULL;
    newRecipe.description = NULL;
    return newRecipe;
}

void setRecipeDescription(Recipe* rec, char* desc){
    int descLength = (int)strlen(desc);
    if(rec->description != NULL){
        free(rec->description);
    }
    rec->description = calloc(descLength+1,sizeof(char));
    strncpy_s(rec->description,descLength+1,desc,descLength+1);
}

void addItemRecipe(Recipe* rec, Item item){
    //Find end of item list
    ListItem* curListElement = rec->items;
    ListItem *newIngredient = malloc(sizeof(ListItem));
    //If list is not empty
    if(curListElement != NULL) {
        while (curListElement->nextItem != NULL) {
            curListElement = curListElement->nextItem;
        }
        //Add item to the end of the list
        newIngredient->prevItem = curListElement;
        curListElement->nextItem = newIngredient;
        newIngredient->nextItem = NULL;

    }
    //If list is empty
    else {
        //Add item as first item on list
        rec->items = newIngredient;
        newIngredient->prevItem = NULL;
        newIngredient->nextItem = NULL;
    }
    newIngredient->item = item;
}

void removeItemRecipe(Recipe* rec, ListItem* item){
    if(item != NULL){
        if(item->prevItem == NULL) {
            if(item->nextItem != NULL){
                item->nextItem->prevItem = NULL;
                rec->items = item->nextItem;
            } else{
                rec->items = NULL;
            }
            free(item);
        }
        else if(item->nextItem == NULL) {
            item->prevItem->nextItem = NULL;
            free(item);
        }
        else{
            item->prevItem->nextItem = item->nextItem;
            item->nextItem->prevItem = item->prevItem;
            free(item);
        }
    } else{
        printf("\n!!!Failed at removing item!!!\n");
    }
}
void printRecipe(Recipe recipe, IngredientList list){
    printf_s("***%s***\n\nIngredienser:\n",recipe.name);
    ListItem* ing = recipe.items;
    while (ing != NULL){
        printf_s("*%.01f %s %s\n",ing->item.amount,unitNames[ing->item.unit], getIngredientByID(list,ing->item.id).name);
        ing = ing->nextItem;
    }
    printf_s("\nInstruktioner:\n%s\n",recipe.description);
}

Recipe loadRecipeFromFile(char* fileName)
{
    Recipe recipe;
    FILE* file;
    fopen_s(&file,fileName, "r");
    char* line;
    char lineBuffer[MAX_LINE_LENGTH];
    if(file != NULL){
        if((line = fgets(lineBuffer,MAX_LINE_LENGTH,file)) != NULL){
            char* name = getStringProperty(line,"recipeName:");
            int numItems = getIntProperty(line,"numIng:");
            int descLen = getIntProperty(line,"descLen:");
            recipe = createRecipe(name);
            for(int i = 0; i < numItems; i++){
                if((line = fgets(lineBuffer,MAX_LINE_LENGTH,file)) != NULL){
                    int id = getIntProperty(line,"id:");
                    float amount = getFloatProperty(line,"amount:");
                    char* unitString = getStringProperty(line,"unit:");
                    Unit unit = getUnitFromName(unitString);
                    if(unit == -1){
                        printf("Unit doesn't exist");
                    }
                    Item item = createItem(id,amount,unit);
                    addItemRecipe(&recipe,item);
                }
            }
            recipe.description = calloc(descLen,sizeof(char));
            fread(recipe.description,sizeof(char),descLen,file);
            recipe.description[descLen-1] = '\0';
        } else{
            printf_s("Failed reading first line in %s",fileName);
            recipe = createRecipe("");
        }
    }else{
        printf_s("Failed Loading Recipe %s",fileName);
        recipe = createRecipe("");
    }
    return recipe;
}
void saveRecipeToFile(char* fileName,Recipe recipe){
    FILE* file;
    fopen_s(&file,fileName, "w");
    ListItem* currentItem = recipe.items;
    int items = 0;
    while (currentItem != NULL){
        items++;
        currentItem = currentItem->nextItem;
    }
    if(file != NULL){
        fprintf_s(file,"recipeName: \"%s\"; numIng: %d; descLen: %d\n",recipe.name,items, (int)strlen(recipe.description));
    }else{
        printf("\nFile %s not found\n",fileName);
    }
    currentItem = recipe.items;
    while (currentItem != NULL){
        fprintf_s(file,"id: %d; unit: \"%s\"; amount: %f\n",currentItem->item.id,unitNames[currentItem->item.unit],currentItem->item.amount);
        currentItem = currentItem->nextItem;
    }
    fprintf_s(file,"%s",recipe.description);
    fclose(file);
}