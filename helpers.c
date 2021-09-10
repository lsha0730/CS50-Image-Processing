#include "helpers.h"
#include <stddef.h>
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue)/3.0);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i=0; i<height; i++) {
        for (int j=0; j<width/2; j++) {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width-j-1];
            image[i][width-j-1] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Define a copy of image
    RGBTRIPLE image_copy[height][width];
    for (int x=0; x<height; x++) {
        for (int y=0; y<width; y++) {
            image_copy[x][y].rgbtRed = image[x][y].rgbtRed;
            image_copy[x][y].rgbtGreen = image[x][y].rgbtGreen;
            image_copy[x][y].rgbtBlue = image[x][y].rgbtBlue;
        }
    }

    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            //For every pixel in the image...

            int sum_red = 0;
            int sum_green = 0;
            int sum_blue = 0;

            float counter = 0;

            for (int a=-1; a<2; a++) {
                for (int b=-1; b<2; b++) {
                    //For every neighbour pixel, if it exists, add it to sum
                    if (i+a>=0 && i+a<height && j+b>=0 && j+b<width) {
                        sum_red += image_copy[i+a][j+b].rgbtRed;
                        sum_green += image_copy[i+a][j+b].rgbtGreen;
                        sum_blue += image_copy[i+a][j+b].rgbtBlue;
                        counter++;
                    }
                }
            }

            //Divide sum by number of valid neighbours, then assign to image pixel

            image[i][j].rgbtRed = round(sum_red/counter);
            if (round (sum_red/counter) > 255) {image[i][j].rgbtRed = 255;}

            image[i][j].rgbtGreen = round(sum_green/counter);
            if (round (sum_green/counter) > 255) {image[i][j].rgbtGreen = 255;}

            image[i][j].rgbtBlue = round(sum_blue/counter);
            if (round (sum_blue/counter) > 255) {image[i][j].rgbtBlue = 255;}

            //End of this pixel of the image
        }
    }

    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    /*Define a copy of image for use as reference so that
    while creating the new image, the reference material is not contaminated*/
    RGBTRIPLE image_copy[height][width];
    for (int x=0; x<height; x++) {
        for (int y=0; y<width; y++) {
            image_copy[x][y].rgbtRed = image[x][y].rgbtRed;
            image_copy[x][y].rgbtGreen = image[x][y].rgbtGreen;
            image_copy[x][y].rgbtBlue = image[x][y].rgbtBlue;
        }
    }

    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            //For every pixel in the image...

            RGBTRIPLE neighbour[9];

            int counter = 0;

            for (int a=-1; a<2; a++) {
                for (int b=-1; b<2; b++) {
                    //For every neighbour pixel...
                    if (i+a>=0 && i+a<height && j+b>=0 && j+b<width) {
                        //if it exists, add it to the neighbour array
                        neighbour[counter].rgbtRed = image_copy[i+a][j+b].rgbtRed;
                        neighbour[counter].rgbtGreen = image_copy[i+a][j+b].rgbtGreen;
                        neighbour[counter].rgbtBlue = image_copy[i+a][j+b].rgbtBlue;
                        counter++;
                    } else {
                        //if it doesn't, add a black pixel to the neighbour array
                            //Doing this makes it have zero influence over the final sum
                        neighbour[counter].rgbtRed = 0;
                        neighbour[counter].rgbtGreen = 0;
                        neighbour[counter].rgbtBlue = 0;
                        counter++;
                    }
                }
            }

            //Create kernel values
            float Gx_red = 0;
            float Gx_green = 0;
            float Gx_blue = 0;

            Gx_red += neighbour[0].rgbtRed*(-1);
            Gx_red += neighbour[2].rgbtRed*(1);
            Gx_red += neighbour[3].rgbtRed*(-2);
            Gx_red += neighbour[5].rgbtRed*(2);
            Gx_red += neighbour[6].rgbtRed*(-1);
            Gx_red += neighbour[8].rgbtRed*(1);

            Gx_green += neighbour[0].rgbtGreen*(-1);
            Gx_green += neighbour[2].rgbtGreen*(1);
            Gx_green += neighbour[3].rgbtGreen*(-2);
            Gx_green += neighbour[5].rgbtGreen*(2);
            Gx_green += neighbour[6].rgbtGreen*(-1);
            Gx_green += neighbour[8].rgbtGreen*(1);

            Gx_blue += neighbour[0].rgbtBlue*(-1);
            Gx_blue += neighbour[2].rgbtBlue*(1);
            Gx_blue += neighbour[3].rgbtBlue*(-2);
            Gx_blue += neighbour[5].rgbtBlue*(2);
            Gx_blue += neighbour[6].rgbtBlue*(-1);
            Gx_blue += neighbour[8].rgbtBlue*(1);

            float Gy_red = 0;
            float Gy_green = 0;
            float Gy_blue = 0;

            Gy_red += neighbour[0].rgbtRed*(-1);
            Gy_red += neighbour[1].rgbtRed*(-2);
            Gy_red += neighbour[2].rgbtRed*(-1);
            Gy_red += neighbour[6].rgbtRed*(1);
            Gy_red += neighbour[7].rgbtRed*(2);
            Gy_red += neighbour[8].rgbtRed*(1);

            Gy_green += neighbour[0].rgbtGreen*(-1);
            Gy_green += neighbour[1].rgbtGreen*(-2);
            Gy_green += neighbour[2].rgbtGreen*(-1);
            Gy_green += neighbour[6].rgbtGreen*(1);
            Gy_green += neighbour[7].rgbtGreen*(2);
            Gy_green += neighbour[8].rgbtGreen*(1);

            Gy_blue += neighbour[0].rgbtBlue*(-1);
            Gy_blue += neighbour[1].rgbtBlue*(-2);
            Gy_blue += neighbour[2].rgbtBlue*(-1);
            Gy_blue += neighbour[6].rgbtBlue*(1);
            Gy_blue += neighbour[7].rgbtBlue*(2);
            Gy_blue += neighbour[8].rgbtBlue*(1);

            //Combine kernel values via sqrt(a^2 + b^2) to find final values
            int finalred = round(sqrt(Gx_red*Gx_red + Gy_red*Gy_red));
            if (finalred > 255) { finalred = 255; }
            if (finalred < 0) { finalred = 0; }

            int finalgreen = round(sqrt(Gx_green*Gx_green + Gy_green*Gy_green));
            if (finalgreen > 255) { finalgreen = 255; }
            if (finalgreen < 0) { finalgreen = 0; }

            int finalblue = round(sqrt(Gx_blue*Gx_blue + Gy_blue*Gy_blue));
            if (finalblue > 255) { finalblue = 255; }
            if (finalblue < 0) { finalblue = 0; }

            image[i][j].rgbtRed = finalred;
            image[i][j].rgbtGreen = finalgreen;
            image[i][j].rgbtBlue = finalblue;

            /* This pixel of the image has been altered.
            Move onto the next pixel in the image. */
        }
    }

    return;
}
