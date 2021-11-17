#include <stdio.h>
#include "ingredient.h"
#include "recipe.h"
int main() {
    printf("--OpenSauce--\n");
    IngredientList inList;
    Ingredient ins[2];
    ins[0].id = 1;
    ins[0].unit = gram;
    ins[1].id = 2;
    ins[1].unit = tablespoon;

    inList.ingredients = ins;
    inList.numIngredients = 2;

    saveIngredients("ingredients.sauce",inList);

    IngredientList list = loadIngredients("ingredients.sauce");
    printf("%d and %d", list.ingredients[0].unit,list.ingredients[1].unit);
    return 0;
}
