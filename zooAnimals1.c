#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
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
    puts("\n========== SHOW ALL RECORDS ==========");
    if (head == NULL) {
        printf("Linked List is empty!\n");
    } else {
        animal_t* current = head;
        while (current != NULL) {
            printf("%d) %s (%c)\n", current->animalID, current->name, current->sex);
            current = current->next;
        }
    }
}

// Search an animal by its AnimalID

void searchById(animal_t* head) {
    puts("\n========== SEARCH BY ANIMAL ID ==========");
    int recordId;
    do {
        printf("Enter the Animal ID of record to SEARCH (-1 to exit): ");
        FLUSH;
        scanf("%d", &recordId);

        if (recordId > 0) {
            viewRecord(head, recordId);
        } else if (recordId <= 1 && recordId != -1) {
            puts("Please enter an Animal ID greater than 0");
        }
    } while (recordId != -1);
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

void printToFile(FILE* fp, animal_t* head) {
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

    // Allocate memory for new node
    animal_t* newNode = (animal_t*) malloc(sizeof (animal_t));
    if (newNode == NULL) {
        printf("Cannot allocate memory for this animal!\n");
        return;
    }

    int animalId, quantity;
    char sex;
    char temp[MAX_LEN] = {0};

    // Ask user for animal ID
    bool isValid = false;
    do {
        printf("Enter Animal ID for new animal record: ");
        FLUSH;
        scanf("%d", &animalId);

        if (animalId > 0) {
            isValid = true;
            animal_t* current = *head;
            while (current != NULL) {
                if (current->animalID == animalId) {
                    puts("This animal ID is already taken. Please try again.");
                    isValid = false;
                    break;
                }
                current = current->next;
            }
        } else {
            puts("Please enter an Animal ID greater than 0");
        }
    } while (!isValid || animalId <= 0);
    newNode->animalID = animalId;

    // Ask user for animal name
    printf("Enter the name of the animal: ");
    FLUSH;
    fgets(temp, MAX_LEN, stdin);
    REMOVEN(temp);
    newNode->name = (char*) calloc(strlen(temp) + 1, sizeof (char));
    if (newNode->name == NULL) {
        printf("Cannot allocate memory for this animal's name!\n");
        return;
    }
    strToUppercase(temp);
    strcpy(newNode->name, temp);

    // Ask user for animal's sex
    do {
        printf("Enter the sex of the animal (M/F): ");
        FLUSH;
        sex = toupper(getchar());
        if (sex != 'M' && sex != 'F') {
            printf("Please state 'M' or 'F'\n");
        }
    } while (sex != 'M' && sex != 'F');
    newNode->sex = sex;

    // Ask user for animal quantity
    do {
        printf("Enter the quantity of the animal: ");
        FLUSH;
        scanf("%d", &quantity);

        if (quantity <= 0) {
            printf("Quantity must be greater than 0!\n");
        }
    } while (quantity <= 0);
    newNode->quantity = quantity;


    // Ask user for animal location
    printf("Enter the location of the animal: ");
    FLUSH;
    fgets(temp, MAX_LEN, stdin);
    REMOVEN(temp);
    newNode->location = (char*) calloc(strlen(temp) + 1, sizeof (char));
    if (newNode->location == NULL) {
        printf("Cannot allocate memory for this animal's location!\n");
        return;
    }
    strToUppercase(temp);
    strcpy(newNode->location, temp);

    // Prints current information of new animal
    printf("Here are the information for the new animal:\n ");
    printf("\n---------------------------------------------\n");
    printf("Animal ID:\t%d\n", newNode->animalID);
    printf("Name:\t\t%s\n", newNode->name);
    printf("Sex:\t\t%c\n", newNode->sex);
    printf("Quantity:\t%d\n", newNode->quantity);
    printf("Location:\t%s", newNode->location);
    printf("\n---------------------------------------------\n\n");


    // Confirms with the user if they want to inser tthe new record
    char choice;
    do {
        printf("CONFIRMATION: Would you like to add this new record (Y/N)? ");
        FLUSH;
        scanf("%c", &choice);
        choice = toupper(choice);

        if (choice == 'Y') {
            // If there's a record with the exact same name, sex, and location, then just INCREMENT the quantity
            animal_t* current = *head;
            bool recordExists = false;

            // Creates a temporary UPPERCASED version of the name + location (used for comparison)            
            while (current != NULL) {
                if (current->sex == newNode->sex &&
                        strcmp(current->name, newNode->name) == 0 &&
                        strcmp(current->location, newNode->location) == 0) {

                    printf("This exact record already exists.\n***Quantity of existing animal is incremented.***\n\n");
                    current->quantity += quantity;
                    recordExists = true;
                    break;
                }
                current = current->next;
            }

            // If animal doesn't exist in the list, then add it to the list
            if (!recordExists) {
                insertToList(head, newNode);
                printf("This record has been added to the list\n\n");
            }
        } else if (choice == 'N') {
            printf("The record will NOT be inserted!\n\n");
            free(newNode->name);
            free(newNode->location);
            free(newNode);
        }
    } while (choice != 'Y' && choice != 'N');
}

// Converts a string to uppercase

void strToUppercase(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
    }
}

