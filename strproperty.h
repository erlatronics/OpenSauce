//
// Created by erlan on 2021-11-21.
//

#ifndef COOKINGASSISTANT_STRPROPERTY_H
#define COOKINGASSISTANT_STRPROPERTY_H
#define MAX_LINE_LENGTH 100
#include <wchar.h>


float getFloatProperty(wchar_t* string, wchar_t* property);
char* getStringProperty(wchar_t* string, wchar_t* property);
int getIntProperty(wchar_t* string, wchar_t* property);
char* autoComplete(wchar_t* prompt, wchar_t** suggestions, int numSuggestions, int* chosen);
char* getStrInput(wchar_t* prompt, int minChars, int maxChars);
#endif //COOKINGASSISTANT_STRPROPERTY_H
