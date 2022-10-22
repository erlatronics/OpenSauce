//
// Created by erlan on 2021-11-21.
//
#include "strproperty.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

int strContains(wchar_t* string, wchar_t* substring);

float getFloatProperty(wchar_t* string, wchar_t* property){
    wchar_t stringCopy[MAX_LINE_LENGTH];
    wcscpy_s(stringCopy,MAX_LINE_LENGTH, string);
    wchar_t* valueStringStr = wcsstr(stringCopy, property);
    if(wcslen(valueStringStr) > 1){
        //Find the id
        char* intStr = wcsstr(valueStringStr, property);
        float value;
        if(intStr != NULL){
            //Add the length of the property text to find start read position
            char* intStart = intStr + wcslen(property);
            char* intEnd;
            value = wcstof(intStart, &intEnd);
        }
        else{
            wprintf_s(L"\nNot able to read float property \"%s\" for string \"%s\"\n",property,string);
            value = -1;
        }
        return value;
    }
    wprintf_s(L"\nNot able to read float property \"%s\" since string is empty",property,string);
    return -1;
}
char* getStringProperty(wchar_t* string, wchar_t* property){
    //Find the valueString
    wchar_t stringCopy[MAX_LINE_LENGTH];
    wcscpy_s(stringCopy,MAX_LINE_LENGTH, string);
    wchar_t* valueStringStr = wcsstr(stringCopy, property);
    wchar_t* valueString;
    wchar_t* nextToken;
    if(valueStringStr != NULL){
        //Find the beginning of string after the "
        char* stringStart = wcsstr(valueStringStr, "\"");
        if(stringStart != NULL){
            valueString = wcstok_s(stringStart , "\"", &nextToken);
        } else{
            valueString = "";
            printf("\nCould not find string property \"%s\" in string \"%s\"\n",property,string);
        }
    }
    else{
        valueString = "";
        printf("\nNot able to read string property \"%s\" for string \"%s\"\n",property,string);
    }
    wchar_t* valueStringAllocated = calloc(wcslen(valueString)+1,sizeof(char));
    wcsncpy_s(valueStringAllocated,wcslen(valueString) + 1, valueString, wcslen(valueString) + 1);
    return valueStringAllocated;
}
int getIntProperty(wchar_t* string, wchar_t* property){
    wchar_t stringCopy[MAX_LINE_LENGTH];
    wcscpy_s(stringCopy, MAX_LINE_LENGTH, string);
    wchar_t* valueStringStr = wcsstr(stringCopy, property);
    if(wcslen(valueStringStr) > 1){
        //Find the id
        char* intStr = wcsstr(valueStringStr, property);
        int value;
        if(intStr != NULL){
            //Add the length of the property text to find start read position
            wchar_t* idStart = intStr + wcslen(property);
            wchar_t* idEnd;
            value = wcstol(idStart,&idEnd,0);
        }
        else{
            wprintf_s("L\nNot able to read int property \"%s\" for string \"%s\"\n",property,stringCopy);
            value = -1;
        }
        return value;
    }
    wprintf_s("L\nNot able to read int property \"%s\" since string is empty",property,stringCopy);
    return -1;
}
char* autoComplete(wchar_t* prompt, wchar_t** suggestions, int numSuggestions, int* chosen){
    wint_t input;
    int length = 0;
    int selection = 0;
    int suggested = 0;
    system("cls");
    wprintf_s(L"%s\r\n>>>",prompt);
    wchar_t* text = malloc(sizeof(wchar_t)* 100);
    while((input = _getwch()) != L'\r'){
        system("cls");
        if(input == L'\b'){
            if(length > 0){
                text[length-1] = L'\0';
                length--;
            }
            wprintf_s(L"%s\r\n%s%s\n", prompt,selection == 0 ? L">>>" : L"", text);
        }
        else if(input == L'\t'){
            if(selection == suggested){
                selection = 0;
            } else {
                selection++;
            }
            wprintf_s(L"%s\r\n%s%s\n", prompt,selection == 0 ? L">>>" : L"", text);
        }
        else {
            selection = 0;
            text[length] = input;
            text[length + 1] = '\0';
            length++;
            wprintf_s(L"%s\r\n>>>%s\n", prompt, text);
        }
        if(length > 2){
            suggested = 0;
            for(int i = 0; i < numSuggestions; i++){
                if(strContains(suggestions[i],text)){
                    suggested++;
                    wprintf_s(L"%s*%s*\n",selection == suggested ? L">>>" : L"",suggestions[i]);
                }
            }
        }
    }
    if(selection == 0){
        if(chosen != NULL){
            *chosen = -1;
        }
        char* returnedText = calloc(strlen(text)+1,sizeof(wchar_t));
        wcscpy_s(returnedText,wcslen(text)+1, text);
        free(text);
        return returnedText;
    }
    else{
        int sel = 0;
        for(int i = 0; i < numSuggestions; i++){
            if(strContains(suggestions[i],text)){
                sel++;
                if(sel == selection){
                    if(chosen != NULL){
                        *chosen = i;
                    }
                    free(text);
                    return suggestions[i];
                }
            }
        }
    }
}
int strContains(wchar_t* string, wchar_t* substring){
    int stringLength = (int)wcslen(string);
    int subLength = (int)wcslen(substring);
    wchar_t* lowerString = calloc(stringLength,sizeof(wchar_t));
    wcscpy_s(lowerString, stringLength + 1, string);
    wchar_t* lowerSubString = calloc(subLength, sizeof(wchar_t));
    wcscpy_s(lowerSubString, stringLength + 1, substring);

    //for(int i = 0; i < stringLength; i++){
        _wcslwr_s(lowerString,stringLength+1);
    //}
    //for(int i = 0; i < subLength; i++){
        _wcslwr_s(lowerSubString,subLength+1);
    //}
    return (wcsstr(lowerString,lowerSubString)) != NULL;
}
char* getStrInput(wchar_t* prompt, int minChars, const int maxChars){
    system("cls");
    wchar_t input;
    //fflush(stdin);
    //while ((getchar()) != '\n');
    wprintf_s(L"%s\r\n",prompt);
    wchar_t* answer = calloc(maxChars+3,sizeof(wchar_t));
    fgetws(answer,maxChars+3,stdin);
    //while ((getchar()) != '\n');
    while (wcslen(answer) <= minChars || wcslen(answer) > maxChars+1){
        system("cls");
        wprintf_s(L"%s\n%s",prompt, strlen(answer) < maxChars ? L"String too short\r\n" : L"String too long\r\n");
        //fflush(stdin);
        fgetws(answer,maxChars+3,stdin);
        //while ((getchar()) != '\n');
    }
    //fflush(stdin);
    //while ((getchar()) != '\n');
    wchar_t* returnAnswer = calloc(strlen(answer)+1,sizeof(wchar_t));
    wcscpy_s(returnAnswer,wcslen(answer)+1, answer);
    returnAnswer[wcslen(answer)-1] = L'\0';
    free(answer);
    return returnAnswer;
}