//This is going to be the main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Global variables for the total nutrition
double globalCals = 0.0;
double globalCarbs = 0.0;
double globalFats = 0.0;
double globalProtein = 0.0;

//The data structure used with the large binary search tree
typedef struct foodEntry {
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
    struct foodEntry *left;
    struct foodEntry *right;
}foodEntry;

//The data structure used when the user enters an amount of servings they consumed for a specific food
typedef struct enteredFoodEntry {
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
    struct enteredFoodEntry *left;
    struct enteredFoodEntry *right;
    double unitsConsumed;
}enteredFoodEntry;


//Function Declarations
void CreateLog(foodEntry *root);

void RetrieveLog(foodEntry *root);

void UpdateLog(foodEntry *root);

void PrintLog(char *fileName2);

void AddEditDelete(char *fileName3,foodEntry *root);

void textGui(foodEntry *root);

void DeleteLog(foodEntry *root);

struct enteredFoodEntry* ParseLog(char* logInput);

void PrintLog(char *fileName2);

struct enteredFoodEntry* CreateEnteredFood(int id, char *prod, char *manuf, double calories, double carbs, double fat, double proteins, double gmlServeSize, char *gmlUnit, double regServeSize, char *regUnit, double servingsAte);

struct enteredFoodEntry *InsertFoodEntry(struct enteredFoodEntry *root, struct enteredFoodEntry *newFoodEntry);

void PrintLogInorder(struct enteredFoodEntry *root);

void PrintEnteredFoodEntry(struct enteredFoodEntry  *printEntry);

void PrintNutrition(struct enteredFoodEntry *foodTemp);

struct enteredFoodEntry *minValueNode(struct enteredFoodEntry* node);

void DeleteTheEntry(char *nameOfFile, foodEntry *root, char *deleteString);

void PrintToFile(char *fileName, struct enteredFoodEntry *root);



//Function that input that takes parsed information and turns it into the data struct foodEntry
struct foodEntry* CreateFood(int id, char *prod, char *manuf, double calories, double carbs, double fat, double proteins, double gmlServeSize, char *gmlUnit, double regServeSize, char *regUnit){
    struct foodEntry *tempStruct = (struct foodEntry*)malloc(sizeof(foodEntry));
    
    if(strcmp(gmlUnit, "G")==0){
        gmlUnit = "Grams";
    }
    if(strcmp(gmlUnit, "Ml")==0){
        gmlUnit = "Millileters";
    }
    
    int x = strlen(regUnit);
    
    int counter= 0;
    
    char *unitTemp;
    unitTemp = regUnit;
    for (int i = 0; i < x; i++){
        if (isalpha(unitTemp[i])){
            counter = i;
        }
    }
    if(counter < (x-1)){
        unitTemp[counter+1] = '\0';
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
    tempStruct->left = NULL;
    tempStruct->right = NULL;    
    return tempStruct;
}

//Remove the tabs from the input string
void RemoveDoubleSpaces(char* inputString){
    for(int i =0; i<strlen(inputString); i++){
        if (inputString[i] == '\t'){
            inputString[i] = ' ';
        }
    }
}

//Change all of the first letters of a string to capital and all others to lower case
void UpperAndLowerCase(char *stringToParse){
    for(int i =0; i<(strlen(stringToParse)-1); i++){

        if ((stringToParse[i] == ' ') || (stringToParse[i] == '~')){
            stringToParse[i+1] = toupper(stringToParse[i+1]);
        }
        else if (stringToParse[i] != ' '){
            stringToParse[i+1] = tolower(stringToParse[i+1]);
        }
    }
}

//Remove the spaces at the end of the string
void RemoveSpacesAndCaps(char *stringIn){
    int x = 0;
    for (int i = 0; i<strlen(stringIn); i++){
        if (stringIn[i] != ' '){
            stringIn[i] = tolower(stringIn[i]);
            stringIn[x++] = stringIn[i];
        }
    }
    stringIn[x-1] = '\0';
}

//Insert an item into the large binary search tree
struct foodEntry *InsertEntry(struct foodEntry *root, struct foodEntry *newEntry){
    struct foodEntry *rootPtr = root;
    struct foodEntry *entryPtr = newEntry;
    
