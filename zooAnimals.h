#include <stdbool.h>

typedef struct animal {
    int animalID;
    char* name;
    char sex;
    int quantity;
    char* location;
    struct animal* next;
    struct animal* prev;
} animal_t;

// Linked List operations
animal_t* createList(FILE*);
animal_t* createNodeFromFile(FILE*);
void deleteList(animal_t**);
void insertToList(animal_t**, animal_t*);
void deleteNode(animal_t**, int);
int binarySearch(animal_t*, int);
animal_t* findMiddleNode(animal_t*, animal_t*);
int getNumNodesFromList(animal_t*);

// "Database" operations
void displayListBrief(animal_t*);
void showRecord(animal_t*, int);
void addRecord(animal_t**);
void editRecord(animal_t**);
void deleteRecord(animal_t**);
void searchById(animal_t*);

// Miscellaneous operations
void strToUppercase(char*);
void printToFile(FILE*, animal_t*);
int getAnimalId(animal_t*);
void setAnimalName(char*);
void setAnimalLocation(char*);
char getSex();
int getQuantity();