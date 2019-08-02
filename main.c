#include <stdio.h>
#include <stdlib.h>
#include "zooAnimals.h"

#define FILENAME "dataNEW.txt"
#define FLUSH stdin=freopen(NULL,"r",stdin)

/*
 * To-Do List:
 * 1) SEARCH for a record 
 *      - Partial searching?
 *      - Range searching?
 * 
 */

int main() {
    // Opens text file for reading (r+)
    FILE *fpStart = fopen(FILENAME, "r+");
    if (fpStart == NULL) {
        printf("Error reading file.\n");
        exit(1);
    }
    
    // Create linked list from file
    animal_t* list = createList(fpStart);
    fclose(fpStart);

    puts("========== Welcome to the Zoo Gallery ==========\n");
    int choice;

    do {
        puts("\n----------------------- MAIN MENU ----------------------");
        puts(" 1) SHOW ALL RECORDS / VIEW DETAILS");
        puts(" 2) ADD RECORD");
        puts(" 3) EDIT RECORD");
        puts(" 4) DELETE RECORD");
        puts(" 6) SEARCH RECORD");
        puts("-1) EXIT + SAVE");
        puts("----------------------------------------------------------");
        puts("Please select from one of the following options:");
        FLUSH;
        scanf("%d", &choice);

        // Switch statements do not test for negative numbers
        if (choice == -1) {
            printf("\nThanks for using our app! Data has been saved to file!\n");
            break;
        }
        
        switch (choice) {
            case 1:
                displayListBrief(list);
                searchById(list);
                break;
            case 2:
                addRecord(&list);
                break;
            case 3:
                editRecord(&list);
                break;
            case 4:
                deleteRecord(&list);
                break;
            case 5:
                break;
            default:
                printf("Invalid choice! Please try again!");
                break;

        }

    } while (choice != -1);

    // Overwrite contents of file (w+)
    FILE *fpEnd = fopen(FILENAME, "w+");
    if (fpEnd == NULL) {
        printf("Error reading file.\n");
        exit(1);
    }
    printToFile(fpEnd, list);
    fclose(fpEnd);
    
    
    // Free linked list & contents of nodes
    deleteList(&list);

    return 0;
}

