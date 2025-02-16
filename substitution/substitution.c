#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int check_key(string key);
string cipher_text(string key, string plaintext);

int main(int argc, string argv[])
{
    // make substitution && ./substitution
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check key
    string key = argv[1];
    if (check_key(key) == 1)
    {
        printf("Error! Contain non-alphabetical character!\n");
        return 1;
    }
    else if (check_key(key) == 2)
    {
        printf("Error! Invalid key length!\n");
        return 1;
    }
    else if (check_key(key) == 3)
    {
        printf("Error! Contain repeated character!\n");
        return 1;
    }

    // Prompt plain_text
    string plaintext = get_string("plaintext: ");

    // Cipher text
    string ciphertext = cipher_text(key, plaintext);

    // Output cipher text
    printf("ciphertext: %s\n", ciphertext);

    // End w/ return 0
    return 0;
}

int check_key(string key)
{
    int key_length = 0;
    bool seen[26] = {false};

    for (int i = 0, n = strlen(key); i < n; i++)
    {
        // Check for non-alphabetical character
        if (!islower(key[i]) && !isupper(key[i]))
        {
            return 1;
        }

        // Check for repeated character
        if (islower(key[i]))
        {
            seen[key[i] - 'a'] = true;
        }
        else if (isupper(key[i]))
        {
            seen[key[i] - 'A'] = true;
        }

        // Increment key
        key_length = key_length + 1;
    }

    // Validate key length
    if (key_length != 26)
    {
        return 2;
    }

    // Check if seen has false
    for (int i = 0; i < 26; i++)
    {
        if (seen[i] == false)
        {
            return 3;
        }
    }

    return 0;
}

string cipher_text(string key, string plaintext)
{
    // Substitution
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (islower(plaintext[i]))
        {
            char indexChar = plaintext[i];
            int indexInt = (int) indexChar - 'a';
            plaintext[i] = tolower(key[indexInt]);
        }
        else if (isupper(plaintext[i]))
        {
            char indexChar = plaintext[i];
            int indexInt = (int) indexChar - 'A';
            plaintext[i] = toupper(key[indexInt]);
        }
    }
    return plaintext;
}
