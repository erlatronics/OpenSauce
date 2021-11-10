//
// Created by erlan on 2021-11-09.
//
#include "unit.h"

enum unitType{volume, weight};

//Convert the volume from one volume Unit to another
float convertUnit(float value , Unit currentUnit, Unit desiredUnit){
    unsigned short unitTypeChosen;


    switch (currentUnit) {
        //Convert value of the current Unit to liters
        case liter:
            unitTypeChosen = volume;
            break;
        case deciliter:
            value = value / 10;
            unitTypeChosen = volume;
            break;
        case centiliter:
            value = value / 100;
            unitTypeChosen = volume;
            break;
        case milliliter:
            value = value / 1000;
            unitTypeChosen = volume;
            break;
        case tablespoon:
            value = value / (1000.0f/15);
            unitTypeChosen = volume;
            break;
        case teaspoon:
            value = value / 200;
            unitTypeChosen = volume;
            break;
        case cup:
            value = value / 4.22675284f;
            unitTypeChosen = volume;
            break;
        //Convert value of the current Unit to grams
        case kilogram:
            value = value * 1000;
            unitTypeChosen = weight;
            break;
        case hectogram:
            value = value * 100;
            unitTypeChosen = weight;
            break;
        case gram:
            unitTypeChosen = weight;
            break;
        case ounce:
            value = value * 28.3495231f;
            unitTypeChosen = weight;
            break;
        case pounds:
            value = value * 453.59237f;
            unitTypeChosen = weight;
            break;
        default:
            //Could not do conversion due to not having case for chosen Unit
            return -1;
    }
    //Check if the value should be converted to volume or weight
    if(unitTypeChosen == volume){
        //Convert value of liters into desired Unit
        switch (desiredUnit) {
            case liter:
                //Do nothing
                break;
            case deciliter:
                value = value * 10;
                break;
            case centiliter:
                value = value * 100;
                break;
            case milliliter:
                value = value * 1000;
                break;
            case tablespoon:
                value = value * (1000.0f/15);
                break;
            case teaspoon:
                value = value * 200;
                break;
            case cup:
                value = value * 4.22675284f;
                break;
            default:
                //Could not do conversion due to not having case for desired Unit
                return -1;
        }
        return value;
    } else if (unitTypeChosen == weight){
        //Convert value of grams into desired Unit
        switch (desiredUnit) {
            case kilogram:
                value = value / 1000;
                break;
            case hectogram:
                value = value / 100;
            case gram:
                //Do nothing
                break;
            case ounce:
                value = value / 28.3495231f;
                break;
            case pounds:
                value = value / 453.59237f;
                break;
            default:
                //Could not do conversion due to not having case for desired Unit
                return -1;
        }
        return value;
    }
}