    if (root == NULL){
        return newEntry;
    }
    
    if (strcmp(entryPtr->prodName, rootPtr->prodName)<0){
        rootPtr->left = InsertEntry(rootPtr->left, entryPtr);
    }
    
    else {
        rootPtr->right = InsertEntry(rootPtr->right, entryPtr);
    }   
    return root;
}

//Search for a node in the large binary search tree
struct foodEntry* SearchForNode(struct foodEntry* root, char *input){
    
    struct foodEntry *searchPtr = root;
    
    if (searchPtr == NULL){
        //printf("Your entry cannot be found, please try again\n");
        return searchPtr;
    }
  
    
    else if (strcmp(input, searchPtr->prodName)<0) {
        return SearchForNode(searchPtr->left, input); 
    }
    
    else if (strcmp(input, searchPtr->prodName)>0) {
        return SearchForNode(searchPtr->right, input);
    }
 
    return root;
}

//Do an inorder print of the large binary tree, used for testing
void PrintInorder(struct foodEntry *root) { 
    if (root != NULL) 
    { 
        PrintInorder(root->left); 
        printf("%s\n", root->prodName); 
        PrintInorder(root->right); 
    } 
} 

//Print out the data struct foodEntry to the user
void PrintFoodEntry(foodEntry* printEntry){
    if (strcmp(printEntry->unitTypeGML, "g")==0){
        printEntry->unitTypeGML = "Grams";
    }
    if (strcmp(printEntry->unitTypeGML, "ml")==0){
        printEntry->unitTypeGML = "Millileters";
    }
    
    printf("\t\tFood ID: %d\n", printEntry->foodId);
    printf("\t\tProduct Name: %s\n", printEntry->prodName);
    printf("\t\tManufacture Name: %s\n", printEntry->manufName);
    printf("\t\tNumber of Calories: %.2lf\n", printEntry->calNum);
    printf("\t\tGrams of Carbs: %.2lf\n", printEntry->carbNum);
    printf("\t\tGrams of Fat: %.2lf\n", printEntry->fatNum);
    printf("\t\tGrams of Protein: %.2lf\n", printEntry->proteinNum);
    printf("\t\tServing Size is: %.2lf %s\n", printEntry->serveSizeGML, printEntry->unitTypeGML);
    printf("\t\tThe normal Serving Size is: %.2lf %s\n", printEntry->normServeSize, printEntry->normUnitType);
}

//Take a line from the file and parse it into multipe elements
struct foodEntry* ParseText (char* stringPtr){
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
    
    struct foodEntry *tempFood = NULL;
    
