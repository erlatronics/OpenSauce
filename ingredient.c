//
// Created by erlan on 2021-11-09.
//
#include "ingredient.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

//Create ingredient
Item createItem(int id, float amount, Unit unit){
    Item item;
    item.id = id;
    item.amount = amount;
    item.unit = unit;
    return item;
}
void addIngredient(IngredientList* list, char* name, Unit unit){
    Ingredient ingredient;
    strcpy(ingredient.name,name);
    ingredient.name[INGREDIENT_NAME_MAX_CHARACTERS-1] = '\0';
    ingredient.unit = unit;
    //TODO add id functionality
    ingredient.id = 0;
}
//Change the unit of the ingredient and convert the value simultaneously
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
    FILE* file = fopen(fileName,"r");
    IngredientList list;
    fread(&list.numIngredients,sizeof(int),1,file);
    list.ingredients = calloc(list.numIngredients,sizeof(Ingredient));
    fread(list.ingredients,sizeof(Ingredient),list.numIngredients,file);
    fclose(file);
    return list;
}

int saveIngredients(char* fileName, IngredientList list){
    FILE* file = fopen(fileName,"w");
    fwrite(&list.numIngredients,sizeof(int),1,file);
    fwrite(list.ingredients,sizeof(Ingredient), list.numIngredients,file);
    fclose(file);
    return 1;
}

