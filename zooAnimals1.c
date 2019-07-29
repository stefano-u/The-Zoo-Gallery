#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "zooAnimals1.h"

#define MAX_LEN 100
#define FLUSH stdin=freopen(NULL,"r",stdin)
#define REMOVERN(str) str[strcspn(str,"\r\n")]=0
#define REMOVEN(str) str[strcspn(str,"\n")]=0


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

// Displays 3 fields to the screen
void displayListBrief(animal_t* head) {
    if (head == NULL) {
        printf("Linked List is empty!\n");
    } else {
        animal_t* current = head;
        puts("\n========== SHOW ALL RECORDS ==========");
        while (current != NULL) {
            printf("%d) %s (%c)\n", current->animalID, current->name, current->sex);
            current = current->next;
        }
        viewRecord(head);
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

// Prints all contents of the linked list to the file
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

// Add a new record 
void addRecord(animal_t** head) {
    puts("\n========== ADD NEW RECORD ==========");
    animal_t* newNode = (animal_t*) malloc(sizeof (animal_t));
    if (newNode == NULL) {
        printf("Cannot allocate memory for this animal!\n");
        return;
    }
    
    int animalID, quantity;
    char sex;
    char temp[MAX_LEN] = {0};
    
    do {
        printf("Enter Animal ID for new animal record: ");
        scanf("%d", &animalId);

        bool isValid = true;
        // check if record is valid
        animal_t* current = *head;
        while (current != NULL) {
            if (current->animalID == animalId) {
                puts("This animal ID is already taken. Please try again.");
                isValid = false;
                break;
            }
            current = current->next;
        }
    } while (!isValid);

    
    printf("Enter the name of the animal: ");
    fgets(temp, MAX_LEN, stdin);
    newNode->name = (char*) calloc(strlen(temp)+1, sizeof(char));
    if (newNode->name == NULL) {
        printf("Cannot allocate memory for this animal's name!\n");
        return;
    }
    
    strcpy(newNode->name, temp);
    // if EXACTLY same NAME, SEX, and LOCATION, just increment that specific one
    
    
    animal_t* current = *head;
}

// Displays specific records
void viewRecord(animal_t* head) {
    puts("\n========== VIEW SPECIFIC RECORD ==========");
    printf("Enter the Animal ID to view details of that record (-1 to exit): ");
    int recordId;
    FLUSH;
    scanf("%d", &recordId);   

    while (recordId != -1) {
        animal_t* current = head;

        bool recordExists = false;
        while (current != NULL) {
            if (current->animalID == recordId) {
                printf("\n---------------------------------------------\n");
                printf("Animal ID:\t%d\n", current->animalID);
                printf("Name:\t\t%s\n", current->name);
                printf("Sex:\t\t%c\n", current->sex);
                printf("Quantity:\t%d\n", current->quantity);
                printf("Location:\t%s", current->location);
                printf("\n---------------------------------------------\n\n");
                
                recordExists = true;
                break;
            }
            current = current->next;
        }
        if (!recordExists) {
            printf("This record does not exist.\n\n");
        }
        
        printf("Enter the Animal ID to view details of that record (-1 to exit): ");
        FLUSH;
        scanf("%d", &recordId);
    }
    puts("");
}

