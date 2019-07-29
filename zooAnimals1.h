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
void displayList(animal_t*);
void displayListBrief(animal_t*);
void deleteList(animal_t**);
void printToFile(FILE*, animal_t*);

