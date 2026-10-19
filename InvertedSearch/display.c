#include "inverted.h"

void display_database(hash_t HT[])
{

    printf("\033[1;33m");
    printf("---------------------------------------------------------------------------------------------------------------------------\n");
    printf(" INDEX  |        WORD          |  FILE COUNT  |                    FILE NAME                       |      WORD COUNT      |\n");
    printf("---------------------------------------------------------------------------------------------------------------------------\n");

    /* Iterate every bucket (0-25 = 'a'-'z', 26 = non-alphabetic words). */
    for(int i = 0 ; i < 27 ; i++)
    {
        /* Skip empty buckets entirely. */
        if(HT[i].main_link == NULL)
          continue;

        char word[20], file_name[50] , word_count[20];
        int file_count;

        M_node *Mtemp = HT[i].main_link;

        /* Walk every distinct word stored in this bucket. */
        while(Mtemp)
        {
           /* flag tracks whether we're printing the FIRST row for this
            * word (which shows index/word/file_count) or a follow-up
            * row (which only shows the file name/word count).         */
           int flag = 0;
           S_node *Stemp = Mtemp->sub_link;

           file_count = Mtemp->file_count;
           strcpy(word , Mtemp->word);

           file_name[0] = '\0';
           word_count[0] = '\0';

           /* Walk every file this word occurs in. */
           while(Stemp)
           {
                if(flag == 0)
                {
                    /* First row for this word -> print all columns. */
                    printf("%-7d | %-20s | %-12d | %-50s | %-20d |\n", i , Mtemp->word , Mtemp->file_count , Stemp->filename , Stemp->word_count);
                    flag = 1;
                }
                else
                {
                    /* Subsequent rows -> only file name/word count differ. */
                    printf("%-7s | %-20s | %-12s | %-50s | %-20d |\n", " " , " " , " " , Stemp->filename , Stemp->word_count);
                }

                Stemp = Stemp->sub_link;
           }

           printf("---------------------------------------------------------------------------------------------------------------------------\n");
           Mtemp = Mtemp->main_link;
        }
    }
    printf("\033[0m");
}