    tempString = strndup(stringPtr, strlen(stringPtr));
    
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
            cals = atof(partition);
        }
        else if (i==4){
            carbs = atof(partition);
        }
        else if (i==5){
            fats = atof(partition);
        }
        else if (i==6){
            protein = atof(partition);
        }
        else if (i==7){
            serveGML = atof(partition);
        }
        else if (i==8){
            unitGML = partition;
        }
        else if (i==9){
            serveNorm = atof(partition);
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

//calculate the nutrition of a food by using the consumed servings
void calculateNutrition(foodEntry *foodTemp, double unitsAte){
    
    double servingsConsumed = 0;
    double caloriesConsumed = 0;
    double carbsConsumed = 0;
    double fatConsumed = 0;
    double proteinConsumed = 0;
    double gmlServings = 0;
     
    servingsConsumed = (unitsAte / foodTemp->normServeSize);
    gmlServings = (servingsConsumed * foodTemp->serveSizeGML);
    gmlServings = (gmlServings / 100);
    caloriesConsumed = (gmlServings * foodTemp->calNum);
    carbsConsumed = (gmlServings * foodTemp->carbNum);
    fatConsumed = (gmlServings * foodTemp->fatNum);
    proteinConsumed = (gmlServings * foodTemp->proteinNum);
    printf("\t\tYou consumed: %.2lf Servings\n", servingsConsumed);
    printf("\t\tYou consumed: %.2lf Calories\n", caloriesConsumed);
    printf("\t\tYou consumed: %.2lf %s of carbs\n", carbsConsumed, foodTemp->unitTypeGML);
    printf("\t\tYou consumed: %.2lf %s of fat\n", fatConsumed, foodTemp->unitTypeGML);
    printf("\t\tYou consumed: %.2lf %s protein\n\n", proteinConsumed, foodTemp->unitTypeGML);
    return;
}

//Enter a new entry to a users log file
void EnterNewEntry(char *nameOfFile, foodEntry *root){
    
    char userInput[250];
    printf("\n\t\tType in the exact name of the food you want to enter: ");
    fgets(userInput, 250, stdin);
    char *inputPtr = userInput;
    inputPtr[strlen(inputPtr)-1] = '\0';
    
    struct foodEntry *searchFood = NULL; 
    searchFood = SearchForNode(root, inputPtr);
   
    if(searchFood != NULL){
        printf("\n\n\t\tYour entry was found!\n\n");    
        PrintFoodEntry(searchFood);
    }
    else{
        printf("\n\t\tYou entry was not found, make sure you type in the name of the food exactly\n");
        printf("\t\tPlease check the user manual for the food name format\n\n\n");
        textGui(root);
    }
    
    double unitsConsumed = 0;
    
    printf("\t\tThe serving size of %s is %.2lf %ss\n\n", searchFood->prodName, searchFood->normServeSize, searchFood->normUnitType);
    printf("\t\tEnter the number of %ss that you consumed (ex: 25.0): ", searchFood->normUnitType);
    scanf("%lf", &unitsConsumed);
    printf("\n\t\tYou consumed: %.2lf %ss\n", unitsConsumed, searchFood->normUnitType);
 
    FILE *fpEnter;
    fpEnter = fopen(nameOfFile, "a");
    
    fprintf(fpEnter, "%d~%s~%s~%lf~%lf~%lf~%lf~%lf~%s~%lf~%s~%lf\n", searchFood->foodId, searchFood->prodName, searchFood->manufName, searchFood->calNum, searchFood->carbNum, searchFood->fatNum, searchFood->proteinNum, searchFood->serveSizeGML, searchFood->unitTypeGML, searchFood->normServeSize, searchFood->normUnitType, unitsConsumed);
    
    fclose(fpEnter);
    
    calculateNutrition(searchFood, unitsConsumed);
    
    printf("\n\t\t\tYour entry has been submitted to your log!\n");
    printf("\n\t\tWould you like to add another entry? Type \'Y\' or \'N\':  ");
    
    char inputText[50];
    fgets(inputText, 50, stdin);
    char *inputPtr1 = inputText;
    
    if(inputPtr1[0] == '\n'){
        fgets(inputText, 50, stdin);
    }
    
    int inputLength = strlen(inputPtr1);
    inputPtr1[inputLength-1] = '\0';
    
    if (strcmp(inputPtr1,"Y")==0){
        EnterNewEntry(nameOfFile, root);
    }
    else if (strcmp(inputPtr1,"N")==0){
        printf("\n");
        textGui(root);
    }
    return;
}
    
//This is the Gui that the user can move around in   
void textGui(foodEntry *root){
    struct foodEntry *rootPtr = root;
    char inputText[50];
    printf("\tWelcome to the Nutritional Database, where you can create and maintain your own nutritional log!!\n\n");
    printf("\tYou have four options that are found below, please enter the number attached with each\n\n");
    printf("\t\t\t 1: Create your own nutrtional log (first time user)\n");
    printf("\t\t\t 2: Retrieve your nutritional log (returning user)\n");
    printf("\t\t\t 3: Update your nutritional log (returning user)\n");
    printf("\t\t\t 4: Delete your nutritional log (returning user)\n\n");
    printf("\t\t\t\t Enter \'Q\' to quit the program\n\n");
    printf("\t\tEnter a number or \'Q\' that correpsonds with your action: ");

    fgets(inputText, 50, stdin);
    char *inputPtr = inputText;
    if(inputPtr[0] == '\n'){
        fgets(inputText, 50, stdin);
    }
    int inputLength = strlen(inputPtr);
    inputPtr[inputLength-1] = '\0';

    if (strcmp(inputPtr,"1")==0){
        printf("\n\t\tYou chose: Create your own nutritional log\n\n");
        CreateLog(rootPtr);
    }
    else if (strcmp(inputPtr,"2")==0){
        printf("\n\t\tYou chose: Retrieve your exisiting nutritional log\n\n");
        RetrieveLog(rootPtr);
    }
    else if (strcmp(inputPtr,"3")==0){
        printf("\n\t\tYou chose: Update your exisiting nutritional log\n\n");
        UpdateLog(rootPtr);
    }
    else if (strcmp(inputPtr,"4")==0){
        printf("\n\t\tYou chose: Delete your nutritional log\n\n");
        DeleteLog(rootPtr);
    }
    else if (strcmp(inputPtr,"Q")==0){
        printf("\n\tBye, please come back soon!!\n\n");
        exit(0);
    }
    else {
        printf("%s", inputPtr);
        printf("\n\n\t\tYour input is invalid, please try again\n\n");
        textGui(rootPtr);
    }
    return;
}

//Option 1 in text gui, this creates a new file if the user is new and returns them to textGui if they are not new
void CreateLog(foodEntry *root){
    char nameInput[40];
    printf("\t\tEnter your first and last name (40 character limit): ");
    char *namePtr;
    fgets(nameInput, 40, stdin);
    namePtr = nameInput;
    RemoveSpacesAndCaps(namePtr);
    char log[5] = ".log"; 
    char *logPtr = log;
    int y = strlen(logPtr);
    logPtr[y] = '\0';
    strcat(namePtr, logPtr);
    
    
    FILE *fpCreate;
    fpCreate = fopen(namePtr, "r");
    if (fpCreate == NULL || !fpCreate){
        EnterNewEntry(namePtr, root);
        printf("\t\tYour nutrional log was created and your entry was succesful!\n");
        printf("\n\t\tReturning to home screen\n");
        printf("\t\t\tTo see your current log select the retrieve option\n");
        printf("\t\t\tTo add another food entry select the update option\n\n");
        textGui(root);
    }
    else {
        printf("\n\n\t\tA file with your name already exists, returning to home screen\n\n\n\n");
        textGui(root);
    }
}

//Option 2 in text gui, this returns a users file and prints out all foods recorded inside
void RetrieveLog(foodEntry *root){
    char nameInput[40];
    printf("\t\tEnter your first and last name (40 character limit): ");
    char *namePtr;
    fgets(nameInput, 40, stdin);
    namePtr = nameInput;
    RemoveSpacesAndCaps(namePtr);
    char log[5] = ".log"; 
    char *logPtr = log;
    int y = strlen(logPtr);
    logPtr[y] = '\0';
    strcat(namePtr, logPtr);
    
    FILE *fpCreate;
    fpCreate = fopen(namePtr, "r");
    if (fpCreate == NULL || !fpCreate){
        printf("\n\t\tNo log with your name exists, please select the create log option\n\n\n");
        textGui(root);
    }
    else {
        printf("\n\n\t\tA file with your name exists, Go to file\n\n\n\n");
        PrintLog(namePtr);
    }
    textGui(root);
}

//Option 3 of textGui, this gives a returning user the option to add, update, or remove food entries in their log
void UpdateLog(foodEntry *root){
    char nameInput[40];
    printf("\t\tEnter your first and last name (40 character limit): ");
    char *namePtr;
    fgets(nameInput, 40, stdin);
    namePtr = nameInput;
    RemoveSpacesAndCaps(namePtr);
    char log[5] = ".log"; 
    char *logPtr = log;
    int y = strlen(logPtr);
    logPtr[y] = '\0';
    strcat(namePtr, logPtr);
    
    FILE *fpCreate;
    fpCreate = fopen(namePtr, "r");
    if (fpCreate == NULL || !fpCreate){
        printf("\t\tNo log with your name exists, please select the create log option\n");
        textGui(root);
    }
    else {
        printf("\n\n\t\tA file with your name exists, Go to file\n\n\n\n");
        AddEditDelete(namePtr, root);
    }
}

//This is a function called in updateLog that lets the user decide if they watn to add, edit, or delete
void AddEditDelete(char *fileName3, foodEntry *root){
    
    char inputText[50];
    printf("\t\tYou have selected to update your nutritional log, there are three options listed below\n\n");
    printf("\t\tPlease enter the number attached with each\n\n");
    printf("\t\t\t 1: Add a new food entry to your log\n");
    printf("\t\t\t 2: Update an existing food entry (change the number of servings)\n");
    printf("\t\t\t 3: Delete an existing food entry\n\n");
    printf("\t\tIf none of these options are what you desired type \'Q\' to return to the home page\n");
    printf("\n\t\tEnter the number or \'Q\' that corresponds with with your desired option: ");

    fgets(inputText, 50, stdin);
    char *inputPtr = inputText;
    if(inputPtr[0] == '\n'){
        fgets(inputText, 50, stdin);
    }
    int inputLength = strlen(inputPtr);
    inputPtr[inputLength-1] = '\0';

    if (strcmp(inputPtr,"1")==0){
        printf("\n\t\tYou chose: Add a new food entry\n\n");
        EnterNewEntry(fileName3, root);
    }
    else if (strcmp(inputPtr,"2")==0){
        printf("\n\t\tYou chose: Update an existing entry\n\n");
        printf("\t\t\tTo update and existing entry delete the entry and then add a\n"); 
        printf("\t\t\tnew entry of the same food type with a different serving size\n\n");
    }
    else if (strcmp(inputPtr,"3")==0){
        printf("\n\t\tYou chose: Delete an existing entry\n\n");
        printf("\t\tPlease enter the food that you would like to delete:  ");
        char deleteText[50];
        fgets(deleteText, 50, stdin);
        char *deletePtr = deleteText;
        if(deletePtr[0] == '\n'){
            fgets(deleteText, 50, stdin);
        }
        int deleteLength = strlen(deletePtr);
        deletePtr[deleteLength-1] = '\0';
        DeleteTheEntry(fileName3, root, deletePtr);
         
    }
    else if (strcmp(inputPtr,"Q")==0){
        printf("\n\tBye, please come back soon!!\n\n");
        textGui(root);
    }
}

//This function is called multiple timess and it reads in a user file and calls the binary tree build and calls the print function
void PrintLog(char *fileName2){
    FILE *fpPrint = NULL;
    fpPrint = fopen(fileName2, "r");
    char fileString[250];
    
    struct enteredFoodEntry *tempEntry = NULL;
    
    struct enteredFoodEntry *printRoot = NULL;
    
    int x = 0;
    
    printf("\n\n\t\tRetrieving log information...\n\n");
    
    while(fgets(fileString, 250, fpPrint)){
        char *printPtr = fileString;
        
        tempEntry = ParseLog(printPtr);
        if (x == 0){
            printRoot = InsertFoodEntry(printRoot, tempEntry); 
            x = 1;
        }
        else {
            InsertFoodEntry(printRoot, tempEntry);
        }
    }
    PrintLogInorder(printRoot);
    
    printf("\t\tTOTAL NUTRITION INFORMATION:\n\n");
    printf("\t\tTotal Calories: %.2lf\n", globalCals);
    printf("\t\tTotal Carbs: %.2lf\n", globalCarbs);
    printf("\t\tTotal Fat: %.2lf\n", globalFats);
    printf("\t\tTotal Protein: %.2lf\n\n\n", globalProtein);

}

//This function parses the user file information and returns a enteredFoodEntry type from the fileline strings
struct enteredFoodEntry* ParseLog(char* logInput){
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
    double enteredServings = 0.0;
    
    struct enteredFoodEntry *tempEnteredFood = NULL;
    
    tempString = strndup(logInput, strlen(logInput));    
    
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
            cals = atof(partition);
        }
        else if (i==4){
            carbs = atof(partition);
        }
        else if (i==5){
            fats = atof(partition);
        }
        else if (i==6){
            protein = atof(partition);
        }
        else if (i==7){
            serveGML = atof(partition);
        }
        else if (i==8){
            unitGML = partition;
        }
        else if (i==9){
            serveNorm = atof(partition);
        }
        else if (i==10){
            unitNorm = partition;
        }
        else if (i==11){
            enteredServings = atof(partition);
        }
        i = i + 1;   
    }
    free(tempString); 
    tempEnteredFood = CreateEnteredFood(ID, foodName, manufact, cals, carbs, fats, protein, serveGML, unitGML, serveNorm, unitNorm, enteredServings);
    return tempEnteredFood;
}
 
