//
// Created by erlan on 2021-11-09.
//
#include "ingredient.h"
#include "string.h"
#include "stdio.h"
//Create ingredient
Ingredient createIngredient(char* name, float amount, Unit unit){

    Ingredient ingredient;
    strcpy(ingredient.name,name);
    ingredient.name[INGREDIENT_NAME_MAX_CHARACTERS-1] = '\0';
    ingredient.amount = amount;
    ingredient.unit = unit;
    return ingredient;
}
//Change the unit of the ingredient and convert the value simultaneously
void changeIngredientUnit(Ingredient* i, Unit desiredUnit){
    float newAmount = convertUnit(i->amount,i->unit,desiredUnit);
    if(newAmount != -1){
        i->amount = newAmount;
        i->unit = desiredUnit;
    } else{
        printf("\n!!!Unit conversion failed!!!\n");
    }

}
