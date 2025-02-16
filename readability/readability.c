#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Compute the Coleman-Liau index
    float L = (float) letters / words * 100;
    float S = (float) sentences / words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int index_rounded = round(index);

    // Print the grade level
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index_rounded);
    }
}

int count_letters(string text)
{
    int count = 0;
    // Return the number of letters in text
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if ((islower(text[i])) || (isupper(text[i])))
        {
            count = count + 1;
        }
    }
    printf("Letters: %i\n", count);
    return count;
}

int count_words(string text)
{
    // Return the number of words in text
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char curr = text[i];
        char next = text[i + 1];
        // Transition non-space to space
        if ((curr != ' ') && ((next == ' ') || next == '\0'))
        {
            count = count + 1;
        }
    }
    printf("Words: %i\n", count);
    return count;
}

int count_sentences(string text)
{
    // Return the number of sentences in text
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char curr = text[i];
        char next = text[i + 1];
        // Transition character to 3 symbols
        if ((isupper(curr) || islower(curr)) && (next == '.' || next == '?' || next == '!'))
        {
            count = count + 1;
        }
    }

    printf("Sentences: %i\n", count);
    return count;
}