//This function takes the parsed information and creates an enteredFoodEntry type
struct enteredFoodEntry* CreateEnteredFood(int id, char *prod, char *manuf, double calories, double carbs, double fat, double proteins, double gmlServeSize, char *gmlUnit, double regServeSize, char *regUnit, double servingsAte){
    struct enteredFoodEntry *newTempStruct = (struct enteredFoodEntry*)malloc(sizeof(enteredFoodEntry));
    
    newTempStruct->foodId = id;
    newTempStruct->prodName = malloc(strlen(prod)+1);
    strcpy(newTempStruct->prodName, prod);
    newTempStruct->manufName = malloc(strlen(manuf)+1);
    strcpy(newTempStruct->manufName, manuf);
    newTempStruct->calNum = calories;
    newTempStruct->carbNum = carbs;
    newTempStruct->fatNum = fat;
    newTempStruct->proteinNum = proteins;
    newTempStruct->serveSizeGML = gmlServeSize;
    newTempStruct->unitTypeGML = malloc(strlen(gmlUnit)+1);
    strcpy(newTempStruct->unitTypeGML, gmlUnit);
    newTempStruct->normServeSize = regServeSize;
    newTempStruct->normUnitType = malloc(strlen(regUnit)+1);
    strcpy(newTempStruct->normUnitType, regUnit);
    newTempStruct->left = NULL;
    newTempStruct->right = NULL;
    newTempStruct->unitsConsumed = servingsAte;
    
