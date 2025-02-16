#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long number = get_long("Number: ");
    int l = 0;
    int sum = 0;
    int digit = 0;
    int twodigits = 0;

    // Loop
    while (number != 0)
    {
        l = l + 1;            // Number length
        digit = number % 10;  // Digit
        number = number / 10; // Current number

        if (number > 9 && number < 100) // 10 to 99
        {
            twodigits = number;
        }

        // printf("Digit: %i \n", digit);
        // printf("Current Number: %li \n", number);

        // Adding checksum
        if (l % 2 == 0)
        {
            int digit_X2 = digit * 2;
            if (digit_X2 > 9)
            {
                int d1 = digit_X2 % 10;        // 12 % 10 = 2
                int d2 = (digit_X2 / 10) % 10; // 1 % 10 = 1
                sum = sum + d1 + d2;
            }
            else
                sum = sum + digit_X2;
        }
        else
            sum = sum + digit;
    }

    if (sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else if (l == 15 && (twodigits == 34 || twodigits == 37))
    {
        printf("AMEX\n");
    }
    else if ((l == 13 || l == 16) && digit == 4)
    {
        printf("VISA\n");
    }
    else if (l == 16 && (twodigits == 51 || twodigits == 52 || twodigits == 53 || twodigits == 54 ||
                         twodigits == 55))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
