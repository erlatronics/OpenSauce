//
// Created by erlan on 2021-11-21.
//
#include "strproperty.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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