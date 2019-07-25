#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
} animal_t;

animal_t* createList(FILE*);
animal_t* createNodeFromFile(FILE*);
void displayList(animal_t*);
void displayListBrief(animal_t*);
void deleteList(animal_t**);
void printToFile(FILE*, animal_t*);

int main() {
    // Opens text file for reading/writing
    FILE *fp = fopen("dataNEW.txt", "r+");
    if (fp == NULL) {
        printf("Error reading file.\n");
        exit(1);
    }

    // Create linked list from file
    animal_t* list = createList(fp);


    puts("=== Welcome to the Zoo Gallery ===\n");
    int choice;

    do {
        puts("Please select from one of the following options:");
        puts("'1' to show all records");
        puts("'2' to show details of a specific record");
        puts("'3' to add a record");
        puts("'5 to edit a record");
        puts("'6' to delete a record");
        puts("'7' to search for a record");
        puts("'0' to quit");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                break;
            case 1:
                displayListBrief(list);
                break;
            case 2:
                break;

        }

    } while (choice != 0);
    // Display contents of the list
    //    displayList(list);

    // Close file pointer
    fclose(fp);

    // Print to file
    printToFile(fp, list);

    // Free linked list & contents of nodes
    deleteList(&list);

    return 0;
}

// Creates a linked list based on data on file

animal_t* createList(FILE *fp) {
    animal_t *node = NULL, *current = NULL, *head = NULL;

    // Creates nodes and puts it on the linked list
    while ((node = createNodeFromFile(fp)) != NULL) {
        if (head == NULL) {
            head = node;
        } else {
            current->next = node;
        }
        current = node;
    }

    return head;
}

// Create nodes based on input from file

animal_t* createNodeFromFile(FILE* fp) {

    char temp[MAX_LEN] = {0};

    // Checks to see if there is more data in the file
    if (fgets(temp, sizeof (temp), fp) != NULL) {

        // Allocate memory for the node
        animal_t* node = (animal_t*) malloc(sizeof (animal_t));
        if (node == NULL) {
            printf("Cannot allocate memory for this animal!\n");
            return node;
        }

        // Remove "\r\n" characters from the temporary string
        REMOVERN(temp);

        node->next = NULL;

        // Used for breaking the string by the commas
        char* token;

        if (strlen(temp) > 0) {

            // Animal ID            
            token = strtok(temp, ",");
            node->animalID = strtol(token, NULL, 10);

            // Name
            token = strtok(NULL, ",");
            node->name = (char*) calloc((strlen(token) + 1), sizeof (char));
            if (node->name == NULL) {
                printf("Cannot allocate memory for the animal's name!\n");
                free(node);
                return NULL;
            }
            strcpy(node->name, token);

            // Sex
            token = strtok(NULL, ",");
            node->sex = token[0];

            // Quantity
            token = strtok(NULL, ",");
            node->quantity = strtol(token, NULL, 10);

            // Location
            token = strtok(NULL, ",");
            node->location = (char*) calloc((strlen(token) + 1), sizeof (char));
            if (node->location == NULL) {
                printf("Cannot allocate memory for the animal's location!\n");
                free(node->name);
                free(node);
                return NULL;
            }
            strcpy(node->location, token);

            return node;
        } else {
            free(node);
            return NULL;
        }
    } else {
        return NULL;
    }
}

// Display contents of linked list

void displayList(animal_t* head) {
    if (head == NULL) {
        printf("Linked List is empty!\n");
    } else {
        animal_t* current = head;
        while (current != NULL) {
            printf("%d - %s - %c - %d - %s\n", current->animalID, current->name, current->sex, current->quantity, current->location);
            current = current->next;
        }
    }
}

void displayListBrief(animal_t* head) {
    if (head == NULL) {
        printf("Linked List is empty!\n");
    } else {
        animal_t* current = head;
        puts("\n");
        while (current != NULL) {
            printf("%d) %s (%c)\n", current->animalID, current->name, current->sex);
            current = current->next;
        }
        puts("\n");
    }
}

// Free linked list & contents of nodes

void deleteList(animal_t** head) {
    animal_t* current = *head;
    animal_t* nextLocation = NULL;
    while (current != NULL) {
        nextLocation = current->next;
        free(current->name);
        free(current->location);
        free(current);
        current = nextLocation;
    }
}

void printToFile(FILE* fp, animal_t* head) {\
    if (head == NULL) {
        printf("Linked List is empty! Cannot print to file.\n");
    } else {
        animal_t* current = head;
        while (current != NULL) {
            fprintf(fp, "%d,%s,%c,%d,%s\r\n", current->animalID, current->name, current->sex, current->quantity, current->location);
            current = current->next;
        }
    }
}