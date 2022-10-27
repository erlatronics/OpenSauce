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
#include <locale.h>
#define printHeader() wprintf_s(L"--OpenSauce--\n\n");

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
    list = loadIngredients(L"ingredients.txt");
    //saveIngredients("ingredients.txt",list);
    Recipe* r =  importRecipes();
    while (running){
        system("chcp 10000");
        _wsetlocale(LC_ALL, "sv_SE.UTF-16");
        system("cls");
        wprintf_s(L"--OpenSauce--\n\n");
        wprintf_s(L"1) Visa Recept\n");
        wprintf_s(L"2) Skapa Recept\n");
        wprintf_s(L"3) Lägg till ingrediens\n");
        wprintf_s(L"4) Skapa inköpslista\n");
        wprintf_s(L"5) Stäng program\n");
        int answer;
        wchar_t enter;
        wscanf_s(L"%d",&answer);
        while ((getwchar()) != L'\n');
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
                    printf(L"* %f %s %s\n",items[i].amount,unitNames[items[i].unit], in.name);
                }
                _getwch();
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
            wprintf_s(L"%d) %s\n",i+1,recipes[i].name);
        }
        wprintf_s(L"0) Gå tillbaka\n");

        int answer;
        wscanf_s("%d",&answer);
        if(answer > 0 && answer < numOfRecipes+1 ){
            int answer1;
            do{
                system("cls");
                printHeader();
                printRecipe(recipes[answer-1],list);
                wprintf_s(L"\n0) Stäng: \n");
                wscanf_s("%d",&answer1);

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
    WIN32_FIND_DATA FindFileData;
    wchar_t* pathToFile;
    pathToFile = _wgetcwd(NULL, FILENAME_MAX*sizeof(wchar_t));
    wchar_t* endPtr = pathToFile + wcsnlen_s(pathToFile, FILENAME_MAX);
    wcscpy_s(endPtr, wcsnlen_s(L"\\recipes\\*.recipe",FILENAME_MAX)*sizeof(wchar_t), L"\\recipes\\*.recipe");
    printf("Hello");
    entry = FindFirstFileW(pathToFile, &FindFileData);
    while( entry != INVALID_HANDLE_VALUE  && entry != NULL)
    {
        
        int fileNameLength = 9 + wcslen(FindFileData.cFileName);
        wchar_t* path = calloc(fileNameLength,sizeof (wchar_t));
        if (path != NULL) {
            wcscpy_s(path, fileNameLength, L"recipes\\");
            wcscat_s(path, fileNameLength, FindFileData.cFileName);
            recipes[numOfRecipes] = loadRecipeFromFile(path);
            numOfRecipes++;
            free(path);
        }
        else {
            wprintf_s(L"Wasn't able to allocate memory");
            return;
        }
        entry = FindNextFile(entry, &FindFileData);
    }
    //FindClose(entry);

    return recipes;
}
void createNewIngredient(IngredientList* list){
    int running = 1;
    while(running){
        wchar_t* ingName =getStrInput(L"--Lägg till ingrediens--\nName:",2,INGREDIENT_NAME_MAX_CHARACTERS);

        wchar_t* ingUnit =getStrInput(L"--Lägg till ingrediens--\nStandard Unit:",1,10);

        while(getUnitFromName(ingUnit) == -1){
            ingUnit =getStrInput(L"--Lägg till ingrediens--\nPlease Enter a valid unit\nStandard Unit:\n",1,10);
        }

        Unit unit = getUnitFromName(ingUnit);

        addIngredient(list,ingName,unit);
        int save = saveIngredients(L"ingredients.txt",*list);
        system("cls");
        wprintf_s(L"%s, %s\n",ingName,unitNames[unit]);
        if(!save){
            wprintf_s(L"\nFailed to save Ingredient\n1)Create another ingredient\n0)Go back");
        }
        else{
            wprintf_s(L"Successfully created Ingredient\n1)Create another ingredient\n0)Go back");
        }
        int quit;
        wscanf_s(L"%d",&quit);
        while ((getwchar()) != L'\n');
        if(quit == 0){
            running = 0;
        }
    }
}
void createNewRecipe(IngredientList* list){
    wchar_t* name = getStrInput(L"---OpenSauce---\n---Recipe Maker---\n\nNamn pa nytt recept\n",3,RECIPE_NAME_MAX_CHARACTERS);
    wchar_t* fileName = getStrInput(L"---OpenSauce---\n---Recipe Maker---\n\nFilnamn for recept\n",3,30);
    wchar_t* fileNameWithEnding = malloc(FILENAME_MAX);
    struct stat st = { 0 };
    if (stat("recipes", &st) == -1) {
        int success = _mkdir("recipes");
        if (!success)
            return;
    }
    _wgetcwd(fileNameWithEnding, FILENAME_MAX);
    wcscat_s(fileNameWithEnding, FILENAME_MAX,L"\\recipes\\");
    wcscat_s(fileNameWithEnding, FILENAME_MAX,fileName);
    wcscat_s(fileNameWithEnding, FILENAME_MAX,L".recipe");
    FILE* file;
    struct stat buffer;
    //Check if recipe already exists
    while(stat(fileNameWithEnding, &buffer) == 0){
        free(fileNameWithEnding);
        fileName = getStrInput(L"---OpenSauce---\n---Recipe Maker---\nFilen fanns redan, skriv nytt\n",3,30);
        fileNameWithEnding = malloc(FILENAME_MAX);
        _wgetcwd(fileNameWithEnding, FILENAME_MAX);
        wcscat_s(fileNameWithEnding,9,L"\\recipes\\");
        wcscat_s(fileNameWithEnding,30,fileName);
        wcscat_s(fileNameWithEnding,8,L".recipe");
    }
    errno_t err;
    err = _wfopen_s(&file,fileNameWithEnding,L"w");
    if (err == 0)
    {
        fclose(file);
    }
    
    int finish = 0;
    if(file != NULL){
        Recipe rec = createRecipe(name);
        setRecipeDescription(&rec,L"None");
        wchar_t** ingredientNames = calloc((*list).numIngredients, sizeof(wchar_t*));
        for(int i = 0; i < (*list).numIngredients; i++){
            ingredientNames[i] = (wchar_t*)(*list).ingredients[i].name;
        }
        int answer;
        int chosen;
        while(!finish){
            system("cls");
            printRecipe(rec,(*list));
            wprintf_s(L"1)Add Ingredient\n");
            wprintf_s(L"2)Remove Ingredient\n");
            wprintf_s(L"3)Edit Description\n");
            wprintf_s(L"0)Save and Quit\n");
            wscanf_s(L"%d",&answer);
            switch (answer) {
                case 1:
                    autoComplete(L"Lagg till Ingrediens:\n",ingredientNames,(*list).numIngredients,&chosen);

                    if(chosen == -1){
                        createNewIngredient(list);
                        free(ingredientNames);
                        ingredientNames = calloc((*list).numIngredients, sizeof(wchar_t*));
                        for(int i = 0; i < (*list).numIngredients; i++){
                            ingredientNames[i] = (wchar_t*)(*list).ingredients[i].name;
                        }
                        autoComplete(L"Ny ingrediens skapad\nLagg till Ingrediens:\n",ingredientNames,(*list).numIngredients,&chosen);
                    }
                    while ((getwchar()) != L'\n');
                    wchar_t* amountAnswer = getStrInput(L"Hur mycket? (mangd enhet)",3, 100);
                    wchar_t* amountStr;
                    wchar_t* nextToken;
                    amountStr = wcstok_s(amountAnswer, L" ", &nextToken);
                    wchar_t* unitStr;
                    unitStr = wcstok_s(NULL, L" ",&nextToken);
                    Unit unit = getUnitFromName(unitStr);
                    int amount = 0;
                    if(amountStr != NULL && unitStr != NULL){
                        amount = (int)(wcstold(amountStr,&unitStr));
                    }
                    while (amount <= 0 || unit == -1){
                        free(amountAnswer);
                        amountAnswer = getStrInput(L"Ej korrekt ifyllt\nHur mycket? (mangd enhet)",4, 100);
                        wcstok_s(amountAnswer," ",&nextToken);
                        unitStr = wcstok_s(NULL,L" ",&nextToken);
                        unit = getUnitFromName(unitStr);
                        if(amountStr != NULL && unitStr != NULL){
                            amount = (int)(strtold(amountStr,&unitStr));
                        }
                    }
                    Item item;
                    item.unit = unit;
                    item.amount = (float)wcstof(amountStr,&unitStr);
                    item.id = list->ingredients[chosen].id;
                    addItemRecipe(&rec,item);
                    amountStr = NULL;
                    unitStr = NULL;
                    free(amountAnswer);
                    break;
                case 2:
                    system("cls");
                    printf(L"Ta bort ingrediens\n");
                    ListItem* it = rec.items;
                    int count = 0;
                    while (it != NULL)
                    {
                        count++;
                        it = it->nextItem;
                    }
                    int sel = 0;
                    wchar_t input;
                    it = rec.items;

                    for(int i = 0; i < count; i++){
                        printf(L"%s %s %f %s\n",sel == i ? L"xx>" : L"",ingredientNames[it->item.id-1],it->item.amount,unitNames[it->item.unit]);
                        it = it->nextItem;
                    }
                    printf(L"%s Avbryt\n",sel == count ? L"xx>" : L"");
                    while((input = (wchar_t)_getwch()) != L'\r'){
                        it = rec.items;
                        system("cls");
                        printf("Ta bort ingrediens\n");
                        if(input == '\t'){
                            sel = (sel + 1) % (count+1);
                        }
                        for(int i = 0; i < count; i++){
                            printf(L"%s %s %f %s\n",sel == i ? L"xx>" : L"",ingredientNames[it->item.id-1],it->item.amount,unitNames[it->item.unit]);
                            it = it->nextItem;
                        }
                        printf(L"%s Avbryt\n",sel == count ? L"xx>" : L"");
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
                    int writeIndex = swprintf_s(rec.description,strlen(description,rec.description),"%s");

                    printf(L"---Description---\n\n");
                    printf(L"%s\n---------\nTryck ESC för att gå tillbaka",description);
                    while ((descInput = (wchar_t)_getwch()) != 27){
                        system("cls");
                        if(descInput == L'\r'){
                            descInput = L'\n';
                        }
                        if(descInput == L'\b'){
                            if(writeIndex > 0){
                                description[writeIndex] = L'\0';
                                writeIndex--;
                            }
                        }else{
                            description[writeIndex] = descInput;
                            writeIndex++;
                        }
                        description[writeIndex] = L'\0';
                        printf(L"---Description---\n\n");
                        printf(L"%s\n---------\nTryck ESC för att gå tillbaka",description);

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
    if (l == NULL)
        return NULL;
    for(int i = 0; i < numOfRec; i++){
        ListItem * currItem = recs[i].items;
        while (currItem != NULL){
            if(allocated == items){
                allocated = allocated + 10;
                Item * tmp = realloc(l,allocated * sizeof(Item));
                if (tmp == NULL){
                    printf(L"Failed to create list");
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