#include <stdio.h>
#include <stdlib.h>
#include "zooAnimals.h"
#define FILENAME "dataNEW.txt"

/*
 * To-Do List:
 * 1) Range Searching
 * 2) Put more data entries in the file
 * 3) Fix edit record function (adds new record)
 */

// 1) we have to note down the exact number of nodes

int main() {        
    // Opens text file for reading (r+)
    FILE *fpStart = fopen(FILENAME, "r+");
    if (fpStart == NULL) {
        printf("Error reading file.\n");
        exit(1);
    }

    // Get total number of nodes (used for the Animal ID)
    int lastId = getLastId(fpStart);

    // Initialize queue (used for storing available animalId after deletion)
    // Only used by add + delete functions
    queue_t* queue = initQueue();

    // Create linked list from file
    animal_t* list = createList(fpStart, queue);
    fclose(fpStart);
    
    int choice;
    char temp[MAX_LEN] = {0};

    do {
        puts("=============== Welcome to the Zoo Gallery ===============");
        puts("----------------------- MAIN MENU ----------------------");
        puts(" 1) SHOW ALL RECORDS / VIEW DETAILS");
        puts(" 2) ADD RECORD");
        puts(" 3) EDIT RECORD");
        puts(" 4) DELETE RECORD");
        puts(" 5) SEARCH RECORD");
        puts("-1) EXIT + SAVE");
        puts("----------------------------------------------------------");
        printf("Please select from one of the following options: ");
        FLUSH;
        fgets(temp, MAX_LEN, stdin);
        choice = strtol(temp, NULL, 10);
        
        clearScreen();
        // Switch statements do not test for negative numbers
        if (choice == -1) {
            printf("\nThanks for using our app! Data has been saved to file!\n");
        }

        switch (choice) {
            case 1:
                displayListBrief(list);
                if (list != NULL) {
                    searchById(&list);
                    clearScreen();
                }
                break;
            case 2:                
                addRecord(&list, queue, &lastId);
                break;
            case 3:
                editRecord(&list);
                break;
            case 4:
                deleteRecord(&list, queue);
                break;
            case 5:
                searchOptions(list);
                break;
        }

    } while (choice != -1);

    // Overwrite contents of file (w+)
    FILE *fpEnd = fopen(FILENAME, "w+");
    if (fpEnd == NULL) {
        printf("Error reading file.\n");
        exit(1);
    }
    printToFile(fpEnd, list, lastId);
    fclose(fpEnd);

    // Free linked list & contents of nodes
    deleteList(&list);

    return 0;
}

