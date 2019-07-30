#include <stdbool.h>

typedef struct animal {
    int animalID;
    char* name;
    char sex;
    int quantity;
    char* location;
    struct animal* next;
} animal_t;

// Linked List operations
animal_t* createList(FILE*);
animal_t* createNodeFromFile(FILE*);
void deleteList(animal_t**);
void insertToList(animal_t**, animal_t*);

// Database operations
void displayListBrief(animal_t*);
bool viewRecord(animal_t*, int);
void addRecord(animal_t**);
void deleteRecord(animal_t**);
void searchById(animal_t*);

// Miscellaneous operations
void strToUppercase(char*);
void printToFile(FILE*, animal_t*);