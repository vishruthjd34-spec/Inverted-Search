#include "inverted.h"

#define HASH_TABLE_SIZE 27

int main(int argc, char *argv[])
{
    F_node *head = NULL;          /* Linked list of validated input files */
    hash_t HT[HASH_TABLE_SIZE];   /* Hash table that backs the inverted index */

    /* No arguments at all -> nothing to do, show usage and quit. */
    if (argc < 2)
    {
        printf("\033[1;34mERROR : No input file(s) supplied.\n\033[0m");
        print_usage(argv[0]);
        return FAILURE;
    }

    /*     Explicit help request -> print usage and exit cleanly.
     *     Checked before any file validation so "-h" is never mistaken
     *     for a file name.                                              */
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        print_usage(argv[0]);
        return SUCCESS;
    }

    /*     Validate every remaining argument as a candidate input file.
     *     validate_files() checks extension, existence, emptiness and
     *     duplicates, and links the good ones into "head".              */
    if (validate_files(argc, argv, &head) == SUCCESS)
    {
        printf("\033[1;32mvalidation SUCCESSFUL\n\n\033[0m");
    }
    else
    {
        printf("\033[1;34mvalidation UNSUCCESSFUL : none of the supplied "
               "arguments resulted in a usable file.\n\n\033[0m");
        print_usage(argv[0]);
        return FAILURE;
    }

    /* Show the user exactly which files were accepted into the database. */
    print_filenames(head);

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HT[i].index = i;
        HT[i].main_link = NULL;
    }

    int cflag = 0, uflag = 0;
    int option;
    do
    {
        printf("Select your choice among following operations:\n"
               "1. Create Database\n"
               "2. Display Database\n"
               "3. Save Database\n"
               "4. Search\n"
               "5. Update Database\n"
               "6. Exit\n\n"
               "Enter your choice : ");

        scanf("%d", &option);

        switch (option)
        {
            case 1:
                /* Build the inverted index from the validated files,
                 * but only once.                                        */
                if (cflag == 0)
                {
                    create_database(HT, head);
                    cflag = 1;
                }
                else
                {
                    printf("\033[1;34mdatabase is already created !!!!!\n\n\033[0m");
                }
                break;

            case 2:
                /* Print the current state of the inverted index.        */
                display_database(HT);
                break;

            case 3:
                /* Write the current inverted index out to a backup file.*/
                save_database(HT);
                break;

            case 4:
                /* Look up a single word typed in by the user.           */
                search_database(HT);
                break;

            case 5:
                if (cflag == 0 && uflag == 0)
                {
                    if (update_database(HT, &head) == SUCCESS)
                    {
                        uflag = 1;
                    }
                    else
                    {
                        printf("\033[1;34mINFO : Enter the valid backup file\n\n\033[0m");
                    }
                }
                else
                {
                    printf("\033[1;34mdatabase is already created or updated !!!!\n\n\033[0m");
                }
                print_filenames(head);
                break;

            case 6:
                /* Exit -> handled by the while() condition below.       */
                break;

            default:
                printf("\033[1;34mINFO : Please enter the valid option\n\n\033[0m");
        }
    } while (option != 6);

    return SUCCESS;
}
