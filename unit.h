//
// Created by erlan on 2021-11-09.
//

#ifndef OPENSAUCE_UNIT_H
#define OPENSAUCE_UNIT_H
#define NUM_UNITS 14
#include <wchar.h>
typedef enum {deciliter, centiliter, milliliter, liter, tablespoon, teaspoon, cup, hectogram, kilogram, gram, ounce, pounds, pieces, krm} Unit;
static const wchar_t *unitNames[NUM_UNITS] = {L"dl",L"cl",L"ml",L"l",L"msk",L"tsk",L"cup",L"hg",L"kg",L"g",L"oz",L"lbs",L"st",L"krm"};
float convertUnit(float value , Unit currentUnit, Unit desiredUnit);
Unit getUnitFromName(wchar_t* name);
#endif //OPENSAUCE_UNIT_H
