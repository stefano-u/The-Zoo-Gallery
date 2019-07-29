#include <stdio.h>
#include <stdlib.h>
#include "zooAnimals1.h"

#define FLUSH stdin=freopen(NULL,"r",stdin)


int main() {
    // Opens text file for reading/writing
    FILE *fp = fopen("dataNEW.txt", "r+");
    if (fp == NULL) {
        printf("Error reading file.\n");
        exit(1);
    }

    // Create linked list from file
    animal_t* list = createList(fp);


    puts("========== Welcome to the Zoo Gallery ==========\n");
    int choice;

    do {
        puts("Please select from one of the following options:");
        puts("'1' to show all records and search for a specific record");
        puts("'2' to add a record");
        puts("'3 to edit a record");
        puts("'4' to delete a record");
        puts("'5' to search for a record");
        puts("'-1' to quit");
        puts("----------------------------------------------------------");
        FLUSH;
        scanf("%d", &choice);

        // Switch statements do not use cases for negative numbers
        if (choice == -1) {
            printf("\nThanks for using our app!\n");
            break;
        }
        
        switch (choice) {
            case 1:
                displayListBrief(list);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            default:
                printf("Invalid choice! Please try again!");

        }

    } while (choice != 0);

    // Close file pointer
    fclose(fp);

    // Print to file
    printToFile(fp, list);

    // Free linked list & contents of nodes
    deleteList(&list);

    return 0;
}

