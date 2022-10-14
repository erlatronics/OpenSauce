#include <stdio.h>
#include "ingredient.h"
#include "recipe.h"
#include <windows.h>
#include <fileapi.h>
#include "strproperty.h"
#include <conio.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h>

#define printHeader() printf_s("--OpenSauce--\n\n");

void showRecipes(Recipe* recipes);
Recipe* importRecipes();
void createNewIngredient(IngredientList* list);
void createNewRecipe(IngredientList* list);
Item * createShoppingList(Recipe* recs, int numOfRecipes, int * numOfItems);
IngredientList list;
Recipe* recipes;
int numOfRecipes = 0;
int main() {

    int running = 1;
    list = loadIngredients("ingredients.txt");
    //saveIngredients("ingredients.txt",list);
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
        scanf_s("%d",&answer);
        while ((getchar()) != '\n');
        switch(answer){
            case 1:
                showRecipes(r);
                break;
            case 2:
                system("cls");
                createNewRecipe(&list);
                free(r);
                r = importRecipes();
                break;
            case 3:
                createNewIngredient(&list);
                break;
            case 4:
                system("cls");
                int numItems = 0;
                Item* items = createShoppingList(r,numOfRecipes, &numItems);
                for(int i = 0; i < numItems; i++){
                    Ingredient in = getIngredientByID(list,items[i].id);
                    printf("* %f %s %s\n",items[i].amount,unitNames[items[i].unit], in.name);
                }
                _getch();
                free(items);
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
        scanf_s("%d",&answer);
        if(answer > 0 && answer < numOfRecipes+1 ){
            int answer1;
            do{
                system("cls");
                printHeader();
                printRecipe(recipes[answer-1],list);
                printf_s("\n0) Stäng: \n");
                scanf_s("%d",&answer1);

            }while(answer1 != 0);
        }
        else if(answer == 0){
            running = 0;
        }
    }
}
Recipe* importRecipes(){
    recipes = calloc(30,sizeof(Recipe));
    
    HANDLE entry;
    LPWIN32_FIND_DATA FindFileData;
    //struct dirent* entry;
    //folder = opendir("recipes/");
    /*if (folder == NULL)
    {
        perror("Unable to read directory");
    }*/
    entry = FindFirstFile((LPCSTR)"recipes/*.recipe", &FindFileData);
    while( entry != INVALID_HANDLE_VALUE  )
    {
        int fileNameLength = 9 + strlen(FindFileData->cFileName);
        char* path = calloc(fileNameLength,sizeof (char));
        strcat_s(path,fileNameLength, "recipes/");
        strcat_s(path, fileNameLength, FindFileData->cFileName);
        recipes[numOfRecipes] = loadRecipeFromFile(path);
        numOfRecipes++;
        free(path);
        entry = FindNextFile(entry, &FindFileData);
    }

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
        scanf_s("%d",&quit);
        while ((getchar()) != '\n');
        if(quit == 0){
            running = 0;
        }
    }
}
void createNewRecipe(IngredientList* list){
    char* name = getStrInput("---OpenSauce---\n---Recipe Maker---\n\nNamn pa nytt recept\n",3,RECIPE_NAME_MAX_CHARACTERS);
    char* fileName = getStrInput("---OpenSauce---\n---Recipe Maker---\n\nFilnamn for recept\n",3,30);
    char* fileNameWithEnding = malloc(FILENAME_MAX);
    struct stat st = { 0 };
    if (stat("recipes", &st) == -1) {
        _mkdir("recipes");
    }
    _getcwd(fileNameWithEnding, FILENAME_MAX);
    strcat_s(fileNameWithEnding, FILENAME_MAX,"\\recipes\\");
    strcat_s(fileNameWithEnding, FILENAME_MAX,fileName);
    strcat_s(fileNameWithEnding, FILENAME_MAX,".recipe");
    FILE* file;
    struct stat buffer;
    //Check if recipe already exists
    while(stat(fileNameWithEnding, &buffer) == 0){
        free(fileNameWithEnding);
        fileName = getStrInput("---OpenSauce---\n---Recipe Maker---\nFilen fanns redan, skriv nytt\n",3,30);
        fileNameWithEnding = malloc(FILENAME_MAX);
        _getcwd(fileNameWithEnding, FILENAME_MAX);
        strcat_s(fileNameWithEnding,9,"\\recipes\\");
        strcat_s(fileNameWithEnding,30,fileName);
        strcat_s(fileNameWithEnding,8,".recipe");
    }
    errno_t err;
    err = fopen_s(&file,fileNameWithEnding,"w");
    if (err == 0)
    {
        fclose(file);
    }
    
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
            scanf_s("%d",&answer);
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
                    char* amountStr;
                    char* nextToken;
                    amountStr = strtok_s(amountAnswer, " ", &nextToken);
                    char* unitStr;
                    unitStr = strtok_s(NULL, " ",&nextToken);
                    Unit unit = getUnitFromName(unitStr);
                    int amount = 0;
                    if(amountStr != NULL && unitStr != NULL){
                        amount = (int)(strtold(amountStr,&unitStr));
                    }
                    while (amount <= 0 || unit == -1){
                        amountAnswer = getStrInput("Ej korrekt ifyllt\nHur mycket? (mangd enhet)",4, 100);
                        strtok_s(amountStr,amountAnswer," ");
                        strtok_s(unitStr,NULL," ");
                        unit = getUnitFromName(unitStr);
                        if(amountStr != NULL && unitStr != NULL){
                            amount = (int)(strtold(amountStr,&unitStr));
                        }
                    }
                    Item item;
                    item.unit = unit;
                    item.amount = (float)strtof(amountStr,&unitStr);
                    item.id = list->ingredients[chosen].id;
                    addItemRecipe(&rec,item);
                    free(amountAnswer);
                    amountStr = NULL;
                    unitStr = NULL;
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
                    while((input = (char)_getch()) != '\r'){
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
                    int writeIndex = sprintf_s(strlen(description,rec.description),"%s",rec.description);

                    printf("---Description---\n\n");
                    printf("%s\n---------\nTryck ESC för att gå tillbaka",description);
                    while ((descInput = (char)_getch()) != 27){
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

//Create shopping list
Item * createShoppingList(Recipe* recs, int numOfRec, int * numOfItems){
    Item * l;
    int allocated = 10;
    int items = 0;
    l = calloc(allocated,sizeof(Item));
    for(int i = 0; i < numOfRec; i++){
        ListItem * currItem = recs[i].items;
        while (currItem != NULL){
            if(allocated == items){
                allocated = allocated + 10;
                Item * tmp = realloc(l,allocated * sizeof(Item));
                if (tmp == NULL){
                    printf("Failed to create list");
                    break;
                }
            }
            l[items] = currItem->item;
            items ++;
            currItem = currItem->nextItem;
        }
    } *numOfItems = items;
    return l;
}