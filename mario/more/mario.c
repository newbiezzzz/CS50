#include <cs50.h>
#include <stdio.h>

int main(void)
{

    // Get height
    int h = get_int("Height: ");
    while (h > 8 || h < 1)
    {
        h = get_int("Height: ");
    }

    // Print mario
    for (int i = 0; i < h; i++)
    {
        // First space
        for (int j = 0; j < h - 1 - i; j++)
        {
            printf(" ");
        }

        // First #
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }

        // Second space
        printf("  ");

        // Second #
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }

        printf("\n");
    }
}
