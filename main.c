#include <stdio.h>
#include "ingredient.h"
int main() {
    printf("Hello, World!\n");
    Ingredient i = createIngredient("Yoghurt",2,deciliter);
    changeIngredientUnit(&i,teaspoon);
    printf("%s %f",i.name,i.amount);
    return 0;
}