// Inserts node into the Linked List in order of the AnimalID

void insertToList(animal_t** head, animal_t* node) {
    // insert as first:
    if (*head == NULL || node->animalID <= (*head)->animalID) {
        printf("\nHere\n");
        node->next = *head;
        *head = node;
    } else {
        animal_t* current = *head;
        while (current->next != NULL && (node->animalID > current->next->animalID)) {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
        printf("\nThere\n");
    }
}

void deleteRecord(animal_t** head) {
    puts("\n========== DELETE RECORD ==========");
    int recordId;
    char choice;
    do {
        printf("Enter the Animal ID of record to DELETE (-1 to exit): ");
        FLUSH;
        scanf("%d", &recordId);

        if (recordId > 0) {
            if (viewRecord(*head, recordId)) {
                do {
                    printf("CONFIRMATION: Would you like to DELETE this record (Y/N)? ");
                    FLUSH;
                    scanf("%c", &choice);
                    choice = toupper(choice);

                    if (choice == 'Y') {

                        if (recordId == 1) {
                            // if recordId = 1, then delete the head node
                            animal_t* toDelete = *head;
                            *head = (*head)->next;
                            free(toDelete);
                            printf("The record has been deleted successfully!\n\n");
                        } else {
                            // delete records AFTER the head node
                            animal_t* current = *head;
                            animal_t* toDelete = NULL;

                            // Moves to the node BEFORE the specified node and keeps track of it
                            while (current != NULL) {
                                if (current->next->animalID == recordId) {
                                    break;
                                }
                                current = current->next;
                            }
                            
                            viewRecord(*head, current->animalID);
                            
                            // Saves node to be deleted (so that we can free it)
                            toDelete = current->next;

                            // Check if node is LAST or not
                            if (toDelete->next == NULL) {
                                current->next = NULL;
                            } else {
                                // Sets current node to point to the node AFTER
                                current->next = toDelete->next;
                            }

                            free(toDelete);
                            printf("The record has been deleted successfully!\n\n");
                        }

                    } else if (choice == 'N') {
                        printf("The record will NOT be deleted!\n\n");
                    }

                } while (choice != 'Y' && choice != 'N');
            }
        } else if (recordId <= 1 && recordId != -1) {
            puts("Please enter an Animal ID greater than 0");
        }
    } while (recordId != -1);


}

// Displays specific records (returns true if record is found, else return false)
bool viewRecord(animal_t* head, int animalId) {
    if (head == NULL) {
        printf("Linked List is empty!\n");
    } else {
        animal_t* current = head;
        while (current != NULL) {
            if (current->animalID == animalId) {
                printf("\n---------------------------------------------\n");
                printf("Animal ID:\t%d\n", current->animalID);
                printf("Name:\t\t%s\n", current->name);
                printf("Sex:\t\t%c\n", current->sex);
                printf("Quantity:\t%d\n", current->quantity);
                printf("Location:\t%s", current->location);
                printf("\n---------------------------------------------\n\n");

                return true;
            }
            current = current->next;
        }

        printf("This record does not exist.\n\n");
        return false;
    }
}

