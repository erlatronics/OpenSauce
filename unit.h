//
// Created by erlan on 2021-11-09.
//

#ifndef OPENSAUCE_UNIT_H
#define OPENSAUCE_UNIT_H
#define NUM_UNITS 13
typedef enum {deciliter, centiliter, milliliter, liter, tablespoon, teaspoon, cup, hectogram, kilogram, gram, ounce, pounds, pieces} Unit;
static const char *unitNames[NUM_UNITS] = {"dl","cl","ml","l","msk","tsk","cup","hg","kg","g","oz","lbs","st"};
float convertUnit(float value , Unit currentUnit, Unit desiredUnit);
Unit getUnitFromName(char* name);
#endif //OPENSAUCE_UNIT_H
