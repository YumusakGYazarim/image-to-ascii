#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define ASCII_CHARS " .:-=+*%@#"

// Function to map grayscale values (0-255) to ASCII characters
char map_to_ascii(int gray_value) {
    int num_chars = strlen(ASCII_CHARS);
    int index = (gray_value * (num_chars - 1)) / 255;
    return ASCII_CHARS[index];
}

// Function to resize the image to a specific width and height
unsigned char* resize_image(unsigned char* image, int original_width, int original_height, int new_width, int new_height, int channels) {
    unsigned char* resized_image = (unsigned char*)malloc(new_width * new_height * channels);

    // Simple nearest-neighbor resizing
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            int old_x = (x * original_width) / new_width;
            int old_y = (y * original_height) / new_height;
            int old_index = (old_y * original_width + old_x) * channels;
            int new_index = (y * new_width + x) * channels;

            // Copy the pixel (RGB values) to the new resized image
            resized_image[new_index] = image[old_index];
            resized_image[new_index + 1] = image[old_index + 1];
            resized_image[new_index + 2] = image[old_index + 2];
        }
    }

    return resized_image;
}

int main() {
    const char *filename = "YourImage.jpg";  // Replace with your image path
    int width, height, channels;

    // Load the image
    unsigned char *image = stbi_load(filename, &width, &height, &channels, 0);
    if (image == NULL) {
        printf("Error loading image\n");
        return 1;
    }

    // Resize the image to the desired amount
    int new_width = 30;
    int new_height = 30;

    unsigned char* resized_image = resize_image(image, width, height, new_width, new_height, channels);

    // Create a new buffer for the grayscale image (1 channel)
    unsigned char *gray_image = (unsigned char *)malloc(new_width * new_height);

    // Convert to grayscale
    for (int i = 0; i < new_width * new_height; i++) {
        int r = resized_image[i * channels];         // Red component
        int g = resized_image[i * channels + 1];     // Green component
        int b = resized_image[i * channels + 2];     // Blue component

        // Grayscale conversion (luminosity method)
        unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
        gray_image[i] = gray;  // Store grayscale value
    }

    // Open a file to save the ASCII art
    FILE *file = fopen("ascii_art.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        stbi_image_free(image);
        free(resized_image);
        free(gray_image);
        return 1;
    }

    // Write the ASCII art to the file
    fprintf(file, "ASCII Art (Image: %s)\n", filename);
    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            // Calculate the index of the current pixel in the grayscale buffer
            int pixel_index = y * new_width + x;
            unsigned char gray_value = gray_image[pixel_index];
            
            // Map the grayscale value to an ASCII character and write it to the file
            char ascii_char = map_to_ascii(gray_value);
            fprintf(file, "%c", ascii_char);
        }
        fprintf(file, "\n");  // Move to the next line after each row of pixels
    }

    // Close the file
    fclose(file);

    // Free the memory
    stbi_image_free(image);
    free(resized_image);
    free(gray_image);

    printf("ASCII art saved successfully to ascii_art.txt\n");

    return 0;
}
