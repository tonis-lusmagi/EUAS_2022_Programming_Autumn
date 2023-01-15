#include "game-db.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// The aim of this homework will be to create a game database application.
// a) The game database will be an application to store info about games that a person owns
// b) It should support adding new games and removing existing ones
// c) It should be queryable by an identifier, Game title and Publisher
// d) All information should be stored in a file

int main() {
  int choice{};
  int highestID{};
  game *head = readFromFile(&highestID);
  char searchString[100];
  int searchId{};
  int idToDelete{};
  int running{1};

  while (running) {
    displayUserMenu();
    choice = getUserChoice();

    switch (choice) {
      case 1:
        // Add game
        head = addGame(head, &highestID);
        break;
      case 2:
        // List game
        // Sorting is run each time, before outputting, not a goal of the homework
        mergeSort(&head);
        printGameList(head);
        break;
      case 3:
        // Search for a game by title or game name
        printf("Enter the word to search for: ");
        scanf("\n%s", searchString);
        // Pass head as a variable
        findGamesByTitle(head, searchString);
        break;
      case 4:
        // Search for a game by ID
        printf("\nEnter game ID to search for: ");
        scanf("\n%d", &searchId);
        // Pass head as a variable
        findGame(head, searchId);
        break;
      case 5:
        // Remove a game by ID
        printf("\nEnter the game ID to delete: ");
        scanf("\n%d", &idToDelete);
        // Pass the memory address of head
        removeGame(&head, idToDelete);
        break;
      case 6:
        // Exit program
        return 0;
      default:
        break;
    }
  }
}

game *addGame(game *head, int *highestID) {
  game *newgame = (game *)malloc(sizeof(game));
  // The "++" operator in front of (*highestID) increments
  // the variable that highestID points to by one,
  // and then returns that number (to be used as the newgame's ID)
  newgame->ID = ++(*highestID);
  getgameTitle(newgame);
  getgamePublisher(newgame);
  newgame->next = head;
  writeToFile(newgame);
  return newgame;
}

void removeGame(game **head, int ID) {
  for (game *curr = *head; curr != NULL; curr = curr->next) {
    if (curr->next != NULL) {
      // Edge case of the first element being the one to delete
      if (curr->ID == ID) {
        *head = curr->next;
        break;
      }

      // Check if the next element in the list is the one we are looking for
      if (curr->next->ID == ID) {
        printf("\nremoving game %d) %s - %s", curr->next->ID, curr->next->publisher, curr->next->title);

        // If there is an element after the next, we set the current element's next
        // pointer to be that
        // example:
        // in a list of:
        // a->b->c
        // where we are trying to remove b
        // we set a's next to be c, thereby skipping b, and removing it from the list
        // and resulting in
        // a->c
        if (curr->next->next != NULL) {
          curr->next = curr->next->next;
        } else {
          // If there isn't, we just set net to null
          curr->next = NULL;
        }
      }
    } else {
      if (curr->ID == ID) {
        *head = NULL;
        break;
      }
    }
  }
  // Write the result to the file
  writeToFile(*head);
}

game *findGame(game *head, int ID) {
  game *currentHead = head;

  while (currentHead != NULL) {
    if (currentHead->ID == ID) {
      printf("\nFound game: %d) %s - %s", currentHead->ID, currentHead->publisher, currentHead->title);
      return currentHead;
    }
    currentHead = currentHead->next;
  }
  return NULL;
}

void findGamesByTitle(game *head, char *searchString) {
  game *currentHead = head;

  while (currentHead != NULL) {
    // Comparison is done via the strstr function, which returns a pointer
    // to the start of the first occurrence of the string searchString
    // in the title or publisher
    // || - is the OR operator
    // we check whether the returned pointer of the strstr function is
    // NULL or not, if it is not NULL, then the string searchString
    // appears in the currentHead->title (similar check for currentHead->publisher)
    if (strstr(currentHead->title, searchString) != NULL || strstr(currentHead->publisher, searchString) != NULL) {
      printf("\nFound game: %d) %s - %s", currentHead->ID, currentHead->publisher, currentHead->title);
    }
    currentHead = currentHead->next;
  }
}

void printGameList(game *head) {
  game *currentHead = head;

  if (currentHead == NULL) {
    printf("\nGame library empty!");
  } else {
    while (currentHead != NULL) {
      printf("\n%d) %s - %s", currentHead->ID, currentHead->publisher, currentHead->title);
      currentHead = currentHead->next;
    }
  }
}

