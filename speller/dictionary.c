// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 150001; // Approx. words in the dictionary

// Global variable
unsigned int count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int index = hash(word);

    node *n = table[index];
    if (n == NULL)
    {
        return false;
    }

    while (strcasecmp(n->word, word) != 0)
    {
        if (n->next == NULL)
        {
            return false;
        }
        n = n->next;
    }
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // ASCII sum
    unsigned int sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        sum = sum + tolower(word[i]); // word[i] -> int if arithmetic operation
    }
    return sum % N; // modulos prevent overflow (unlikely happen)
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Initialize table to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char word[LENGTH + 1];

    // Read each word in the file
    while (fscanf(dict, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(dict);
            return false;
        }
        n->next = NULL;

        // Copy string
        strcpy(n->word, word);

        // Get index and put to array of linked list (table)
        unsigned int index = hash(n->word); // Unsigned int is all positive

        if (table[index] == NULL)
        {
            table[index] = n;
            count++;
        }
        else
        {
            // First, link current node to node in table[index]
            n->next = table[index];
            // Then, link table[index] to current node
            table[index] = n;
            count++;
        }
    }
    // Close file
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *curr = table[i];
        node *next = NULL;

        while (curr != NULL) // will stop before last node
        {
            next = curr->next; // get next pointer,
            free(curr);
            curr = next; // update current pointer
        }
    }
    return true;
}
