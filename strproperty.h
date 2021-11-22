//
// Created by erlan on 2021-11-21.
//

#ifndef COOKINGASSISTANT_STRPROPERTY_H
#define COOKINGASSISTANT_STRPROPERTY_H
#define MAX_LINE_LENGTH 100

float getFloatProperty(char* string, char* property);
char* getStringProperty(char* string, char* property);
int getIntProperty(char* string, char* property);

#endif //COOKINGASSISTANT_STRPROPERTY_H
