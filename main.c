//This is going to be the main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct foodEntry_struct {
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

foodEntry* CreateFood(int id, char *prod, char *manuf, double calories, double carbs, double fat, double proteins, double gmlServeSize, char *gmlUnit, double regServeSize, char *regUnit){
    foodEntry *tempStruct = (foodEntry*)malloc(sizeof(foodEntry));
    
    if(strcmp(gmlUnit, "G")==0){
        gmlUnit = "Grams";
    }
    if(strcmp(gmlUnit, "Ml")==0){
        gmlUnit = "Millileters";
    }
    
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


void RemoveDoubleSpaces(char* inputString){
    for(int i =0; i<strlen(inputString); i++){
        if (inputString[i] == '\t'){
            inputString[i] = ' ';
        }
    }
}

void UpperAndLowerCase(char *stringToParse){
    for(int i =0; i<(strlen(stringToParse)-1); i++){
        //printf("%c", stringToParse[i]);

        if ((stringToParse[i] == ' ') || (stringToParse[i] == '~')){
            stringToParse[i+1] = toupper(stringToParse[i+1]);
        }
        else if (stringToParse[i] != ' '){
            stringToParse[i+1] = tolower(stringToParse[i+1]);
        }
    }
}

void PrintFoodEntry(foodEntry* printEntry){
    if (strcmp(printEntry->unitTypeGML, "g")==0){
        printEntry->unitTypeGML = "Grams";
    }
    if (strcmp(printEntry->unitTypeGML, "ml")==0){
        printEntry->unitTypeGML = "Millileters";
    }
    
    printf("Food ID: %d\n", printEntry->foodId);
    printf("Product Name: %s\n", printEntry->prodName);
    printf("Manufacture Name: %s\n", printEntry->manufName);
    printf("Number of Calories: %.2lf\n", printEntry->calNum);
    printf("Grams of Carbs: %.2lf\n", printEntry->carbNum);
    printf("Grams of Fat: %.2lf\n", printEntry->fatNum);
    printf("Grams of Protein: %.2lf\n", printEntry->proteinNum);
    printf("Serving Size is: %.2lf %s\n", printEntry->serveSizeGML, printEntry->unitTypeGML);
    //printf("%s\n", printEntry->unitTypeGML);
    printf("The normal Serving Size is: %.2lf %s\n", printEntry->normServeSize, printEntry->normUnitType);
    
    
    //printf("%s\t", printEntry->normUnitType);
    /*printf("\n\n....%s.....\n\n", printEntry->normUnitType);
    int x = 0;
    x = strlen(printEntry->normUnitType);
    char test[x];
    for (int i = 0; i < x; i++){
        test[i] = printEntry->normUnitType[i];
        printf("%d\n", test[i]);
    }
    printf("%d\n\n",x);*/
}


foodEntry* ParseText (char* stringPtr){
    char *partition;
    char *tempString;
    int i = 0;
    int ID = 0;
    char* foodName = "";
    char* manufact = "";
    double cals = 0.0;
    double carbs = 0.0;
    double fats = 0.0;
    double protein = 0.0;
    double serveGML = 0.0;
    char* unitGML = "";
    double serveNorm = 0.0;
    char* unitNorm = "";
    
    foodEntry *tempFood = NULL;
    
    tempString = strndup(stringPtr, strlen(stringPtr));
    
    
    //printf("INSIDE OF PARSETEXT FUNCTION\n");
    
    
    while ((partition = strsep(&tempString, "~")) != NULL){
        if (i==0){
            ID = atoi(partition);
        }
        else if (i==1){
            foodName = partition;
        }
        else if (i==2){
            manufact = partition;
        }
        else if (i==3){
            cals = atoll(partition);
        }
        else if (i==4){
            carbs = atoll(partition);
        }
        else if (i==5){
            fats = atoll(partition);
        }
        else if (i==6){
            protein = atoll(partition);
        }
        else if (i==7){
            serveGML = atoll(partition);
        }
        else if (i==8){
            unitGML = partition;
        }
        else if (i==9){
            serveNorm = atoll(partition);
        }
        else if (i==10){
            unitNorm = partition;
        }
        i = i + 1;   
    }
    free(tempString);
    
    tempFood = CreateFood(ID, foodName, manufact, cals, carbs, fats, protein, serveGML, unitGML, serveNorm, unitNorm);
        
    return tempFood;
}



int main(){
    
    FILE* foodData = NULL;
    foodData = fopen("FoodDatabase.txt", "r");
    char lineString[250];
    foodEntry *testFood = NULL;
    
    while(fgets(lineString, 250, foodData)){ 
        char *stringPtr = lineString;
        //printf("%s\n", lineString);
        RemoveDoubleSpaces(stringPtr);
        UpperAndLowerCase(stringPtr);
        //printf("%s\n\n", lineString);

        testFood = ParseText(stringPtr);
        PrintFoodEntry(testFood);
    }
    
    
    fclose(foodData);
    return 0;
}