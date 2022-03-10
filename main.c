#include <stdio.h>
#include "ingredient.h"
#include "recipe.h"
#include <windows.h>
#include <dirent.h>
#include "strproperty.h"
#include <conio.h>
#define printHeader() printf_s("--OpenSauce--\n\n");
void showRecipes(Recipe* recipes);
Recipe* importRecipes();
void createNewIngredient(IngredientList* list);
void createNewRecipe(IngredientList* list);

IngredientList list;
Recipe* recipes;
int numOfRecipes = 0;
int main() {

    int running = 1;
    list = loadIngredients("ingredients.txt");

    Recipe* r =  importRecipes();
    while (running){
        system("chcp 65001");
        system("cls");
        printf_s("--OpenSauce--\n\n");
        printf_s("1) Visa Recept\n");
        printf_s("2) Skapa Recept\n");
        printf_s("3) Lägg till ingrediens\n");
        printf_s("4) Skapa inköpslista\n");
        printf_s("5) Stäng program\n");
        int answer;
        char enter;
        scanf("%d",&answer);

        switch(answer){
            case 1:
                showRecipes(r);
                break;
            case 2:
                system("cls");
                createNewRecipe(&list);
                r = importRecipes();
                break;
            case 3:
                createNewIngredient(&list);
                break;
            case 4:
                system("cls");
                printf_s("Skapar inköpslista");
                scanf("%d",&answer);
                break;
            case 5:
                running = 0;
                break;
        }
    }
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
        printf_s("0) Gå tillbaka\n");

        int answer;
        scanf("%d",&answer);
        if(answer > 0 && answer < numOfRecipes+1 ){
            int answer1;
            do{
                system("cls");
                printHeader();
                printRecipe(recipes[answer-1],list);
                printf_s("\n0) Stäng: \n");
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
        char* ingName =getStrInput("--Lägg till ingrediens--\nName:",2,INGREDIENT_NAME_MAX_CHARACTERS);

        char* ingUnit =getStrInput("--Lägg till ingrediens--\nStandard Unit:",1,10);

        while(getUnitFromName(ingUnit) == -1){
            ingUnit =getStrInput("--Lägg till ingrediens--\nPlease Enter a valid unit\nStandard Unit:\n",1,10);
        }

        Unit unit = getUnitFromName(ingUnit);

        addIngredient(list,ingName,unit);
        int save = saveIngredients("ingredients.txt",*list);
        system("cls");
        printf_s("%s, %s\n",ingName,unitNames[unit]);
        if(!save){
            printf_s("\nFailed to save Ingredient\n1)Create another ingredient\n0)Go back");
        }
        else{
            printf_s("Successfully created Ingredient\n1)Create another ingredient\n0)Go back");
        }
        int quit;
        scanf("%d",&quit);
        if(quit == 0){
            running = 0;
        }
    }
}
void createNewRecipe(IngredientList* list){
    char* name = getStrInput("---OpenSauce---\n---Recipe Maker---\n\nNamn pa nytt recept\n",3,RECIPE_NAME_MAX_CHARACTERS);
    char* fileName = getStrInput("---OpenSauce---\n---Recipe Maker---\n\nFilnamn for recept\n",3,30);
    char* fileNameWithEnding = malloc(strlen(fileName)+16);
    strcpy(fileNameWithEnding,"recipes/");
    strcat(fileNameWithEnding,fileName);
    strcat(fileNameWithEnding,".recipe");
    while(fopen(fileNameWithEnding,"r") != NULL){
        free(fileNameWithEnding);
        fileName = getStrInput("---OpenSauce---\n---Recipe Maker---\nFilnamn finns redan, skriv nytt\n",3,30);
        fileNameWithEnding = malloc(strlen(fileName)+16);
        strcpy(fileNameWithEnding,"recipes/");
        strcat(fileNameWithEnding,fileName);
        strcat(fileNameWithEnding,".recipe");
    }
    FILE* file = fopen(fileNameWithEnding,"w");
    fclose(file);
    int finish = 0;
    if(file != NULL){
        Recipe rec = createRecipe(name);
        setRecipeDescription(&rec,"None");
        char** ingredientNames = calloc(sizeof(char*),(*list).numIngredients);
        for(int i = 0; i < (*list).numIngredients; i++){
            ingredientNames[i] = (char*)(*list).ingredients[i].name;
        }
        int answer;
        int chosen;
        while(!finish){
            system("cls");
            printRecipe(rec,(*list));
            printf_s("1)Add Ingredient\n");
            printf_s("2)Remove Ingredient\n");
            printf_s("3)Edit Description\n");
            printf_s("0)Save and Quit\n");
            scanf("%d",&answer);
            switch (answer) {
                case 1:
                    autoComplete("Lagg till Ingrediens:\n",ingredientNames,(*list).numIngredients,&chosen);

                    if(chosen == -1){
                        createNewIngredient(list);
                        free(ingredientNames);
                        ingredientNames = calloc(sizeof(char*),(*list).numIngredients);
                        for(int i = 0; i < (*list).numIngredients; i++){
                            ingredientNames[i] = (char*)(*list).ingredients[i].name;
                        }
                        autoComplete("Ny ingrediens skapad\nLagg till Ingrediens:\n",ingredientNames,(*list).numIngredients,&chosen);
                    }
                    char* amountAnswer = getStrInput("Hur mycket? (mangd enhet)",3, 100);
                    char* amountStr = strtok(amountAnswer," ");
                    char* unitStr = strtok(NULL," ");
                    Unit unit = getUnitFromName(unitStr);
                    int amount = 0;
                    if(amountStr != NULL && unitStr != NULL){
                        amount = (int)(strtold(amountStr,&unitStr));
                    }
                    while (amount <= 0 || unit == -1){
                        amountAnswer = getStrInput("Ej korrekt ifyllt\nHur mycket? (mangd enhet)",4, 100);
                        amountStr = strtok(amountAnswer," ");
                        unitStr = strtok(NULL," ");
                        unit = getUnitFromName(unitStr);
                        if(amountStr != NULL && unitStr != NULL){
                            amount = (int)(strtold(amountStr,&unitStr));
                        }
                    }
                    Item item;
                    item.unit = unit;
                    item.amount = (float)strtold(amountStr,&unitStr);
                    item.id = list->ingredients[chosen].id;
                    addItemRecipe(&rec,item);
                    break;
                case 2:
                    system("cls");
                    printf("Ta bort ingrediens\n");
                    ListItem* it = rec.items;
                    int count = 0;
                    while (it != NULL)
                    {
                        count++;
                        it = it->nextItem;
                    }
                    int sel = 0;
                    char input;
                    it = rec.items;

                    for(int i = 0; i < count; i++){
                        printf("%s %s %f %s\n",sel == i ? "xx>" : "",ingredientNames[it->item.id-1],it->item.amount,unitNames[it->item.unit]);
                        it = it->nextItem;
                    }
                    printf("%s Avbryt\n",sel == count ? "xx>" : "");
                    while((input = (char)getch()) != '\r'){
                        it = rec.items;
                        system("cls");
                        printf("Ta bort ingrediens\n");
                        if(input == '\t'){
                            sel = (sel + 1) % (count+1);
                        }
                        for(int i = 0; i < count; i++){
                            printf("%s %s %f %s\n",sel == i ? "xx>" : "",ingredientNames[it->item.id-1],it->item.amount,unitNames[it->item.unit]);
                            it = it->nextItem;
                        }
                        printf("%s Avbryt\n",sel == count ? "xx>" : "");
                    }
                    if(sel < count){
                        it = rec.items;
                        for(int i = 0; i < sel; i++){
                            it = it->nextItem;
                        }
                        removeItemRecipe(&rec,it);
                    }
                    break;
                case 3:
                    system("cls");
                    char descInput;
                    char* description = malloc(200);
                    int writeIndex = sprintf(description,"%s",rec.description);

                    printf("---Description---\n\n");
                    printf("%s\n---------\nTryck ESC för att gå tillbaka",description);
                    while ((descInput = (char)getch()) != 27){
                        system("cls");
                        if(descInput == '\r'){
                            descInput = '\n';
                        }
                        if(descInput == '\b'){
                            if(writeIndex > 0){
                                description[writeIndex] = '\0';
                                writeIndex--;
                            }
                        }else{
                            description[writeIndex] = descInput;
                            writeIndex++;
                        }
                        description[writeIndex] = '\0';
                        printf("---Description---\n\n");
                        printf("%s\n---------\nTryck ESC för att gå tillbaka",description);

                    }

                    setRecipeDescription(&rec,description);
                    free(description);
                    break;
                case 0:
                    saveRecipeToFile(fileNameWithEnding,rec);
                    finish = 1;
            }
        }
        fclose(file);
    }
}