game *readFromFile(int *highestID) {
  // Open a new file pointer
  FILE *fp = fopen("gamelist.txt", "r+");
  // Create temporary variables, that will be used
  game *head = NULL;
  game *newgame = NULL;
  char line[250];

  if (fp != NULL) {
    // fgets function gets a line from a file pointer
    // and stores, it into the variable "line"
    // the second argument 250 is the number of characters
    // to be read from the line. fgets Returns NULL
    // if EOF is reached (End of file)
    while (fgets(line, 250, fp) != NULL) {
      // Try to create a game from a line
      newgame = parseLine(line, highestID);

      if (newgame == NULL) {
        // If the parsing fails, exit the loope
        continue;
      }
      newgame->next = NULL;

      // Add the created game to the list
      if (head != NULL) {
        newgame->next = head;
        head = newgame;
      } else {
        head = newgame;
      }
    }
    fclose(fp);
    return head;
  } else {
    return NULL;
  }
  return NULL;
}

game *parseLine(char *line, int *highestID) {
  char *token;
  const char delimiter[3] = ",\n";
  // Create tokens
  token = strtok(line, delimiter);
  int pos = 0;
  game *newgame = (game *)malloc(sizeof(game));

  while (token != NULL) {
    // Copy data based on position
    switch (pos) {
      case 0:
        newgame->ID = atoi(token);
        if (newgame->ID > *highestID) {
          *highestID = newgame->ID;
        }
        break;
      case 1:
        // strcpy is mentioned in lesson slides
        // https://www.tutorialspoint.com/c_standard_library/c_function_strcpy.htm
        strcpy(newgame->publisher, token);
        break;
      case 2:
        strcpy(newgame->title, token);
        break;
      default:
        break;
    }
    // Get next token
    token = strtok(NULL, delimiter);
    // Increment position tracker
    pos++;
  }

  // After reading in the data,
  // the position counter should be incremented once per field,
  // so at least 3. as a simplification, a comparison against 0
  // is made, to see, if the read was successful
  // return the pointer to the newly created
  // game variable, otherwise a NULL pointer
  // to show that no data was created.
  if (pos > 0) {
    return newgame;
  } else {
    return NULL;
  }
}

void mergeSort(game **headPtr) {
  game *head = *headPtr;
  game *a;
  game *b;

  if ((head == NULL) || (head->next == NULL)) {
    return;
  }
  splitList(head, &a, &b);
  mergeSort(&a);
  mergeSort(&b);
  *headPtr = sortedMerge(a, b);
}

void splitList(game *source, game **a, game **b) {
  game *fast;
  game *slow;
  slow = source;
  fast = source->next;

  while (fast != NULL) {
    fast = fast->next;

    if (fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }
  *a = source;
  *b = slow->next;
  slow->next = NULL;
}

game *sortedMerge(game *a, game *b) {
  game *result = NULL;

  if (a == NULL) {
    return (b);
  } else if (b == NULL) {
    return (a);
  }

  if (a->ID <= b->ID) {
    result = a;
    result->next = sortedMerge(a->next, b);
  } else {
    result = b;
    result->next = sortedMerge(a, b->next);
  }
  return (result);
}

void writeToFile(game *head) {
  FILE *fp = fopen("gamelist.txt", "w+");
  game *currentHead = head;

  while (currentHead) {
    fprintf(fp, "%d,%s,%s\n", currentHead->ID, currentHead->publisher, currentHead->title);
    currentHead = currentHead->next;
  }
  fclose(fp);
}

void getgameTitle(game *game) {
  char title[100];
  printf("\nPlease enter the game title: ");
  scanf("\n%[^\n]s", title);
  strcpy(game->title, title);
}

void getgamePublisher(game *game) {
  char publisher[100];
  printf("\nPlease enter the game publisher: ");
  scanf("\n%[^\n]s", publisher);
  strcpy(game->publisher, publisher);
}

void displayUserMenu() {
  printf("\n");
  printf("\nWelcome to your game library! ");
  printf("\n");

  printf("\n1 - Add a game");
  printf("\n2 - List all games");
  printf("\n3 - Search for a game by title or publisher");
  printf("\n4 - Search for a game by ID");
  printf("\n5 - Remove a game by ID");
  printf("\n6 - Exit");
}

int getUserChoice() {
  fflush(stdin);  // Flush stdin input to get rid of any unwanted inputs
  int userChoice, numIntInput;
  printf("\nEnter your choice (1 - 6): ");
  numIntInput = scanf("\n %d", &userChoice);
  clear();

  if (numIntInput < 1 || (userChoice < 1 || userChoice > 6)) {
    printf("\nInvalid choice");
    return getUserChoice();
  }
  return userChoice;
}

void clear() {
  while(getchar() != '\n');
}