    return newTempStruct;
}

//This function enters a new food netry into the binary tree for that user
struct enteredFoodEntry *InsertFoodEntry(struct enteredFoodEntry *root, struct enteredFoodEntry *newFoodEntry){
    struct enteredFoodEntry *rootPtr = root;
    struct enteredFoodEntry *entryPtr = newFoodEntry;
    
    if (root == NULL){
        return newFoodEntry;
    }
    
    if (strcmp(entryPtr->prodName, rootPtr->prodName)<0){
        rootPtr->left = InsertFoodEntry(rootPtr->left, entryPtr);
    }
    
    else {
        rootPtr->right = InsertFoodEntry(rootPtr->right, entryPtr);
    }   
    return root;
}

//prints the binary tree inorder
void PrintLogInorder(struct enteredFoodEntry *root) { 

    if (root != NULL) 
    { 
        PrintLogInorder(root->left); 
        PrintNutrition(root);
        PrintLogInorder(root->right); 
    } 

}

//prints the full binary tree with the specific data types within
void PrintEnteredFoodEntry(struct enteredFoodEntry  *printEntry){
    if (strcmp(printEntry->unitTypeGML, "g")==0){
        printEntry->unitTypeGML = "Grams";
    }
    if (strcmp(printEntry->unitTypeGML, "ml")==0){
        printEntry->unitTypeGML = "Millileters";
    }
    
    printf("\n\t\tFood ID: %d\n", printEntry->foodId);
    printf("\t\tProduct Name: %s\n", printEntry->prodName);
    printf("\t\tManufacture Name: %s\n", printEntry->manufName);
    printf("\t\tNumber of Calories: %.2lf\n", printEntry->calNum);
    printf("\t\tGrams of Carbs: %.2lf\n", printEntry->carbNum);
    printf("\t\tGrams of Fat: %.2lf\n", printEntry->fatNum);
    printf("\t\tGrams of Protein: %.2lf\n", printEntry->proteinNum);
    printf("\t\tServing Size is: %.2lf %s\n", printEntry->serveSizeGML, printEntry->unitTypeGML);
    //printf("%s\n", printEntry->unitTypeGML);
    printf("\t\tThe normal serving size is: %.2lf %s\n", printEntry->normServeSize, printEntry->normUnitType);
    printf("\t\tYour serving size was: %.lf %s\n", printEntry->unitsConsumed, printEntry->normUnitType);
}

