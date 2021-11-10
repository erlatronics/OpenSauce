#include <stdio.h>
#include "ingredient.h"
#include "recipe.h"
int main() {
    printf("--OpenSauce--\n");
    Ingredient i = createIngredient("Yoghurt",2,deciliter);
    Ingredient j = createIngredient("Corn",300,gram);

    Recipe rec = createRecipe("Yoghurt Corn");
    addIngredientRecipe(&rec,i);
    addIngredientRecipe(&rec,j);
    setRecipeDescription(&rec,"Start by adding the corn to the yoghurt.\nThen put in bowl and enjoy!");

    printf("*%s*\n",rec.name);
    printf("%s\n",rec.description);
    printf("%s %f deciliter\n",rec.ingredients->ingredient.name,rec.ingredients->ingredient.amount);
    printf("%s %f gram",rec.ingredients->nextIngredient->ingredient.name,rec.ingredients->nextIngredient->ingredient.amount);
    return 0;
}
