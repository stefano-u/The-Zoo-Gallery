#include <stdbool.h>

#define MAX_LEN 100
#define FLUSH stdin=freopen(NULL,"r",stdin)
#define REMOVERN(str) str[strcspn(str,"\r\n")]=0
#define REMOVEN(str) str[strcspn(str,"\n")]=0

typedef struct animal {
    int animalID;
    char* name;
    char sex;
    int quantity;
    char* location;
    struct animal* next;
    struct animal* prev;
} animal_t;

typedef struct queueNode {
    int availableID;
    struct queueNode* next;
} queueNode_t;

typedef struct queue {
    queueNode_t *head, *tail;
} queue_t;


// Linked List operations
int getLastId(FILE*);
animal_t* createList(FILE*, queue_t*);
animal_t* createNodeFromFile(FILE*, queue_t*);
void deleteList(animal_t**);
void insertToList(animal_t**, animal_t*);
void deleteNode(animal_t**, animal_t*);
animal_t* binarySearch(animal_t**, int);
animal_t* findMiddleNode(animal_t*, animal_t*);
int getNumNodesFromList(animal_t*);

// Queue operations
queue_t* initQueue();
bool isQueueEmpty(queue_t*);
void enqueue(queue_t*, int);
int dequeue(queue_t*);

// "Database" operations
void displayListBrief(animal_t*);
void showRecord(animal_t*);
void addRecord(animal_t**, queue_t*, int*);
void editRecord(animal_t**);
void deleteRecord(animal_t**, queue_t*);
void searchById(animal_t**);
void searchOptions(animal_t*);
void searchByName(animal_t*);

// Miscellaneous operations
void clearScreen();
void strToUppercase(char*);
void printToFile(FILE*, animal_t*, int);
int getAnimalId(animal_t*);
void setAnimalName(char*);
void setAnimalLocation(char*);
char getSex();
int getQuantity();