#include "inverted.h"

void create_database(hash_t HT[], F_node *head)
{
    /* Process every input file, one at a time. */
    while(head)
    {
        FILE *fp = fopen(head->f_name , "r");
        int index;
        char word[20];

        /* Read the file word by word (whitespace separated tokens). */
        while((fscanf(fp ,"%s" , word)) == 1)
        {
            /* Step 1: figure out which hash bucket this word belongs to. */
            index = get_index(word);

            if(HT[index].main_link == NULL)
            {
               /* Step 2: bucket is empty -> this is a brand new word. */
               M_node *Mnew = malloc(sizeof(M_node));
               S_node *Snew = malloc(sizeof(S_node));

               strcpy(Mnew->word , word);
               Mnew->file_count = 1;
               Mnew->main_link = NULL;

               strcpy(Snew->filename , head->f_name);
               Snew->word_count = 1;
               Snew->sub_link = NULL;

               Mnew->sub_link = Snew;
               HT[index].main_link = Mnew;
            }
            else
            {
                /* Step 3: bucket already has words -> search for a match. */
                M_node *Mtemp = HT[index].main_link;
                M_node *Mprev = NULL;

                while(Mtemp)
                {
                    if(strcmp(Mtemp->word, word) == 0)
                    {
                        /* Word already exists in this bucket. */
                        break;
                    }

                    Mprev = Mtemp;
                    Mtemp = Mtemp->main_link;
                }

                if(Mtemp == NULL)
                {
                    /* Step 3a: word not found in the bucket -> new distinct
                     * word that happens to hash-collide with existing
                     * words; append a new M_node at the end of the chain. */
                    M_node *Mnew = malloc(sizeof(M_node));
                    S_node *Snew = malloc(sizeof(S_node));

                    strcpy(Mnew->word , word);
                    Mnew->file_count = 1;
                    Mnew->main_link = NULL;

                    strcpy(Snew->filename , head->f_name);
                    Snew->word_count = 1;
                    Snew->sub_link = NULL;

                    Mnew->sub_link = Snew;
                    Mprev->main_link = Mnew;
                }
                else
                {
                    /* Step 3b: word already known -> look for an existing
                     * occurrence record for the CURRENT file. */
                    S_node *Stemp = Mtemp->sub_link;
                    S_node *Sprev = NULL;

                    while(Stemp)
                    {
                        if(strcmp(Stemp->filename , head->f_name) == 0)
                        {
                            /* Word already recorded for this file ->
                             * just bump its occurrence count.          */
                            Stemp->word_count++;
                            break;
                        }

                        Sprev = Stemp;
                        Stemp = Stemp->sub_link;
                    }

                    if(Stemp == NULL)
                    {
                        /* Word is known, but this is the first time it
                         * shows up in THIS particular file -> add a new
                         * S_node and increase the word's file_count.    */
                        S_node *Snew = malloc(sizeof(S_node));

                        strcpy(Snew->filename , head->f_name);
                        Snew->word_count = 1;
                        Snew->sub_link = NULL;

                        Sprev->sub_link = Snew;

                        Mtemp->file_count++;
                    }
                }
            }
        }

        fclose(fp);

        /* Move on to the next input file. */
        head = head->flink;
    }

    printf("\033[1;32mDatabase created successfully\n\033[0m");
}