//Prints the nutrition information for each food entry
void PrintNutrition(struct enteredFoodEntry *foodTemp){
    
    double servingsConsumed = 0;
    double caloriesConsumed = 0;
    double carbsConsumed = 0;
    double fatConsumed = 0;
    double proteinConsumed = 0;
    double gmlServings = 0;
     
    servingsConsumed = (foodTemp->unitsConsumed / foodTemp->normServeSize);
    gmlServings = (servingsConsumed * foodTemp->serveSizeGML);
    gmlServings = (gmlServings / 100);
    caloriesConsumed = (gmlServings * foodTemp->calNum);
    carbsConsumed = (gmlServings * foodTemp->carbNum);
    fatConsumed = (gmlServings * foodTemp->fatNum);
    proteinConsumed = (gmlServings * foodTemp->proteinNum);
    printf("\n\t\tFood ID: %d\n", foodTemp->foodId);
    printf("\t\tProduct Name: %s\n", foodTemp->prodName);
    printf("\t\tManufacture Name: %s\n", foodTemp->manufName);
    printf("\t\tServings consumed: %.2lf Servings\n", servingsConsumed);
    printf("\t\tCalories consumed: %.2lf Calories\n", caloriesConsumed);
    printf("\t\tCarbs consumed: %.2lf %s of carbs\n", carbsConsumed, foodTemp->unitTypeGML);
    printf("\t\tFat consumed: %.2lf %s of fat\n", fatConsumed, foodTemp->unitTypeGML);
    printf("\t\tProtein consumed: %.2lf %s protein\n\n", proteinConsumed, foodTemp->unitTypeGML);
    globalCals = (globalCals + caloriesConsumed);
    globalCarbs = (globalCarbs + carbsConsumed);
    globalFats = (globalFats + fatConsumed);
    globalProtein = (globalProtein + proteinConsumed);
    return;
}

