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

Ingredient createIngredient(int id, char* name, Unit unit){
    Ingredient ing;
    ing.id = id;
    //snprintf(ing.name,INGREDIENT_NAME_MAX_CHARACTERS,"%s",name);
    strncpy(ing.name, name,INGREDIENT_NAME_MAX_CHARACTERS);
    ing.name[INGREDIENT_NAME_MAX_CHARACTERS-1] = '\0';
    ing.unit = unit;
    return ing;
}

void addIngredient(IngredientList* list, char* name, Unit unit){
    Ingredient ingredient;
    strcpy(ingredient.name,name);
    ingredient.name[INGREDIENT_NAME_MAX_CHARACTERS-1] = '\0';
    ingredient.unit = unit;
    int nextAvailableIndex = 0;
    for(int i = 0; i < list->numIngredients;i++){
        if(nextAvailableIndex <= list->ingredients[i].id){
            nextAvailableIndex = list->ingredients[i].id + 1;
        }
    }
    ingredient.id = nextAvailableIndex;
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
        printf("\n!!!Unit conversion failed!!!\n");
    }

}
IngredientList loadIngredients(char* fileName){
    IngredientList list;
    list.numIngredients = 0;
    FILE* file = fopen(fileName,"r");
    if(file == NULL){
        printf_s("Not a file");
        return list;
    }
    char lineBuffer[MAX_LINE_LENGTH];
    char* line;
    while ((line = fgets(lineBuffer,MAX_LINE_LENGTH,file)) != NULL) {
        int id = getIntProperty(line,"id:");
        if(id == 0){
            printf_s("ID doesn't exist");
        }
        char* name = getStringProperty(line,"name:");
        if(strlen(name) <2){
            printf_s("Name doesn't exist");
        }
        char* unitString = getStringProperty(line,"unit:");
        Unit unit = getUnitFromName(unitString);
        if(unit == -1){
            printf("Unit doesn't exist");
        }

        Ingredient ing = createIngredient(id,name,unit);
        if(list.numIngredients == 0){
            list.ingredients = malloc(sizeof(Ingredient));
        } else{
            Ingredient* tempPtr = realloc(list.ingredients,sizeof(Ingredient) * (list.numIngredients+1));
            if(tempPtr == NULL){
                printf_s("\nCrap! Memory got away...\n");
            }else{
                list.ingredients = tempPtr;
            }
        }
        list.ingredients[list.numIngredients] = ing;
        list.numIngredients++;
    }
    fclose(file);
    return list;
}

int saveIngredients(char* fileName, IngredientList list){
    FILE* file = fopen(fileName,"w");
    if(file == NULL){
        return 0;
    }
    for(int i = 0; i < list.numIngredients; i++){
        int print = fprintf(file,"id: %d; name: \"%s\"; unit: \"%s\"\n",list.ingredients[i].id,list.ingredients[i].name,unitNames[list.ingredients[i].unit]);
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
    strcpy(ing.name,"NO INGREDIENT");
    ing.id = 0;
    ing.unit = 0;
    return ing;
}
