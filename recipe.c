//
// Created by erlan on 2021-11-10.
//
#include "recipe.h"

Recipe createRecipe(wchar_t* name){
    Recipe newRecipe;
    strncpy_s(newRecipe.name,RECIPE_NAME_MAX_CHARACTERS*sizeof(wchar_t), name, RECIPE_NAME_MAX_CHARACTERS * sizeof(wchar_t));
    newRecipe.name[RECIPE_NAME_MAX_CHARACTERS-1] = L'\0';
    newRecipe.items = NULL;
    newRecipe.description = NULL;
    return newRecipe;
}

void setRecipeDescription(Recipe* rec, wchar_t* desc){
    int descLength = (int)wcslen(desc);
    if(rec->description != NULL){
        free(rec->description);
    }
    rec->description = calloc(descLength+1,sizeof(wchar_t));
    wcsncpy_s(rec->description,descLength+1,desc,descLength+1);
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
    printf_s(L"***%S***\n\nIngredienser:\n",recipe.name);
    ListItem* ing = recipe.items;
    while (ing != NULL){
        printf_s(L"*%.01f %S %S\n",ing->item.amount,unitNames[ing->item.unit], getIngredientByID(list,ing->item.id).name);
        ing = ing->nextItem;
    }
    printf_s(L"\nInstruktioner:\n%S\n",recipe.description);
}

Recipe loadRecipeFromFile(wchar_t* fileName)
{
    Recipe recipe;
    FILE* file;
    fopen_s(&file,fileName, "r");
    wchar_t* line;
    wchar_t lineBuffer[MAX_LINE_LENGTH];
    if(file != NULL){
        if((line = fgetws(lineBuffer,MAX_LINE_LENGTH,file)) != NULL){
            wchar_t* name = getStringProperty(line,L"recipeName:");
            int numItems = getIntProperty(line,L"numIng:");
            int descLen = getIntProperty(line,L"descLen:");
            recipe = createRecipe(name);
            for(int i = 0; i < numItems; i++){
                if((line = fgetws(lineBuffer,MAX_LINE_LENGTH,file)) != NULL){
                    int id = getIntProperty(line,L"id:");
                    float amount = getFloatProperty(line,L"amount:");
                    wchar_t* unitString = getStringProperty(line,L"unit:");
                    Unit unit = getUnitFromName(unitString);
                    if(unit == -1){
                        printf(L"Unit doesn't exist");
                    }
                    Item item = createItem(id,amount,unit);
                    addItemRecipe(&recipe,item);
                }
            }
            recipe.description = calloc(descLen,sizeof(wchar_t));
            fread(recipe.description,sizeof(wchar_t),descLen,file);
            recipe.description[descLen-1] = L'\0';
        } else{
            printf_s("Failed reading first line in %S",fileName);
            recipe = createRecipe(L"");
        } 
    }else{
        printf_s("Failed Loading Recipe %S",fileName);
        recipe = createRecipe(L"");
    }
    return recipe;
}
void saveRecipeToFile(wchar_t* fileName,Recipe recipe){
    FILE* file;
    _wfopen_s(&file,fileName, "w");
    ListItem* currentItem = recipe.items;
    int items = 0;
    while (currentItem != NULL){
        items++;
        currentItem = currentItem->nextItem;
    }
    if(file != NULL){
        fprintf_s(file,"recipeName: \"%S\"; numIng: %d; descLen: %d\n",recipe.name,items, (int)wcslen(recipe.description));
    }else{
        printf("\nFile %S not found\n",fileName);
        return;
    }
    currentItem = recipe.items;
    while (currentItem != NULL){
        fprintf_s(file,"id: %d; unit: \"%s\"; amount: %f\n",currentItem->item.id,unitNames[currentItem->item.unit],currentItem->item.amount);
        currentItem = currentItem->nextItem;
    }
    fprintf_s(file,"%S",recipe.description);
    fclose(file);
}