//deletes a node that the user decides on 
struct enteredFoodEntry *deleteNode(struct enteredFoodEntry* root, char *key) {  
    if (root == NULL) return root; 

    if (strcmp(key, root->prodName)<0){
        root->left = deleteNode(root->left, key); 
    }
    else if (strcmp(key, root->prodName)>0){
        root->right = deleteNode(root->right, key); 
    }
    else{ 
        if (root->left == NULL) { 
            struct enteredFoodEntry *temp = (struct enteredFoodEntry*)malloc(sizeof(enteredFoodEntry));
            temp = root->right; 
            free(root); 
            return temp; 
        } 
        else if (root->right == NULL) { 
            struct enteredFoodEntry *temp = (struct enteredFoodEntry*)malloc(sizeof(enteredFoodEntry));
            temp = root->left; 
            free(root); 
            return temp; 
        } 
  
        struct enteredFoodEntry *temp = minValueNode(root->right); 
  
        root->foodId = temp->foodId; 
        strcpy(root->prodName, temp->prodName);
        strcpy(root->manufName, temp->manufName);
        root->calNum = temp->calNum;
        root->carbNum = temp->carbNum;
        root->fatNum = temp->fatNum;
        root->proteinNum = temp->proteinNum;
        root->serveSizeGML = temp->serveSizeGML;
        strcpy(root->unitTypeGML, temp->unitTypeGML);
        root->normServeSize = temp->normServeSize;
        strcpy(root->normUnitType, temp->normUnitType);
        root->unitsConsumed = temp->unitsConsumed;

        root->right = deleteNode(root->right, temp->prodName); 
    } 
    return root; 
} 

//A minvalue function used in the delete node
struct enteredFoodEntry *minValueNode(struct enteredFoodEntry *node) { 
    struct enteredFoodEntry *current = node; 
  
    while (current->left != NULL) {
        current = current->left; 
    }
    
    return current; 
} 

