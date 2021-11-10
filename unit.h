//
// Created by erlan on 2021-11-09.
//

#ifndef OPENSAUCE_UNIT_H
#define OPENSAUCE_UNIT_H

typedef enum unit {liter, deciliter, centiliter, milliliter, tablespoon, teaspoon, cup, gram, hectogram, kilogram, ounce, pounds} Unit;

float convertUnit(float value , Unit currentUnit, Unit desiredUnit);

#endif //OPENSAUCE_UNIT_H
