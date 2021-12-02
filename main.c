#include <stdio.h>
#include "ingredient.h"
#include "recipe.h"
#include <locale.h>
#include <windows.h>
#include <dirent.h>
#include "strproperty.h"
#define printHeader() printf_s("--OpenSauce--\n\n");
void showRecipes(Recipe* recipes);
Recipe* importRecipes();
void createNewIngredient(IngredientList* list);
IngredientList list;
Recipe* recipes;
int numOfRecipes = 0;
int main() {

    setlocale(LC_ALL, "swedish");
    int running = 1;
    list = loadIngredients("ingredients.txt");
    char** ingredientNames = calloc(sizeof(char*),list.numIngredients);
    for(int i = 0; i < list.numIngredients; i++){
        ingredientNames[i] = (char*)list.ingredients[i].name;
    }
    char* text = autoComplete("--OpenSauce--\n\nNamn pa ingrediens:",ingredientNames,list.numIngredients);

    Recipe* r =  importRecipes();
    while (running){
        system("cls");
        printf_s("%s",text);
        printf_s("--OpenSauce--\n\n");
        printf_s("1) Visa Recept\n");
        printf_s("2) Skapa Recept\n");
        printf_s("3) Lagg till ingrediens\n");
        printf_s("4) Skapa inkopslista\n");
        printf_s("5) Stang program\n");
        int answer;
        char enter;
        scanf("%d",&answer);

        switch(answer){
            case 1:
                showRecipes(r);
                break;
            case 2:
                system("cls");
                printf_s("Skapar Recept");
                scanf("%d",&answer);
                break;
            case 3:
                createNewIngredient(&list);
                break;
            case 4:
                system("cls");
                printf_s("Skapar inkopslista");
                scanf("%d",&answer);
                break;
            case 5:
                running = 0;
                break;
        }
    }
    //Recipe r = loadRecipeFromFile("recipes/Spaghetti_Bolognese.recipe");
    //printRecipe(r,list);
    return 0;
}


void showRecipes(Recipe* recipes){
    int running = 1;
    while(running){
        system("cls");
        printHeader();
        for(int i = 0; i < numOfRecipes; i++){
            printf_s("%d) %s\n",i+1,recipes[i].name);
        }
        printf_s("0) Ga tillbaka\n");

        int answer;
        scanf("%d",&answer);
        if(answer > 0 && answer < numOfRecipes+1 ){
            int answer1;
            do{
                system("cls");
                printHeader();
                printRecipe(recipes[answer-1],list);
                printf_s("\n0) Stang: \n");
                scanf("%d",&answer1);
            }while(answer1 != 0);
        }
        else if(answer == 0){
            running = 0;
        }
    }
}
Recipe* importRecipes(){
    recipes = calloc(30,sizeof(Recipe));
    DIR* folder;
    struct dirent* entry;
    int files = 0;
    folder = opendir("recipes/");
    if(folder == NULL)
    {
        perror("Unable to read directory");
    }
    while( (entry=readdir(folder)) )
    {
        files++;
        if(strstr(entry->d_name,".recipe") != NULL){
            char* path = calloc(9+entry->d_namlen,sizeof (char));
            strcat(path,"recipes/");
            strcat(path,entry->d_name);
            recipes[numOfRecipes] = loadRecipeFromFile(path);
            numOfRecipes++;
            free(path);
        }
    }

    closedir(folder);
    return recipes;
}
void createNewIngredient(IngredientList* list){
    int running = 1;
    while(running){
        system("cls");
        printf_s("--Add Ingredient--\nName:");
        char answer[INGREDIENT_NAME_MAX_CHARACTERS];
        do{
            scanf("%s",&answer);
            answer[INGREDIENT_NAME_MAX_CHARACTERS-1] = '\0';
        }while(strlen(answer) < 1);
        system("cls");
        printf_s("--Add Ingredient--\nStandard Unit:");
        char unitName[INGREDIENT_NAME_MAX_CHARACTERS];
        do{
            scanf("%s",&unitName);
            answer[INGREDIENT_NAME_MAX_CHARACTERS-1] = '\0';
            system("cls");
            printf_s("\nForsok igen\n");
        }while(getUnitFromName(unitName) == -1);
        system("cls");
        printf_s("Fint");

        Unit unit = getUnitFromName(unitName);

        addIngredient(list,answer,unit);
        int save = saveIngredients("ingredients.txt",*list);
        system("cls");
        printf_s("%s, %s\n",answer,unitNames[unit]);
        if(!save){
            printf_s("\nFailed to save Ingredient\n0)Create another ingredient\n1)Go back");
        }
        else{
            printf_s("Successfully created Ingredient\n0)Create another ingredient\n1)Go back");
        }
        int quit;
        scanf("%d",&quit);
        if(quit){
            running = 0;
        }
    }
}
void createNewRecipe(IngredientList list){

}

