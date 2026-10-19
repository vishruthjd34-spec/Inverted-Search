# 🔎 Inverted Search in C

A menu-driven **inverted index** builder for the terminal — index words from multiple text files, search them instantly, and save / restore the index from a backup file.

![Language](https://img.shields.io/badge/Language-C-blue)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20WSL-lightgrey)
![Storage](https://img.shields.io/badge/Storage-Text%20File-green)

---

## 📖 About

This project is a console-based **Inverted Search** (inverted index) written in C. You pass one or more `.txt` files on the command line, and the program builds a database that maps **every distinct word → the files it appears in → how many times it appears in each file**.

The database can be displayed as a table, searched by word, saved to a backup `.txt` file, and loaded back later.

It was built to practise core C and data-structure concepts: **linked lists, hash tables, dynamic memory (`malloc`), file I/O, command-line arguments, string handling** and **modular programming** (multiple `.c` / `.h` files).

---

## 💡 What is an Inverted Index?

A normal index says *"this file contains these words"*.
An **inverted index** flips it: *"this word is found in these files"* — which makes searching a word very fast (the same idea search engines use).

```
Word      →  Files it appears in (with count)
-----------------------------------------------
how       →  f1.txt (1), f2.txt (1)
hii       →  f2.txt (1), f3.txt (1)
vishruth  →  f3.txt (1)
```

---

## ✨ Features

| | Feature | Description |
|---|---|---|
| 🛠️ | **Create database** | Reads every valid file word by word and builds the inverted index |
| 📋 | **Display database** | Prints the full index in a formatted table (index, word, file count, file name, word count) |
| 🔍 | **Search a word** | Type a word and see which files contain it and how many times |
| 💾 | **Save database** | Writes the index to a `.txt` backup file (semicolon-separated) |
| ♻️ | **Update database** | Loads a previously saved backup file back into memory |
| ✅ | **Input file validation** | Checks extension, existence, empty files and duplicates before indexing |
| 🎨 | **Colour-coded output** | Success / info / table messages in different terminal colours |
| 🆘 | **Help option** | `-h` or `--help` prints usage instructions |

---

## 🛡️ File Validation Rules

Every file name given on the command line is checked before it is accepted:

| Check | Rule |
|---|---|
| 📎 **Extension** | Must have an extension, and it must be `.txt` |
| 📂 **Existence** | File must exist and be readable |
| 📭 **Empty file** | Empty files are rejected |
| 🔁 **Duplicates** | A file name repeated in the arguments is stored only once |

At least **one valid file** is required, otherwise the program prints the usage message and exits.

---

## 🗂️ Project Structure

```
InvertedSearch/
├── main.c           # 🚪 Entry point, argument handling and menu loop
├── inverted.h       # 📐 Structures, macros and all function declarations
├── inverted.c       # 🧰 Validation, file-list helpers, hash index function, usage message
├── create.c         # 🛠️ Build the inverted index from the input files
├── display.c        # 📋 Print the database as a table
├── search.c         # 🔍 Search a word in the database
├── save.c           # 💾 Save the database to a backup file
├── update.c         # ♻️ Restore the database from a backup file
├── f1.txt f2.txt f3.txt  # 🧪 Sample input files
├── backup.txt       # 📄 Empty file you can use as a save target
├── Makefile         # 🔧 Build shortcuts
├── .gitignore       # 🙈 Ignores compiled binaries
├── source_files/    # 📚 Reference PDFs for the project problem statement
└── README.md        # 📄 You are here
```

---

## 🧩 Data Structures

The index is a **hash table of 27 buckets** — `0–25` for words starting with `a–z` (case-insensitive) and `26` for everything else (numbers, symbols, `?` etc.).

Each bucket holds a **linked list of words**, and each word holds a **linked list of files** it appears in.

```c
typedef struct file {                 /* list of validated input files   */
    char f_name[20];
    struct file *flink;
} F_node;

typedef struct Sub {                  /* word occurrence in ONE file     */
    int  word_count;
    char filename[20];
    struct Sub *sub_link;
} S_node;

typedef struct Main {                 /* one distinct word               */
    char word[20];
    int  file_count;
    S_node *sub_link;
    struct Main *main_link;
} M_node;

typedef struct hash_table {           /* one bucket (a-z, other)         */
    int index;
    struct Main *main_link;
} hash_t;
```

### 🔗 How it looks in memory

```
HT[0]  ──► [are] ──► NULL
             │
             └─► [f1.txt,1] ──► [f2.txt,1] ──► NULL

HT[7]  ──► [how] ──► [hii] ──► [hello] ──► NULL
             │         │          │
             │         │          └─► [f2.txt,1]
             │         └─► [f2.txt,1] ──► [f3.txt,1]
             └─► [f1.txt,1] ──► [f2.txt,1]
   ⋮
HT[26] ──► [?] ──► NULL      (non-alphabet words)
```

---

## 🚀 Getting Started

### ✅ Prerequisites

- A C compiler such as **GCC**
- Linux, macOS, or Windows with **WSL / MinGW** (the code uses POSIX `strtok_r`)

### 📥 Clone the repository

```bash
git clone https://github.com/<your-username>/<your-repo-name>.git
cd <your-repo-name>
```

### 🔧 Build

Using make:

```bash
make
```

Or directly with GCC:

```bash
gcc -Wall -o inverted main.c create.c display.c inverted.c save.c search.c update.c
```

### ▶️ Run

Pass one or more `.txt` files as arguments:

```bash
./inverted f1.txt f2.txt f3.txt
```

Or run the sample files with one command:

```bash
make run
```

Show help:

```bash
./inverted -h
```

---

## 🎮 How to Use

After the files are validated, you will see this menu:

```
Select your choice among following operations:
1. Create Database
2. Display Database
3. Save Database
4. Search
5. Update Database
6. Exit

Enter your choice :
```

| Choice | What happens |
|---|---|
| **1** | Builds the inverted index from all validated files (can be done only once per run) |
| **2** | Displays the whole database in a table |
| **3** | Asks for a file name (must end in `.txt`) and saves the database into it |
| **4** | Asks for a word and shows the files it appears in with the count in each |
| **5** | Asks for a backup file name and loads the saved database (allowed only **before** creating the database) |
| **6** | Exits the program |

> ⚠️ **Note:** Option **5 (Update)** works only on a fresh run — if you have already created or updated the database in the current run, it will refuse. Use it **instead of** option 1 to restore a saved index.

---

## 🖥️ Sample Run

**Input files**

```
f1.txt  →  how are you ? i dont know
f2.txt  →  hii hello how are you ?
f3.txt  →  hii jd vishruth
```

**Command**

```bash
./inverted f1.txt f2.txt f3.txt
```

**Validation output**

```
INFO : Successful : inserted file name f1.txt into file linked list
INFO : Successful : inserted file name f2.txt into file linked list
INFO : Successful : inserted file name f3.txt into file linked list

validation SUCCESSFUL

Files name -> f1.txt -> f2.txt -> f3.txt -> NULL
```

**Option 2 – Display Database** *(shortened)*

```
 INDEX  |        WORD          |  FILE COUNT  |  FILE NAME  |  WORD COUNT
------------------------------------------------------------------------
0       | are                  | 2            | f1.txt      | 1
        |                      |              | f2.txt      | 1
------------------------------------------------------------------------
7       | how                  | 2            | f1.txt      | 1
        |                      |              | f2.txt      | 1
------------------------------------------------------------------------
7       | hii                  | 2            | f2.txt      | 1
        |                      |              | f3.txt      | 1
------------------------------------------------------------------------
21      | vishruth             | 1            | f3.txt      | 1
------------------------------------------------------------------------
26      | ?                    | 2            | f1.txt      | 1
        |                      |              | f2.txt      | 1
```

**Option 4 – Search**

```
Enter the word you need search : how
word how is present in 2 file
In file f1.txt 1 time
In file f2.txt 1 time
```

---

## 💾 Backup File Format

When you choose **Save Database**, the index is written as a semicolon-separated text file with a header line:

```
INDEX;WORD;FILECOUNT;FILENAME;WORDCOUNT
```

Each following line is one word. Multiple files / counts for the same word are comma-separated:

```
INDEX;WORD;FILECOUNT;FILENAME;WORDCOUNT
0;are;2;f1.txt,f2.txt;1,1
3;dont;1;f1.txt;1
7;how;2;f1.txt,f2.txt;1,1
7;hii;2;f2.txt,f3.txt;1,1
7;hello;1;f2.txt;1
21;vishruth;1;f3.txt;1
26;?;2;f1.txt,f2.txt;1,1
```

> ⚠️ Don't save the backup under the **same name** as one of your input files (e.g. `f1.txt`) — it will overwrite your input data. Use a name like `backup.txt`.

---

## 🧹 Clean build files

```bash
make clean
```

---

## 🧠 Concepts Used

- 🧱 Structures (`struct`) and `typedef`
- 🔗 Singly linked lists (file list, word list, per-file occurrence list)
- #️⃣ Hash table with 27 buckets (chaining to handle collisions)
- 🧮 Dynamic memory allocation with `malloc`
- 💻 Command-line arguments (`argc`, `argv`)
- 📂 File handling: `fopen`, `fscanf`, `fprintf`, `fgets`, `fseek`, `ftell`, `fclose`
- 🔤 String functions: `strcmp`, `strcpy`, `strcat`, `strstr`, `strtok_r`
- 🧩 Modular code with multiple source and header files
- 🔁 Loops and `switch-case` menu
- 🎨 ANSI escape codes for coloured terminal output

---

## ⚠️ Known Limitations

- Words are limited to **19 characters**, file names to **19 characters** (fixed-size buffers)
- Search is **case-sensitive** (`Hello` and `hello` are stored as different words, though they share the same bucket)
- Punctuation stays attached to words (`hello,` and `hello` are different words)
- The backup file must have been created by this program's **Save** option

---

## 🔮 Future Improvements

- [ ] 🔠 Convert words to lowercase and strip punctuation before indexing
- [ ] 📏 Remove the fixed word / file name length limits
- [ ] 🧹 Free all allocated memory before exiting
- [ ] ♻️ Allow update (merge) even after the database has been created
- [ ] 📊 Sort words alphabetically inside each bucket

---

## 👨‍💻 Author

**Vishruth J D**

---

⭐ If you found this project helpful, consider giving it a star on GitHub!
