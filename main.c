//This is going to be the main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            printf("######");
        }
        if (i<(strlen(inputString)-1)){
            printf("%d %c", i, inputString[i]);
            printf("....");
            if ((inputString[i]==' ') && (inputString[i+1] == ' ')){
                printf("     !!!!!");
                for (int x = i; x<strlen(inputString); x++){
                    inputString[x] = inputString[x+1];
                }
            }
        }
        printf("\n");
        //if (inputString[i] == '\n'){
        //    inputString[i] = ' ';
        //}
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
    printf("%s of Calories: %lf\n", printEntry->unitTypeGML, printEntry->calNum);
    printf("%s of Carb: %lf\n", printEntry->unitTypeGML, printEntry->carbNum);
    printf("%s of Fat: %lf\n", printEntry->unitTypeGML, printEntry->fatNum);
    printf("%s of Protein: %lf\n", printEntry->unitTypeGML, printEntry->proteinNum);
    printf("Serving Size is: %lf %s\n", printEntry->serveSizeGML, printEntry->unitTypeGML);
    //printf("%s\n", printEntry->unitTypeGML);
    printf("The normal Serving Size is: %lf %ss\n", printEntry->normServeSize, printEntry->normUnitType);
    //printf("%s\n", printEntry->normUnitType);
    printf("\n\n....%s.....\n\n", printEntry->normUnitType);
    int x = 0;
    x = strlen(printEntry->normUnitType);
    printf("%d\n\n",x);
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
            //printf("The ID is: %d\n", ID);
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
    foodData = fopen("FoodDatabaseTest.txt", "r");
    char lineString[250];
    foodEntry *testFood = NULL;
    
    while(fgets(lineString, 250, foodData)){ 
        char *stringPtr = lineString;
        RemoveDoubleSpaces(stringPtr);
        //printf("%s\n", lineString);

        testFood = ParseText(stringPtr);
        PrintFoodEntry(testFood);
    }
    

    /*
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
    
    
    testFood = CreateFood(y, prod, manuf, cals, carbs, fats, proteins, gmlSize, gmlUnit, normSize, normUnit);
    */
    
    //printf("%s\n\n", testFood->normUnitType);
     
    
    fclose(foodData);
    return 0;
}