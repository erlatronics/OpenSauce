//
// Created by erlan on 2021-11-21.
//
#include "strproperty.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

int strContains(char* string, char* substring);

float getFloatProperty(char* string, char* property){
    char stringCopy[MAX_LINE_LENGTH];
    strcpy(stringCopy, string);
    char* valueStringStr = strstr(stringCopy, property);
    if(strlen(valueStringStr) > 1){
        //Find the id
        char* intStr = strstr(valueStringStr, property);
        float value;
        if(intStr != NULL){
            //Add the length of the property text to find start read position
            char* intStart = intStr + strlen(property);
            char* intEnd;
            value = strtof(intStart, &intEnd);
        }
        else{
            printf_s("\nNot able to read float property \"%s\" for string \"%s\"\n",property,string);
            value = -1;
        }
        return value;
    }
    printf_s("\nNot able to read float property \"%s\" since string is empty",property,string);
    return -1;
}
char* getStringProperty(char* string, char* property){
    //Find the valueString
    char stringCopy[MAX_LINE_LENGTH];
    strcpy(stringCopy, string);
    char* valueStringStr = strstr(stringCopy, property);
    char* valueString;
    if(valueStringStr != NULL){
        //Find the beginning of string after the "
        char* stringStart = strstr(valueStringStr, "\"");
        if(stringStart != NULL){
            valueString = strtok(stringStart, "\"");
        } else{
            valueString = "";
            printf("\nCould not find string property \"%s\" in string \"%s\"\n",property,string);
        }
    }
    else{
        valueString = "";
        printf("\nNot able to read string property \"%s\" for string \"%s\"\n",property,string);
    }
    char* valueStringAllocated = calloc(strlen(valueString)+1,sizeof(char));
    strncpy(valueStringAllocated,valueString, strlen(valueString)+1);
    return valueStringAllocated;
}
int getIntProperty(char* string, char* property){
    char stringCopy[MAX_LINE_LENGTH];
    strcpy(stringCopy, string);
    char* valueStringStr = strstr(stringCopy, property);
    if(strlen(valueStringStr) > 1){
        //Find the id
        char* intStr = strstr(valueStringStr, property);
        int value;
        if(intStr != NULL){
            //Add the length of the property text to find start read position
            char* idStart = intStr + strlen(property);
            char* idEnd;
            value = strtol(idStart,&idEnd,0);
        }
        else{
            printf_s("\nNot able to read int property \"%s\" for string \"%s\"\n",property,stringCopy);
            value = -1;
        }
        return value;
    }
    printf_s("\nNot able to read int property \"%s\" since string is empty",property,stringCopy);
    return -1;
}
char* autoComplete(char* prompt, char** suggestions, int numSuggestions, int* chosen){
    char input;
    int length = 0;
    int selection = 0;
    int suggested = 0;
    system("cls");
    printf_s("%s\r\n>>>",prompt);
    char* text = malloc(sizeof(char)* 100);
    while((input = (char)getch()) != '\r'){
        system("cls");
        if(input == '\b'){
            if(length > 0){
                text[length-1] = '\0';
                length--;
            }
            printf_s("%s\r\n%s%s\n", prompt,selection == 0 ? ">>>" : "", text);
        }
        else if(input == '\t'){
            if(selection == suggested){
                selection = 0;
            } else {
                selection++;
            }
            printf_s("%s\r\n%s%s\n", prompt,selection == 0 ? ">>>" : "", text);
        }
        else {
            selection = 0;
            text[length] = input;
            text[length + 1] = '\0';
            length++;
            printf_s("%s\r\n>>>%s\n", prompt, text);
        }
        if(length > 2){
            suggested = 0;
            for(int i = 0; i < numSuggestions; i++){
                if(strContains(suggestions[i],text)){
                    suggested++;
                    printf_s("%s*%s*\n",selection == suggested ? ">>>" : "",suggestions[i]);
                }
            }
        }
    }
    if(selection == 0){
        if(chosen != NULL){
            *chosen = -1;
        }
        char* returnedText = calloc(strlen(text)+1,sizeof(char));
        strcpy(returnedText,text);
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
int strContains(char* string, char* substring){
    int stringLength = (int)strlen(string);
    int subLength = (int)strlen(substring);
    char* lowerString = malloc(stringLength);
    char* lowerSubString = malloc(subLength);
    for(int i = 0; i < stringLength; i++){
        lowerString[i] = (char)tolower(string[i]);
    }
    for(int i = 0; i < subLength; i++){
        lowerSubString[i] = (char)tolower(substring[i]);
    }
    return (strstr(lowerString,lowerSubString)) != NULL;
}
char* getStrInput(char* prompt, int minChars, int maxChars){
    system("cls");
    fflush(stdin);
    printf_s("%s\r\n",prompt);
    char answer[maxChars+3];
    fgets(answer,maxChars+3,stdin);
    while (strlen(answer) <= minChars || strlen(answer) > maxChars+1){
        system("cls");
        printf_s("%s\n%s",prompt, strlen(answer) < maxChars ? "String too short\r\n" : "String too long\r\n");
        fflush(stdin);
        fgets(answer,maxChars+3,stdin);
    }
    char* returnAnswer = malloc(strlen(answer));
    strcpy(returnAnswer,answer);
    returnAnswer[strlen(answer)-1] = '\0';
    return returnAnswer;
}