//Actually deletes the food entry
void DeleteTheEntry(char *nameOfFile, foodEntry *root, char *deleteString){
    
    FILE *fpPrint = NULL;
    fpPrint = fopen(nameOfFile, "r");
    char fileString[250];
    
    struct enteredFoodEntry *tempEntry = NULL;
    
    struct enteredFoodEntry *printRoot = NULL;
    
    int x = 0;
    
    printf("\n\n\t\tRetrieving log information...\n\n");
    
    while(fgets(fileString, 250, fpPrint)){
        char *printPtr = fileString;
        
        tempEntry = ParseLog(printPtr);
        if (x == 0){
            printRoot = InsertFoodEntry(printRoot, tempEntry); 
            x = 1;
        }
        else {
            InsertFoodEntry(printRoot, tempEntry);
        }
    }
    printRoot = deleteNode(printRoot, deleteString);
    FILE *fpDelete;
    fpDelete = fopen(nameOfFile, "r");
    if (fpDelete == NULL || !fpDelete){
        printf("\t\tNo log with your name exists to be able to delete!\n\n");
        printf("\t\tReturning you the the home page\n\n\n");
        textGui(root);
    }
    else {
        remove(nameOfFile);
        PrintToFile(nameOfFile, printRoot);
    }
}

//Prints the new bindary tree to the file
void PrintToFile(char *fileName, struct enteredFoodEntry *root){
    FILE *fpDelete;
    fpDelete = fopen(fileName, "a");
    if (root != NULL) 
    { 
        PrintToFile(fileName, root->left); 
        fprintf(fpDelete, "%d~", root->foodId);
        fprintf(fpDelete, "%s~", root->prodName);
        fprintf(fpDelete, "%s~", root->manufName);
        fprintf(fpDelete, "%lf~", root->calNum);
        fprintf(fpDelete, "%lf~", root->carbNum);
        fprintf(fpDelete, "%lf~", root->fatNum);
        fprintf(fpDelete, "%lf~", root->proteinNum);
        fprintf(fpDelete, "%lf~", root->serveSizeGML);
        fprintf(fpDelete, "%s~", root->unitTypeGML);
        fprintf(fpDelete, "%lf~", root->normServeSize);
        fprintf(fpDelete, "%s~", root->normUnitType);
        fprintf(fpDelete, "%lf\n", root->unitsConsumed);
        PrintToFile(fileName, root->right); 
    } 
}


//Option 4 in textGui, deletes a n entire user
void DeleteLog(foodEntry *root){
    char nameInput[40];
    printf("\t\tEnter your first and last name (40 character limit): ");
    char *namePtr;
    fgets(nameInput, 40, stdin);
    namePtr = nameInput;
    RemoveSpacesAndCaps(namePtr);
    char log[5] = ".log"; 
    char *logPtr = log;
    int y = strlen(logPtr);
    logPtr[y] = '\0';
    strcat(namePtr, logPtr);
    
    FILE *fpDelete;
    fpDelete = fopen(namePtr, "r");
    if (fpDelete == NULL || !fpDelete){
        printf("\t\tNo log with your name exists to be able to delete!\n\n");
        printf("\t\tReturning you the the home page\n\n\n");
        textGui(root);
    }
    else {
        remove(namePtr);
        printf("\n\n\t\tYour user log has been deleted\n\n");
        printf("\t\tReturning you to the home page\n\n\n");
        textGui(root);
    }
}


//int main where the original food database is parsed and textGui is called for the first time 
int main(){
    
    FILE* foodData = NULL;
    foodData = fopen("FoodDatabase.txt", "r");
    char lineString[250];
    struct foodEntry *testFood = NULL;
    
    struct foodEntry *root = NULL;
    
    int x = 0;
    
    printf("\n\n\n\tDownloading food database data...\n\n");
    while(fgets(lineString, 250, foodData)){ 
        char *stringPtr = lineString;
        RemoveDoubleSpaces(stringPtr);
        UpperAndLowerCase(stringPtr);

        testFood = ParseText(stringPtr);
        if (x == 0){
            root = InsertEntry(root, testFood); 
            x = 1;
        }
        else {
            InsertEntry(root, testFood);
        }
    }
    
    struct foodEntry *rootPtr = NULL;
    rootPtr = root;
    
    textGui(rootPtr);
    
    fclose(foodData);
   
    return 0;
}

