#ifndef INVERTED_H
#define INVERTED_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>

#define SUCCESS		1
#define FAILURE		0

typedef struct file
{
    char f_name[20];        /* Name of the input text file              */
    struct file *flink;     /* Pointer to the next file node            */
}F_node;

typedef struct Sub
{
    int word_count;          /* No. of times the word occurs in filename */
    char filename[20];       /* Name of the file this count belongs to   */
    struct Sub *sub_link;    /* Pointer to next file-occurrence record   */
}S_node;

typedef struct Main
{
    char word[20];            /* The distinct word itself                */
    int file_count;           /* No. of DIFFERENT files this word is in  */
    S_node *sub_link;         /* Head of the per-file occurrence list    */
    struct Main *main_link;   /* Pointer to the next distinct word       */
}M_node;

typedef struct hash_table
{
    int index;                /* Bucket index (0-25 => a-z, 26 => other) */
    struct Main *main_link;   /* Head of the M_node chain for this bucket*/
}hash_t;


/* ===================== Core database operations ======================= */

/* Reads every validated input file word by word and builds the in-memory
 * inverted index (hash table of M_node/S_node chains).                    */
void create_database(hash_t HT[], F_node *head);

/* Prints the entire inverted index in a formatted, tabular layout.        */
void display_database(hash_t HT[]);

/* Prompts the user for a word and reports which files contain it and
 * how many times.                                                        */
void search_database(hash_t HT[]);

/* Persists the current in-memory inverted index to a semicolon separated
 * ".txt" file so it can be restored later using update_database().       */
void save_database(hash_t HT[]);

/* Rebuilds/merges the in-memory inverted index from a previously saved
 * backup file created by save_database().                                */
int update_database(hash_t HT[],F_node **head);


/* ===================== Command line / file helpers ===================== */

/* Validates every file name supplied on the command line (existence,
 * extension, emptiness, duplicates) and stores the valid ones in the
 * F_node linked list.                                                     */
int validate_files(int argc, char *argv[], F_node **head);

/* Prints a professional usage/help banner describing how to run the
 * program. Called on -h/--help or when arguments are missing/invalid.    */
void print_usage(const char *prog_name);

/* Inserts a new file name node at the end of the F_node linked list.      */
int insert_last(F_node **head, char *f_name);

/* Checks whether a file name already exists in the F_node linked list,
 * used to silently skip duplicate command line arguments.                */
int check_duplicates(F_node *head, char *f_name);

/* Prints all currently tracked (validated) file names.                    */
int print_filenames(F_node *head);

/* Maps the first character of a word to a hash table bucket index
 * (0-25 for 'a'-'z'/'A'-'Z', 26 for everything else).                     */
int get_index(char *word);

/* Returns FAILURE if the given open file is empty, SUCCESS otherwise.     */
int file_staus(FILE *fp);

/* (Currently unused helper kept for API completeness) reads one
 * whitespace separated token from a file.                                 */
int getworld(FILE *fp , char *world);

/* Removes a file name from the F_node linked list -- used while merging
 * a backup file back into the database so stale entries can be dropped.   */
void delet_filename_present(F_node **head, char *f_name);

#endif
