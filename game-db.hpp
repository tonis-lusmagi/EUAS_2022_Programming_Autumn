#pragma once

// define game structure
typedef struct game {
  // data field
  int ID;
  char publisher[100];
  char title[100];
  struct game *next;
} game;

/*!
 * Clear terminal :)
 */
void clear();

/*!
 * Display the user menu
 */
void displayUserMenu();

/*!
 * Read user choice input
 * @return user choice int
 */
int getUserChoice();

/*!
 * Add a new game - Implementation of linked list
 * The current "head" game is passed as an argument to the function
 * A new game is allocated in memory (variable created)
 * The "next" field of the game is set to the current "head" of the list
 * the new game is returned by the function
 * a pointer to a variable containing the highest ID
 * used is also passed, that is to ensure that we increment the id correctly
 * @param head
 * @param highestID
 * @return
 */
game *addGame(game *head, int *highestID);

/*!
 * function for finding a game by ID from all games
 * @param head
 * @param ID
 * @return
 */
game *findGame(game *head, int ID);

/*!
 * It accepts a pointer, to a pointer of the head structure
 * We will be modifying the linked list in-memory
 * @param head
 * @param ID - ID of the game that should be removed
 */
void removeGame(game **head, int ID);

/*!
 * Function for finding games from all games
 * @param head
 * @param searchString
 */
void findGamesByTitle(game *head, char *searchString);

/*!
 * Get title for a game
 * @param game
 */
void getgameTitle(game *game);

/*!
 * Get publisher for game
 * @param game
 */
void getgamePublisher(game *game);

/*!
 * print game list
 * @param head - the head of the linked list
 */
void printGameList(game *head);

/*!
 * Writes the contents of the list, starting from head to a file
 * @param head
 */
void writeToFile(game *head);

/*!
 * This function creates a linked list
 * from the contents of a file.
 * the passed parameter highestID
 * is a pointer, to an int type
 * variable, that we will use, to
 * store the highest ID, in order
 * for our autoincrement code to work. (see the function addGame)
 * @param highestID
 * @return
 */
game *readFromFile(int *highestID);

/*!
 * This function tries to create a game
 * game pointer from a line of text.
 * The format of how data is stored is
 * defined in the function writeToFile line 317;
 * It works, by using the function strtok()
 * https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
 * to find positions of delimiters
 * that are defined in the string
 * delimiter (',' and '\n' )
 * in a text line such as:
 * "1,children of bodom,living dead beat".
 * The variable pos
 * is used to store the position of the data
 * inbetween the delimiters,
 * ( ID at position 0,  publisher at position 1 and title at position 2 )
 * @param line
 * @param highestID
 * @return
 */
game *parseLine(char *line, int *highestID);

/*!
 * Implementation of Merge Sort algorithm
 * Not necessary for your homework
 * https://en.wikipedia.org/wiki/Merge_sort
 * http://cslibrary.stanford.edu/105/LinkedListProblems.pdf
 * https://www.geeksforgeeks.org/c-program-for-merge-sort-for-linked-lists/
 * 
 * MergeSort algorithm is a good
 * example of divide and conquer
 * with good performance
 * for sorting linked lists.
 */
void mergeSort(game **a);

/*!
 * Split linked list
 * @param source
 * @param a
 * @param b
 */
void splitList(game *source, game **a, game **b);

/*!
 * Merge two sorted linked lists
 * @param a
 * @param b
 * @return
 */
game *sortedMerge(game *a, game *b);
