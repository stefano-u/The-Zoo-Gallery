#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "zooAnimals.h"

// ============================= Linked List Operations ==============================
/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Creates a linked list based on data on input file
 */
int getTotalNodes(FILE* fp) {
    char temp[MAX_LEN] = {0};
    fgets(temp, sizeof (temp), fp);
    char* token = strtok(temp, ",");
    return strtol(token, NULL, 10);
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Creates a linked list based on data on input file
 */
animal_t* createList(FILE *fp) {
    animal_t *new_node = NULL, *head = NULL;

    while ((new_node = createNodeFromFile(fp)) != NULL) {
        insertToList(&head, new_node);
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
void insertToList(animal_t** head, animal_t* new_node) {
    // List is empty
    if (*head == NULL) {
        new_node->prev = new_node; // Only element is also last element
        *head = new_node;
    
    // List is not empty, but new node goes at beginning
    } else if (new_node->animalID <= (*head)->animalID) {
        new_node->prev = (*head)->prev; // Carry over last element
        (*head)->prev = new_node;
        new_node->next = *head;
        *head = new_node;
    
    // List is not empty, new node goes in middle or end
    } else {
        animal_t* current = *head;
        while (current->next != NULL && (new_node->animalID > current->next->animalID)) {
            current = current->next;
        }
        
        // Node goes at end of list
        if (current->next == NULL) {
            current->next = new_node;
            new_node->prev = current;
            (*head)->prev = new_node; // Update last element
        
        // Node goes in middle of list after current node
        } else {
            current->next->prev = new_node;
            new_node->next = current->next;
            new_node->prev = current;
            current->next = new_node;
        }
    }
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Deletes a node from the linked list
 */
void deleteNode(animal_t** head, animal_t* animal) {
    
    // Animal doesn't exist
    if (animal == NULL) {
        puts("This animal does not exist!\n");
        return;
        
    // Only one animal in the list
    } else if (animal == *head) {
        free(animal);
        *head = NULL;
        
    // More than one animal in the list
    } else {
        animal_t* temp = animal->prev;
        
        // Update next and previous values for adjacent nodes
        if (animal->next == NULL) { // Last element
            temp->next = NULL;
            (*head)->prev = temp;
        } else {
            temp->next = animal->next;
            animal->next->prev = temp;
        }
        free(animal);   
    }
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Binary search for a specific ID (returns -1 if NOT found)
 * Reference: https://www.quora.com/What-is-a-C-program-binary-search-using-link-list
 */
animal_t* binarySearch(animal_t** head, int key) {
    animal_t* start = *head;
    animal_t* end = (*head)->prev;
    animal_t* middle;

    if (key > 0) {
        while (start->animalID <= end->animalID) {
            middle = findMiddleNode(start, end);
            if (key == middle->animalID) {
                return middle;
            } else if (start == end && key != middle->animalID) {
                return NULL;
            }else if (key < middle->animalID) {
                end = middle;
            } else {
                start = middle->next;
            }
        };
    }
    return NULL;
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Search for the middle element between the HEAD and TAIL
 * Reference: https://cs.stackexchange.com/questions/90777/how-to-find-middle-element-of-doubly-linked-list-using-head-and-tail
 */
animal_t* findMiddleNode(animal_t* head, animal_t* tail) {  
    // Loop until pointers meet in the middle
    while (head->animalID < tail->animalID) {
        // Move the tail pointer backward
        tail = tail->prev;
        
        // Move the head pointer forward
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

// ================================ Queue Operations =================================
/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Initialize the queue
 */
queue_t* initQueue() {
    queue_t* queue = (queue_t*) malloc(sizeof (queueNode_t));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Check if queue is empty
 */
bool isQueueEmpty(queue_t* queue) {
    return queue->head == NULL;
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Adds a new element to the queue
 */
void enqueue(queue_t* queue, int animalId) {
    queueNode_t* node = (queueNode_t*) malloc(sizeof (queueNode_t));
    node->availableID = animalId;
    node->next = NULL;
    
    if (isQueueEmpty(queue)) {
        // Sets node as the first element in the queue
        queue->head = node;
        queue->tail = node;
    } else {
        // Puts node at the end if the queue is NOT empty
        queue->tail->next = node;
        queue->tail = node;
    }
}

/* ===================================================================================
 * Author: Stefano Gregor Unlayao
 * Description: Remove first element in the queue
 */
int dequeue(queue_t* queue) {
    if (isQueueEmpty(queue)) {
        printf("Queue is empty!\n");
        return -1;
    }
    int animalId = queue->head->availableID;
    queueNode_t* toBeDequeued = queue->head;

    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(toBeDequeued);
    return animalId;
}

// ============================= "Database" Operations ===============================
/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Display 3 fields to the screen
 */
void displayListBrief(animal_t* head) {
    puts("\n========== SHOW ALL RECORDS ==========");
    if (head == NULL) {
        puts("Linked List is empty!\n");
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
 * Description: Displays record information for a specific animal.
 */
void showRecord(animal_t* current) {
    if (current == NULL) {
        puts("This animal does not exist!\n");
    } else {
        printf("\n---------------------------------------------\n");
        printf("Animal ID:\t%d\n", current->animalID);
        printf("Name:\t\t%s\n", current->name);
        printf("Sex:\t\t%c\n", current->sex);
        printf("Quantity:\t%d\n", current->quantity);
        printf("Location:\t%s", current->location);
        printf("\n---------------------------------------------\n\n");
    }
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Adds new node to linked list (with validation)
 */
void addRecord(animal_t** head, queue_t* queue, int* totalCount) {
    puts("\n========== ADD NEW RECORD ==========");
    char temp[MAX_LEN] = {0};

    // CREATE NEW ANIMAL
    // Allocate memory
    animal_t* newNode = (animal_t*) malloc(sizeof (animal_t));
    if (newNode == NULL) {
        printf("Cannot allocate memory for this animal!\n");
        exit(1);
    }

    // Set animal ID
    // Check the pool of free IDs
    if (!isQueueEmpty(queue)) {
        newNode->animalID = dequeue(queue);
    } else {
        newNode->animalID = ++(*totalCount);
    }

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

    printf("Details of new animal:\n ");
    showRecord(newNode);
    
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
    if (*head == NULL) {
        puts("There are no animals at the zoo!\n");
        return;
    }

    int recordId;
    char confirmChoice;
    int editChoice;
    char temp[MAX_LEN] = {0};

    // Continually asks user to enter an AnimalID to edit
    do {
        printf("Enter the Animal ID of record to EDIT (-1 to exit): ");
        FLUSH;

        fgets(temp, MAX_LEN, stdin);
        recordId = strtol(temp, NULL, 10);
        
        animal_t* toBeEdited = binarySearch(head, recordId);
        
        // Checks if animal ID is valid & existing
        if (recordId > 0 && toBeEdited != NULL) {

            // Create a new node to store animal information
            animal_t* newNode = (animal_t*) malloc(sizeof (animal_t));
            if (newNode == NULL) {
                printf("Cannot allocate memory for this animal!\n");
                exit(1);
            }
            newNode->next = NULL;
            newNode->prev = NULL;
            newNode->animalID = toBeEdited->animalID;
            newNode->sex = toBeEdited->sex;
            newNode->quantity = toBeEdited->quantity;

            char tempName[MAX_LEN] = {0};
            strcpy(tempName, toBeEdited->name);
            REMOVEN(tempName);
            newNode->name = tempName;
            
            char tempLocation[MAX_LEN] = {0};
            strcpy(tempLocation, toBeEdited->location);
            REMOVEN(tempLocation);
            newNode->location = tempLocation;
            
            while (1) {
                printf("\n---------------------------------------------\n");
                printf("Animal ID:\t%d\n", newNode->animalID);
                printf("1. Name:\t%s\n", newNode->name);
                printf("2. Sex:\t\t%c\n", newNode->sex);
                printf("3. Quantity:\t%d\n", newNode->quantity);
                printf("4. Location:\t%s", newNode->location);
                printf("\n---------------------------------------------\n\n");
                printf("\n\t>Please select a FIELD to edit (-1 to finish editing): ");
                FLUSH;
                fgets(temp, MAX_LEN, stdin);
                editChoice = strtol(temp, NULL, 10);
                
                if (editChoice == -1) {
                    break;
                }
                
                switch (editChoice) {
                    case 1:
                        setAnimalName(tempName);
                        break;
                    case 2:
                        newNode->sex = getSex();
                        break;
                    case 3:
                        newNode->quantity = getQuantity();
                        break;
                    case 4:
                        setAnimalLocation(tempLocation);
                        break;
                    default:
                        printf("Invalid choice! Please try again!");
                        break;
                }
            }

            // Confirms with the user if they want to edit the record
            do {
                printf("CONFIRMATION: Would you like to EDIT this record (Y/N)? ");
                FLUSH;
                scanf("%c", &confirmChoice);
                confirmChoice = toupper(confirmChoice);

                if (confirmChoice == 'Y') {
                    // Check if there's a record with the exact same name, sex, and location
                    bool recordExists = false;
                    animal_t* current = *head;

                    while (current != NULL) {
                        // ignore the existing node
                        if (current->animalID != toBeEdited->animalID &&
                                current->sex == newNode->sex &&
                                strcmp(current->name, tempName) == 0 &&
                                strcmp(current->location, tempLocation) == 0) {
                            printf("ERROR: A record with the exact same name, sex, and location already exists. \nRecord cannot be edited!\n\n");
                            free(newNode);
                            recordExists = true;
                            break;
                        }
                        current = current->next;
                    }

                    if (!recordExists) {
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

                        deleteNode(head, toBeEdited);
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
void deleteRecord(animal_t** head, queue_t* queue) {
    puts("\n========== DELETE RECORD ==========");
    int recordId;
    char choice;
    char temp[MAX_LEN] = {0};

    // Continually asks user to enter an AnimalID to delete
    do {
        if (*head == NULL) {
            puts("Linked List is empty!\n");
            return;
        }
        printf("Enter the Animal ID of record to DELETE (-1 to exit): ");
        FLUSH;
        fgets(temp, MAX_LEN, stdin);
        
        recordId = strtol(temp, NULL, 10);        
        animal_t* tbdeleted = binarySearch(head, recordId);
        
        // Checks if animal ID is valid & existing
        if (recordId > 0 && tbdeleted != NULL) {
            // Confirms with the user if they want to delete the record
            do {
                showRecord(tbdeleted);
                printf("CONFIRMATION: Would you like to DELETE this record (Y/N)? ");
                FLUSH;
                scanf("%c", &choice);
                choice = toupper(choice);

                if (choice == 'Y') {
                    enqueue(queue, recordId);
                    deleteNode(head, tbdeleted);
                    printf("The record has been deleted successfully!\n\n");
                } else if (choice == 'N') {
                    printf("The record will NOT be deleted!\n\n");
                }
            } while (choice != 'Y' && choice != 'N');
        } else {
            puts("Record does NOT exist!\n");
        }
    } while (recordId != -1);
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Gives the user the option to search by different methods
 */
void searchOptions(animal_t* head) {
    if (head == NULL) {
        puts("\n========== SEARCH OPTIONS ==========");
        puts("Linked List is empty!\n");
        return;
    }

    int choice;
    char temp[MAX_LEN] = {0};
    clearScreen();
    puts("\n========== SEARCH OPTIONS ==========");
    puts(" 1) ID");
    puts(" 2) NAME");
    puts("-1) EXIT");
    puts("----------------------------------------------------------");
    printf("Please select a search method: ");
    FLUSH;
    fgets(temp, MAX_LEN, stdin);
    choice = strtol(temp, NULL, 10);

    clearScreen();
    switch (choice) {
        case 1:
            searchById(&head);
            break;
        case 2:
            //searchByName(head);
            break;
        default: break;
    }
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Search an animal by its AnimalID
 */
void searchById(animal_t** head) {
    int recordId;
    char temp[MAX_LEN] = {0};
    do {
        puts("\n========== SEARCH BY ANIMAL ID ==========");
        printf("Enter the Animal ID of record to SEARCH (-1 to exit): ");
        FLUSH;
        fgets(temp, MAX_LEN, stdin);
        recordId = strtol(temp, NULL, 10);

        animal_t* temp = binarySearch(head, recordId);
        
        if (recordId > 0 && temp != NULL) {
            showRecord(temp);
        } else {
            puts("Record does NOT exist!\n");
        }
    } while (recordId != -1);
}

/*
void searchByName(animal_t* head) {
    char temp[MAX_LEN] = {0};
    animal_t* current = NULL;
    int recordId;
    int count = 0;
    do {
        puts("\n========== SEARCH BY NAME ==========");
        printf("Enter the NAME of animal to SEARCH (type 'EXIT' to exit): ");
        FLUSH;
        fgets(temp, MAX_LEN, stdin);
        REMOVEN(temp);
        strToUppercase(temp);

        if (strcmp(temp, "EXIT") == 0) {
            break;
        } else if (strlen(temp) > 0) {
            current = head;
            while (current != NULL) {
                if (strstr(current->name, temp) != NULL) {
                    printf("%d) %s (%c)\n", current->animalID, current->name, current->sex);
                    count++;
                }
                current = current->next;
            }
            puts("");

            if (count > 0) {
                do {
                    printf("Enter the Animal ID to VIEW RECORD (-1 to exit): ");
                    FLUSH;
                    fgets(temp, MAX_LEN, stdin);
                    recordId = strtol(temp, NULL, 10);
                    showRecord(head, recordId);
                } while (recordId != -1);
                clearScreen();
            } else {
                printf("No result for that name!\n");
            }

        } else if (strlen(temp) <= 0) {
            puts("Please enter at least 1 character\n");
        }

    } while (strcmp(temp, "EXIT") != 0);
}
*/
// ============================= Miscellaneous Operations ===============================
/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Clears the screen
 */
void clearScreen() {
    for (int i = 0; i < 25; i++) {
        puts("");
    }
}

/* ===================================================================================
 * Author: Anna Isabelle Ramos
 * Description: Prints contents of linked list to file
 */
void printToFile(FILE* fp, animal_t* head, int totalCount) {
    if (head == NULL) {
        puts("Linked List is empty! File is now empty!\n");
        fprintf(fp, "0\r\n");
    } else {
        fprintf(fp, "%d\r\n", totalCount);
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
/*
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
*/
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
 * Author: Anna Isabelle Ramos
 * Description: Ask user for the animal's name
 */
void setAnimalLocation(char* name) {
    int choice;
    char temp[MAX_LEN] = {0};
    clearScreen();
    puts("\n========== LOCATION OPTIONS ==========");
    puts(" 1) AFRICAN SAVANNAH");
    puts(" 2) ASIAN FALLS");
    puts(" 3) CAVE ENCLOSURE");
    puts(" 4) GRASSY PLAINS");
    puts(" 5) HIGH PLAINS");
    puts(" 6) ICEBERG LOUNGE");
    puts(" 7) KOALA OUTBACK");
    puts(" 8) PINEY WOODS");
    puts(" 9) PRIMATE EXHIBIT");
    puts(" 10) SWAMP EXHIBIT");
    puts(" 11) WILD EXHIBIT");
    puts("-1) ");
    puts("----------------------------------------------------------");
    printf("Enter the location of the animal: ");
    FLUSH;
    fgets(temp, MAX_LEN, stdin);
    choice = strtol(temp, NULL, 10);

    clearScreen();
    switch (choice) {
        case 1:
            strcpy(name, "AFRICAN SAVANNAH"); break;
        case 2:
            strcpy(name, "ASIAN FALLS"); break;
        case 3:
            strcpy(name, "CAVE ENCLOSURE"); break;
        case 4:
            strcpy(name, "GRASSY PLAINS"); break;
        case 5:
            strcpy(name, "HIGH PLAINS"); break;
        case 6:
            strcpy(name, "ICEBERG LOUNGE"); break;
        case 7:
            strcpy(name, "KOALA OUTBACK"); break;
        case 8:
            strcpy(name, "PINEY WOODS"); break;
        case 9:
            strcpy(name, "PRIMATE EXHIBIT"); break;
        case 10:
            strcpy(name, "SWAMP EXHIBIT"); break;
        case 11:
            strcpy(name, "WILD EXHIBIT"); break;
        default:
            break;
    }
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
    char temp[MAX_LEN] = {0};
    int quantity;
    do {
        printf("Enter the quantity of the animal: ");
        FLUSH;
        fgets(temp, MAX_LEN, stdin);
        quantity = strtol(temp, NULL, 10);

        if (quantity <= 0) {
            printf("Quantity must be greater than 0!\n");
        }
    } while (quantity <= 0);
    return quantity;
}
