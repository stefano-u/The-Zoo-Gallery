typedef struct animal {
    int animalID;
    char* name;
    char sex;
    int quantity;
    char* location;
    struct animal* next;
} animal_t;

animal_t* createList(FILE*);
animal_t* createNodeFromFile(FILE*);
void displayListBrief(animal_t*);
void deleteList(animal_t**);
void printToFile(FILE*, animal_t*);
void viewRecord(animal_t*);
void addRecord(animal_t**);
void insertToList(animal_t**, animal_t*);
void strToUppercase(char*);