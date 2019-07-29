#include <stdio.h>
#include <stdlib.h>
#include "zooAnimals1.h"





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

