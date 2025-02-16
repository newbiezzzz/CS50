#include "helpers.h"
#include <math.h>

int clamp(int value);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // For loop
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Rounding: 0.5 for each color, 1.5 for 3 colors
            BYTE gray =
                (image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen + 1.5) / 3;
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtRed = gray;
            image[i][j].rgbtGreen = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Left to right, right to left
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // end = width - 1
            // image[0][0] = image[0][end]
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_copy[height][width];
    RGBTRIPLE zero = {0, 0, 0};

    // Initialize same size empty image_copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // image_copy[i][j] = 0;
            image_copy[i][j] = zero;
        }
    }

    // Loop whole image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Loop in 3x3 around the pixel
            int num = 0;
            int R = 0;
            int G = 0;
            int B = 0;
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int new_i = i + di;
                    int new_j = j + dj;

                    // If out of bounds, then skip
                    if (new_i < 0 || new_i >= height || new_j < 0 || new_j >= width)
                    {
                        continue;
                    }

                    // Else, add
                    R = R + image[new_i][new_j].rgbtRed;
                    G = G + image[new_i][new_j].rgbtGreen;
                    B = B + image[new_i][new_j].rgbtBlue;
                    num = num + 1;
                }
            }
            // After sum, average out
            // float is for avoids truncation for int
            image_copy[i][j].rgbtRed = round((float) R / num);
            image_copy[i][j].rgbtGreen = round((float) G / num);
            image_copy[i][j].rgbtBlue = round((float) B / num);
        }
    }

    // At the end of loop, copy back to original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_copy[height][width];
    int G_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int G_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Copy image to not change original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
        }
    }

    // Loop whole image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // RGB sum for image_x
            int Rx = 0;
            int Gx = 0;
            int Bx = 0;
            // RGB sum for image_y
            int Ry = 0;
            int Gy = 0;
            int By = 0;

            // Loop in 3x3 around the pixel
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    // Image index
                    int new_i = i + di;
                    int new_j = j + dj;

                    // Kernel index
                    int Gi = di + 1;
                    int Gj = dj + 1;

                    // If out of bounds, then skip
                    if (new_i < 0 || new_i >= height || new_j < 0 || new_j >= width)
                    {
                        continue;
                    }

                    // x-direction
                    Rx = Rx + image_copy[new_i][new_j].rgbtRed * G_x[Gi][Gj];
                    Gx = Gx + image_copy[new_i][new_j].rgbtGreen * G_x[Gi][Gj];
                    Bx = Bx + image_copy[new_i][new_j].rgbtBlue * G_x[Gi][Gj];

                    // y-direction
                    Ry = Ry + image_copy[new_i][new_j].rgbtRed * G_y[Gi][Gj];
                    Gy = Gy + image_copy[new_i][new_j].rgbtGreen * G_y[Gi][Gj];
                    By = By + image_copy[new_i][new_j].rgbtBlue * G_y[Gi][Gj];
                }
            }

            // sqrt(Gx^2 + Gy^2)
            int R = (int) round(sqrt(Rx * Rx + Ry * Ry));
            int G = (int) round(sqrt(Gx * Gx + Gy * Gy));
            int B = (int) round(sqrt(Bx * Bx + By * By));

            // Clamp values 0 to 255
            image[i][j].rgbtRed = clamp(R);
            image[i][j].rgbtGreen = clamp(G);
            image[i][j].rgbtBlue = clamp(B);
        }
    }
    return;
}

int clamp(int value)
{
    if (value > 255)
    {
        return 255;
    }
    if (value < 0)
    {
        return 0;
    }
    return value;
}
