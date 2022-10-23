//
// Created by erlan on 2021-11-09.
//
#include "ingredient.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "strproperty.h"

//Create item
Item createItem(int id, float amount, Unit unit){
    Item item;
    item.id = id;
    item.amount = amount;
    item.unit = unit;
    return item;
}

Ingredient createIngredient(int id, wchar_t* name, Unit unit){
    Ingredient ing;
    ing.id = id;
    //snprintf(ing.name,INGREDIENT_NAME_MAX_CHARACTERS,"%s",name);
    wcsncpy_s(ing.name, INGREDIENT_NAME_MAX_CHARACTERS,  name, INGREDIENT_NAME_MAX_CHARACTERS);
    ing.name[INGREDIENT_NAME_MAX_CHARACTERS-1] = L'\0';
    ing.unit = unit;
    return ing;
}

void addIngredient(IngredientList* list, wchar_t* name, Unit unit){
    Ingredient ingredient;
    wcscpy_s(ingredient.name, INGREDIENT_NAME_MAX_CHARACTERS,name);
    ingredient.name[INGREDIENT_NAME_MAX_CHARACTERS-1] = L'\0';
    ingredient.unit = unit;
    int nextAvailableIndex = 0;
    for(int i = 0; i < list->numIngredients;i++){
        if(nextAvailableIndex <= list->ingredients[i].id){
            nextAvailableIndex = list->ingredients[i].id + 1;
        }
    }
    ingredient.id = nextAvailableIndex;
    if (list->numIngredients > 0) {
        Ingredient* tmp = realloc(list->ingredients, (list->numIngredients + 1) * sizeof(Ingredient));
        if (tmp != NULL)
            list->ingredients = tmp;
        else {
            wprintf_s(L"Realloc didn't work");
        }

    }
    else
    {
        list->ingredients = malloc(sizeof(Ingredient));
    }
    list->ingredients[list->numIngredients] = ingredient;
    list->numIngredients++;
}
//Change the unit of the item and convert the value simultaneously
void changeItemUnit(Item* i, Unit desiredUnit){
    float newAmount = convertUnit(i->amount,i->unit,desiredUnit);
    if(newAmount != -1){
        i->amount = newAmount;
        i->unit = desiredUnit;
    } else{
        printf(L"\n!!!Unit conversion failed!!!\n");
    }

}
IngredientList loadIngredients(wchar_t* fileName){
    IngredientList list;
    list.numIngredients = 0;
    FILE* file;
    _wfopen_s(&file,fileName, L"r");
    if(file == NULL){
        wprintf_s(L"Not a file");
        return list;
    }
    wchar_t lineBuffer[MAX_LINE_LENGTH];
    wchar_t* line;
    while ((line = fgetws(lineBuffer,MAX_LINE_LENGTH,file)) != NULL) {
        int id = getIntProperty(line,L"id:");
        /*if (id == 0) {
            wprintf_s("ID doesn't exist");
        }*/
        wchar_t* name = getStringProperty(line,L"name:");
        if(wcslen(name) <2){
            wprintf_s(L"Name doesn't exist");
        }
        wchar_t* unitString = getStringProperty(line,L"unit:");
        Unit unit = getUnitFromName(unitString);
        if(unit == -1){
            printf(L"Unit doesn't exist");
        }

        Ingredient ing = createIngredient(id,name,unit);
        if(list.numIngredients == 0){
            list.ingredients = malloc(sizeof(Ingredient));
            if(list.ingredients == NULL){
                wprintf_s(L"\nCrap! Memory got away...\n");
            }
        } else{
            Ingredient* tempPtr = realloc(list.ingredients,sizeof(Ingredient) * (list.numIngredients+1));
            if(tempPtr == NULL){
                wprintf_s(L"\nCrap! Memory got away...\n");
            }else{
                list.ingredients = tempPtr;
            }
        }

        list.ingredients[list.numIngredients] = ing;
        list.numIngredients++;
    }
    if(list.numIngredients == 0){
        list.ingredients = malloc(sizeof(Ingredient));
        if(list.ingredients == NULL){
            wprintf_s(L"\nCrap! Memory got away...\n");
        }
    }
    fclose(file);
    return list;
}

int saveIngredients(wchar_t* fileName, IngredientList list){
    FILE* file;
    _wfopen_s(&file, fileName, L"w");
    if(file == NULL){
        return 0;
    }
    for(int i = 0; i < list.numIngredients; i++){
        int print = fwprintf(file,L"id: %d; name: \"%s\"; unit: \"%s\"\n",list.ingredients[i].id,list.ingredients[i].name,unitNames[list.ingredients[i].unit]);
        if(print == 0){
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

Ingredient getIngredientByID(IngredientList list, int id){
    for(int i = 0; i < list.numIngredients; i++) {
        if (list.ingredients[i].id == id) {
            return list.ingredients[i];
        }
    }
    Ingredient ing;
    wcscpy_s(ing.name,INGREDIENT_NAME_MAX_CHARACTERS,L"NO INGREDIENT");
    ing.id = 0;
    ing.unit = 0;
    return ing;
}
