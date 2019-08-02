#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "zooAnimals.h"

#define MAX_LEN 100
#define FLUSH stdin=freopen(NULL,"r",stdin)
#define REMOVERN(str) str[strcspn(str,"\r\n")]=0
#define REMOVEN(str) str[strcspn(str,"\n")]=0


// ============================= Linked List Operations ==============================

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Creates a linked list based on data on input file
 */
animal_t* createList(FILE *fp) {
    animal_t *node = NULL, *current = NULL, *head = NULL;

    // Creates nodes and puts it on the linked list
    while ((node = createNodeFromFile(fp)) != NULL) {
        if (head == NULL) {
            head = node;
        } else {
            current->next = node;
            node->prev = current;
        }
        current = node;
    }

    return head;
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Create linked list nodes based on input from file
 */
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
        node->prev = NULL;

        // Used for breaking the string by the commas
        char* token;

        // Input string must have at least 1 character
        if (strlen(temp) > 0) {

            // Parse Animal ID            
            token = strtok(temp, ",");
            node->animalID = strtol(token, NULL, 10);

            // Parse Name
            token = strtok(NULL, ",");
            node->name = (char*) calloc((strlen(token) + 1), sizeof (char));
            if (node->name == NULL) {
                printf("Cannot allocate memory for the animal's name!\n");
                free(node);
                return NULL;
            }
            strcpy(node->name, token);

            // Parse Sex
            token = strtok(NULL, ",");
            node->sex = token[0];

            // Parse Quantity
            token = strtok(NULL, ",");
            node->quantity = strtol(token, NULL, 10);

            // Parse Location
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

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Frees memory allocate for node's name + location, and the node itself
 */
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

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Inserts node into the Linked List by ascending order of the AnimalID
 */
void insertToList(animal_t** head, animal_t* node) {
    if (*head == NULL || node->animalID <= (*head)->animalID) {
        // Insert to the head of the linked list
        node->next = *head;
        *head = node;
    } else {
        // Inserts to the middle or end of the linked list
        animal_t* current = *head;
        while (current->next != NULL && (node->animalID > current->next->animalID)) {
            current = current->next;
        }
        // Connects next nodes
        node->next = current->next;
        current->next = node;
        
        // Connects previous nodes
        current->next->prev = node;
        node->prev = current;
    }
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Deletes a node from the linked list
 */
void deleteNode(animal_t** head, int recordId) {
    if (recordId == 1) {
        // if recordId = 1, then delete the head node
        animal_t* toDelete = *head;
        *head = (*head)->next;
        (*head)->prev = NULL;
        free(toDelete);
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

        // Saves node to be deleted (so that we can free it)
        toDelete = current->next;

        // Check if node is LAST or not
        if (toDelete->next == NULL) {
            current->next = NULL;
        } else {
            // Sets current node to point to the node AFTER
            current->next = toDelete->next;
            toDelete->next->prev = current;
        }

        free(toDelete);
    }
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Binary search for a specific ID (returns -1 if NOT found)
 * Reference: https://www.quora.com/What-is-a-C-program-binary-search-using-link-list
 */
int binarySearch(animal_t* head, int key) {

    animal_t* startNode = head;
    while (head->next != NULL) {
        head = head->next;
    }
    animal_t* endNode = head;
    animal_t* middleNode;
    if (key > 0) {
        do {
            middleNode = findMiddleNode(startNode, endNode);
            if (key == middleNode->animalID) {
                return middleNode->animalID;
            } else if (startNode == endNode && key != middleNode->animalID) {
                // If it's already at the last location and has not found that record, then that means it does NOT exist
                return -1;
            }
            if (key < middleNode->animalID) {
                endNode = middleNode;
            } else {
                startNode = middleNode->next;
            }
        } while (endNode->next != startNode);
    }
    // Key is not found
    return -1;
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Search for the middle element between the HEAD and TAIL
 * Reference: https://cs.stackexchange.com/questions/90777/how-to-find-middle-element-of-doubly-linked-list-using-head-and-tail
 */
animal_t* findMiddleNode(animal_t* head, animal_t* tail) {
    // Keep doing this until head == tail
    while (head != tail) {
        // Move the tail backward
        if (head != tail) {
            tail = tail->prev;
        }
        // Move the head forward
        if (head != tail) {
            head = head->next;
        }
    }
    return head;
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Returns the number of nodes in the list
 */
int getNumNodesFromList(animal_t* head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}






// ============================= "Database" Operations ===============================

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Display 3 fields to the screen
 */
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

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Displays specific record information 
 *              (returns true if record is found, else return false)
 */
void showRecord(animal_t* head, int animalId) {
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
            }
            current = current->next;
        }
    }
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Adds new node to linked list (with validation)
 */
void addRecord(animal_t** head) {
    puts("\n========== ADD NEW RECORD ==========");
    char temp[MAX_LEN] = {0};

    // Allocate memory for new node
    animal_t* newNode = (animal_t*) malloc(sizeof (animal_t));
    if (newNode == NULL) {
        printf("Cannot allocate memory for this animal!\n");
        exit(1);
    }
    newNode->next = NULL;
    newNode->prev = NULL;

    // Set animal's ID
    newNode->animalID = getAnimalId(*head);

    // Set animal's name
    setAnimalName(temp);
    newNode->name = (char*) calloc(strlen(temp) + 1, sizeof (char));
    if (newNode->name == NULL) {
        printf("Cannot allocate memory for this animal's name!\n");
        exit(1);
    }
    strcpy(newNode->name, temp);

    // Set animal's sex
    newNode->sex = getSex();

    // Set animal's quantity
    newNode->quantity = getQuantity();

    // Set animal's location
    setAnimalLocation(temp);
    newNode->location = (char*) calloc(strlen(temp) + 1, sizeof (char));
    if (newNode->location == NULL) {
        printf("Cannot allocate memory for this animal's location!\n");
        exit(1);
    }
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


    // Confirms with the user if they want to insert the new record
    char choice;
    do {
        printf("CONFIRMATION: Would you like to add this new record (Y/N)? ");
        FLUSH;
        scanf("%c", &choice);
        choice = toupper(choice);

        if (choice == 'Y') {
            animal_t* current = *head;
            bool recordExists = false;

            // If there's a record with the exact same name, sex, and location, then just INCREMENT the quantity         
            while (current != NULL) {
                if (current->sex == newNode->sex &&
                        strcmp(current->name, newNode->name) == 0 &&
                        strcmp(current->location, newNode->location) == 0) {

                    printf("This exact record already exists.\n"
                            "***Quantity of existing animal is incremented.***\n\n");
                    current->quantity += newNode->quantity;
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

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Edits node in the linked list (with validation)
 */
void editRecord(animal_t**head) {
    puts("\n========== EDIT RECORD ==========");
    int recordId;
    char confirmChoice;
    int editChoice;
    char temp[MAX_LEN] = {0};
    
    // Contiually asks user to enter an AnimalID to delete
    do {
        printf("Enter the Animal ID of record to EDIT (-1 to exit): ");
        FLUSH;
        
        fgets(temp, MAX_LEN, stdin);
        recordId = strtol(temp, NULL, 10);

        // Checks if animal ID is valid & existing
        if (recordId > 0  && binarySearch(*head, recordId) != -1) {
            animal_t* toBeEdited = NULL;
            animal_t* current = *head;
            while (current != NULL) {
                if (current->animalID == recordId) {
                    toBeEdited = current;
                    break;
                }
                current = current->next;
            }



            animal_t* newNode = (animal_t*) malloc(sizeof (animal_t));
            if (newNode == NULL) {
                printf("Cannot allocate memory for this animal!\n");
                exit(1);
            }
            newNode->next = NULL;
            newNode->animalID = toBeEdited->animalID;
            newNode->sex = toBeEdited->sex;
            newNode->quantity = toBeEdited->quantity;

            char tempName[MAX_LEN] = {0};
            strcpy(tempName, toBeEdited->name);
            REMOVEN(tempName);

            char tempLocation[MAX_LEN] = {0};
            strcpy(tempLocation, toBeEdited->location);
            REMOVEN(tempLocation);
            
            bool onlyIdChanged = true;


            do {
                printf("\n---------------------------------------------\n");
                printf("1) Animal ID:\t%d\n", newNode->animalID);
                printf("2) Name:\t%s\n", tempName);
                printf("3) Sex:\t\t%c\n", newNode->sex);
                printf("4) Quantity:\t%d\n", newNode->quantity);
                printf("5) Location:\t%s", tempLocation);
                printf("\n---------------------------------------------\n\n");
                printf("\nPlease select a FIELD to edit (-1 to finish editing): ");
                FLUSH;
                fgets(temp, MAX_LEN, stdin);
                editChoice = strtol(temp, NULL, 10);
                
                if (editChoice == -1) {
                    break;
                }

                switch (editChoice) {
                    case 1:
                        newNode->animalID = getAnimalId(*head);
                        break;

                    case 2:                        
                        setAnimalName(tempName);
                        onlyIdChanged = false;
                        break;

                    case 3:
                        newNode->sex = getSex();
                        onlyIdChanged = false;
                        break;

                    case 4:
                        newNode->quantity = getQuantity();
                        onlyIdChanged = false;
                        break;

                    case 5:
                        setAnimalLocation(tempLocation);
                        onlyIdChanged = false;
                        break;

                    default:
                        printf("Invalid choice! Please try again!");
                        break;

                }

            } while (editChoice != -1);


            // Confirms with the user if they want to delete the record
            do {
                printf("CONFIRMATION: Would you like to EDIT this record (Y/N)? ");
                FLUSH;
                scanf("%c", &confirmChoice);
                confirmChoice = toupper(confirmChoice);

                if (confirmChoice == 'Y') {
                    // Check if there's a record with the exact same name, sex, and location
                    bool recordExists = false;
                    current = *head;
                    while (current != NULL && !onlyIdChanged) {
                        if (current->sex == newNode->sex &&
                                strcmp(current->name, tempName) == 0 &&
                                strcmp(current->location, tempLocation) == 0) {
                            printf("This exact record already exists. Record cannot be edited!\n\n");
                            free(newNode);
                            recordExists = true;
                            break;
                        }
                        current = current->next;
                    }

                    if (!recordExists || onlyIdChanged) {
                        newNode->name = (char*) calloc(strlen(tempName) + 1, sizeof (char));
                        if (newNode->name == NULL) {
                            printf("Cannot allocate memory for this animal's name!\n");
                            exit(1);
                        }
                        strcpy(newNode->name, tempName);

                        newNode->location = (char*) calloc(strlen(tempLocation) + 1, sizeof (char));
                        if (newNode->location == NULL) {
                            printf("Cannot allocate memory for this animal's location!\n");
                            exit(1);
                        }
                        strcpy(newNode->location, tempLocation);

                        deleteNode(head, toBeEdited->animalID);
                        insertToList(head, newNode);
                        printf("The record has been edited successfully!\n\n");
                    }
                    


                } else if (confirmChoice == 'N') {
                    free(newNode);
                    printf("The record will NOT be edited!\n\n");
                }
            } while (confirmChoice != 'Y' && confirmChoice != 'N');
        } else if (recordId > 0 && binarySearch(*head, recordId) == -1) {
            puts("Record does NOT exist!\n"); 
        } else if (recordId <= 1 && recordId != -1) {
            puts("Please enter an Animal ID greater than 0\n");
        }

    } while (recordId != -1);
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Deletes node in the linked list (with validation)
 */
void deleteRecord(animal_t** head) {
    puts("\n========== DELETE RECORD ==========");
    int recordId;
    char choice;

    // Contiually asks user to enter an AnimalID to delete
    do {
        printf("Enter the Animal ID of record to DELETE (-1 to exit): ");
        FLUSH;
        scanf("%d", &recordId);

        // Checks if animal ID is valid & existing
        if (recordId > 0 && binarySearch(*head, recordId) != -1) {
            // Confirms with the user if they want to delete the record
            do {
                printf("CONFIRMATION: Would you like to DELETE this record (Y/N)? ");
                FLUSH;
                scanf("%c", &choice);
                choice = toupper(choice);

                if (choice == 'Y') {
                    deleteNode(head, recordId);
                    printf("The record has been deleted successfully!\n\n");
                } else if (choice == 'N') {
                    printf("The record will NOT be deleted!\n\n");
                }
            } while (choice != 'Y' && choice != 'N');

        } else if (recordId > 0 && binarySearch(*head, recordId) == -1) {
            puts("Record does NOT exist!\n");
        } else if (recordId <= 1 && recordId != -1) {
            puts("Please enter an Animal ID greater than 0\n");
        }

    } while (recordId != -1);
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Search an animal by its AnimalID
 */
void searchById(animal_t* head) {
    puts("\n========== SEARCH BY ANIMAL ID ==========");
    int recordId;
    do {
        printf("Enter the Animal ID of record to SEARCH (-1 to exit): ");
        FLUSH;
        scanf("%d", &recordId);

        if (recordId > 0 && binarySearch(head, recordId) != -1) {
            showRecord(head, recordId);
        } else if (recordId > 0 && binarySearch(head, recordId) == -1) {
            puts("Record does NOT exist!\n");
        } else if (recordId <= 1 && recordId != -1) {
            puts("Please enter an Animal ID greater than 0\n");
        }
    } while (recordId != -1);
}











// ============================= Miscellaneous Operations ===============================

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Prints contents of linked list to file
 */
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

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Converts a string to uppercase
 */
void strToUppercase(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        string[i] = toupper(string[i]);
    }
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Ask user for the animal's ID (with validation)
 */
int getAnimalId(animal_t* head) {
    // Ask user for animal ID (must be greater than 0)
    int animalId;
    char temp[MAX_LEN] = {0};
    do {
        printf("Enter the Animal ID for the animal: ");
        FLUSH;        
        fgets(temp, MAX_LEN, stdin);
        animalId = strtol(temp, NULL, 10);

        // Checks if the animal ID used in an existing record
        if (animalId > 0 && binarySearch(head, animalId) != -1) {
            puts("This animal ID is already taken. Please try again.\n");
        } else if (animalId <= 0) {
            puts("Please enter an Animal ID greater than 0.\n");
        }
    } while (animalId <= 0 || binarySearch(head, animalId) != -1);
    
    return animalId;
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Ask user for the animal's name
 */
void setAnimalName(char* name) {    
    do {
        printf("Enter the name of the animal: ");
        FLUSH;
        fgets(name, MAX_LEN, stdin);
        REMOVEN(name);
        if (strlen(name) <= 0) {
            puts("Please enter a name with at least 1 character!\n");
        }
    } while (strlen(name) <= 0);

    // Converts string to uppercase
    strToUppercase(name);
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Ask user for the animal's name
 */
void setAnimalLocation(char* name) {    
    do {
        printf("Enter the location of the animal: ");
        FLUSH;
        fgets(name, MAX_LEN, stdin);
        REMOVEN(name);
        if (strlen(name) <= 0) {
            puts("Please enter a location with at least 1 character!\n");
        }
    } while (strlen(name) <= 0);

    // Converts string to uppercase
    strToUppercase(name);
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Ask user for the animal's sex (with validation)
 */
char getSex() {
    // Ask user for animal's sex
    char sex;
    do {
        printf("Enter the sex of the animal (M/F): ");
        FLUSH;
        // Converts char to uppercase
        sex = toupper(getchar());

        if (sex != 'M' && sex != 'F') {
            printf("Please state 'M' or 'F'\n");
        }
    } while (sex != 'M' && sex != 'F');
    return sex;
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Ask user for the animal's quantity (with validation)
 */
int getQuantity() {
    int quantity;
    do {
        printf("Enter the quantity of the animal: ");
        FLUSH;
        scanf("%d", &quantity);
        if (quantity <= 0) {
            printf("Quantity must be greater than 0!\n");
        }
    } while (quantity <= 0);
    return quantity;
}
