#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int compute_score(string s);

int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    // Get string
    string s1 = get_string("Player 1: ");
    string s2 = get_string("Player 2: ");

    int score1 = compute_score(s1);
    int score2 = compute_score(s2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string s)
{
    // Score initialize to 0
    int sum = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // Only a-z and A-Z
        if ((islower(s[i])) || (isupper(s[i])))
        {
            // All convert to upper only
            s[i] = toupper(s[i]);
            sum = sum + POINTS[s[i] - 'A'];
        }
    }
    return sum;
}
