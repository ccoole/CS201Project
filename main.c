//This is going to be the main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct foodEntry_struct {
    int hashKey;
    int foodId;
    char *prodName;
    char *manufName;
    double calNum;
    double carbNum;
    double fatNum;
    double proteinNum;
    double serveSizeGML;
    char *unitTypeGML;
    double normServeSize;
    char *normUnitType;
} foodEntry;

foodEntry *createFood(int key, int id, char *prod, char *manuf, double calories, double carbs, double fat, double proteins, double gmlServeSize, char *gmlUnit, double regServeSize, char *regUnit){
    foodEntry *tempStruct = (foodEntry*)malloc(sizeof(foodEntry));
    tempStruct->hashKey = key;
    tempStruct->foodId = id;
    tempStruct->prodName = malloc(strlen(prod)+1);
    strcpy(tempStruct->prodName, prod);
    tempStruct->manufName = malloc(strlen(manuf)+1);
    strcpy(tempStruct->manufName, manuf);
    tempStruct->calNum = calories;
    tempStruct->carbNum = carbs;
    tempStruct->fatNum = fat;
    tempStruct->proteinNum = proteins;
    tempStruct->serveSizeGML = gmlServeSize;
    tempStruct->unitTypeGML = malloc(strlen(gmlUnit)+1);
    strcpy(tempStruct->unitTypeGML, gmlUnit);
    tempStruct->normServeSize = regServeSize;
    tempStruct->normUnitType = malloc(strlen(regUnit)+1);
    strcpy(tempStruct->normUnitType, regUnit);
    return tempStruct;
}



int main(){
    
    foodEntry *testFood = NULL;
    
    int x = 10;
    int y = 20;
    char *prod = "Pepperoni Pizza";
    char *manuf = "Dominoes";
    double cals = 120.0;
    double carbs = 30.0;
    double fats = 8.0;
    double proteins = 11.0;
    double gmlSize = 3.0;
    char *gmlUnit = "g";
    double normSize = 1.5;
    char *normUnit = "onz";
    
    testFood = createFood(x, y, prod, manuf, cals, carbs, fats, proteins, gmlSize, gmlUnit, normSize, normUnit);
    
    printf("%s", testFood->normUnitType);
    

    return 0;
}