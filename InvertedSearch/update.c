#include "inverted.h"

int update_database(hash_t HT[], F_node **head)
{
    char backup_file[20];
    int flag = 0;
    printf("Enter backup file name : ");
    scanf("%s", backup_file);

    FILE *fp = fopen(backup_file, "r");

    if (fp == NULL)
    {
        printf("\033[1;34mERROR : Unable to open file\n\n\033[0m");
        return FAILURE;
    }

    /* Skip past the header line (column titles) written by save.c. */
    char header[100];
    fgets(header, sizeof(header), fp);

    int index, file_count;
    char word[30];
    char files[200];
    char counts[200];
    fseek(fp , 40 , SEEK_SET);

    /* Parse one "INDEX;WORD;FILECOUNT;FILES;COUNTS" row at a time. */
    while (fscanf(fp, "%d;%[^;];%d;%[^;];%[^\n]\n", &index, word, &file_count, files, counts) == 5)
    {
        flag = 1;
        M_node *mnew = malloc(sizeof(M_node));

        strcpy(mnew->word, word);
        mnew->file_count = file_count;
        mnew->main_link = NULL;
        mnew->sub_link = NULL;

        char *save1, *save2;

        char *fname = strtok_r(files, ",", &save1);
        char *wcount = strtok_r(counts, ",", &save2);

        S_node *last = NULL;

        /* Build the S_node chain for this word from the parallel
         * file-name / word-count token streams.                       */
        while (fname != NULL && wcount != NULL)
        {
            S_node *snew = malloc(sizeof(S_node));

            strcpy(snew->filename, fname);
            snew->word_count = atoi(wcount);
            snew->sub_link = NULL;

            /* Make sure this file isn't duplicated in the tracked file
             * list, since it's now accounted for via the backup data. */
            delet_filename_present(head,fname);

            if (mnew->sub_link == NULL)
                mnew->sub_link = snew;
            else
                last->sub_link = snew;

            last = snew;

            fname = strtok_r(NULL, ",", &save1);
            wcount = strtok_r(NULL, ",", &save2);
        }

        /* Attach the fully built M_node to the end of its hash bucket. */
        if (HT[index].main_link == NULL)
        {
            HT[index].main_link = mnew;
        }
        else
        {
            M_node *temp = HT[index].main_link;

            while (temp->main_link != NULL)
                temp = temp->main_link;

            temp->main_link = mnew;
        }
    }

    fclose(fp);

    if(flag == 0)
     return FAILURE;

    printf("\033[1;32mDatabase updated successfully\n\033[0m");

    return SUCCESS;
}
