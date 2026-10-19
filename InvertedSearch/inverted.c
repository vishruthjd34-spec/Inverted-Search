#include "inverted.h"
void print_usage(const char *prog_name)
{
    printf("\033[1;36m");
    printf("----------------------------------------------------------------\n");
    printf(" Inverted Search / Inverted Index -- Usage\n");
    printf("----------------------------------------------------------------\n");
    printf(" %s <file1.txt> [file2.txt] [file3.txt] ...\n", prog_name);
    printf(" Arguments:\n");
    printf("   file1.txt, file2.txt, ...   One or more non-empty .txt files\n");
    printf("                               to index. At least one is required.\n\n");
    printf("\033[0m\n");
}

int validate_files(int argc, char *argv[], F_node **head)
{
    /* Iterate using argc so the loop bound is explicit and self
     * documenting -- argv[1] through argv[argc - 1] are the user
     * supplied file names (argv[0] is the program name and is skipped).*/
    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");

        if (strstr(argv[i], ".") == NULL)
        {
            printf("\033[1;34mINFO : %s => this file has without extension\n\n\033[0m", argv[i]);
        }
        else if (strstr(argv[i], ".txt") == NULL)
        {
            printf("\033[1;34mINFO : %s => this file has not a .txt extension\n\n\033[0m", argv[i]);
        }
        else if (fp == NULL)
        {
            printf("\033[1;34mINFO : %s => this file has not created\n\n\033[0m", argv[i]);
        }
        else if (file_staus(fp) == FAILURE)
        {
            printf("\033[1;34mINFO : %s => this is empty\n\n\033[0m", argv[i]);
        }
        else if (check_duplicates(*head, argv[i]) == FAILURE)
        {
            printf("\033[1;34mINFO : %s => this is repeated, so it will not be stored into the list\n\n\033[0m", argv[i]);
        }
        else
        {
            insert_last(head, argv[i]);
            printf("\033[1;32mINFO : Successful : inserted file name %s into file linked list\n\n\033[0m", argv[i]);
        }

        /* Only close fp if fopen() actually succeeded -- closing a NULL
         * FILE* is undefined behaviour.                                */
        if (fp != NULL)
            fclose(fp);
    }

    /* At least one file must have made it into the list for the run to
     * be considered a valid invocation.                                */
    if (*head == NULL)
        return FAILURE;
    else
        return SUCCESS;
}

int file_staus(FILE *fp)
{
    fseek(fp, 0, SEEK_END);

    if (ftell(fp) == 0)
        return FAILURE;
    else
        return SUCCESS;
}

int insert_last(F_node **head, char *f_name)
{
    F_node *new = malloc(sizeof(F_node));

    strcpy(new->f_name, f_name);
    new->flink = NULL;

    if (*head == NULL)
    {
        /* List was empty -- new node becomes the head. */
        *head = new;
    }
    else
    {
        /* Walk to the last node, then attach the new node after it. */
        F_node *temp = *head;

        while (temp->flink)
        {
            temp = temp->flink;
        }
        temp->flink = new;
    }

    return SUCCESS;
}

int check_duplicates(F_node *head, char *f_name)
{
    if (head == NULL)
    {
        return SUCCESS;
    }

    while (head)
    {
        if (strcmp(head->f_name, f_name) == 0)
            return FAILURE;

        head = head->flink;
    }

    return SUCCESS;
}

int print_filenames(F_node *head)
{
    printf("\033[1;33m");
    if (head == NULL)
    {
        printf("File List is empty\n");
        return FAILURE;
    }

    printf("Files name -> ");

    while (head)
    {
        printf("%s -> ", head->f_name);

        head = head->flink;
    }
    printf("NULL\n\n");
    printf("\033[0m");
    return SUCCESS;
}

int get_index(char *word)
{
    if (islower(word[0]))
    {
        return word[0] - 'a';
    }
    else if (isupper(word[0]))
    {
        return word[0] - 'A';
    }
    else
    {
        return 26;
    }
}

void delet_filename_present(F_node **head, char *f_name)
{
    F_node *temp = *head;
    F_node *prev = NULL;

    while (temp)
    {
        if (strcmp(temp->f_name, f_name) == 0)
        {
            if (prev == NULL)
            {
                /* Removing the head node. */
                *head = temp->flink;
                free(temp);
            }
            else
            {
                /* Removing an interior/tail node. */
                prev->flink = temp->flink;
                free(temp);
            }
            break;
        }
        prev = temp;
        temp = temp->flink;
    }
}
