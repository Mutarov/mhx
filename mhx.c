/*
==============================
Murat's Hex Editor (mhx)
A minimalist hex editor for command-line use.

By Murat N.S. 2024
==============================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_SIZE 256  // Initial memory size

// Function to display the usage instructions
void show_help() {
    printf("Commands:\n");
    printf("ab <num> - Allocate <num> bytes\n");
    printf("adb <num> - Allocate <num> double bytes (8 * 2)\n");
    printf("abs <num> - Allocate <num> bits\n");
    printf("p - Preview entire file\n");
    printf("p <loc> - Preview file at location <loc>\n");
    printf("c <loc> - Change location\n");
    printf("e <bytes> - Edit memory at location with <bytes>\n");
    printf("q - Quit (will ask to save changes)\n");
}

// Function to preview memory in hexadecimal format
void preview_memory(unsigned char *buffer, size_t size, size_t loc, size_t preview_size) {
    printf("Previewing memory at location 0x%zx:\n", loc);
    if (loc < size) {
        printf("%02x\n", buffer[loc]);
    } else {
        printf("Location out of bounds\n");
    }
}

// Function to extend the file buffer by allocating more memory
void extend_buffer(unsigned char **buffer, size_t *size, size_t new_size) {
    *buffer = realloc(*buffer, new_size);
    if (*buffer == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }
    *size = new_size;
}

// Function to save the buffer to file
void save_file(const char *filename, unsigned char *buffer, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for saving");
        exit(1);
    }
    fwrite(buffer, 1, size, file);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    // Open the file for reading and writing
    FILE *file = fopen(argv[1], "rb+");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Load file content into buffer
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    unsigned char *buffer = malloc(file_size);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, file_size, file);
    fclose(file);

    size_t buffer_size = file_size;

    // Command loop
    char command[256];
    size_t loc = 0;

    while (1) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) {
            break;
        }

        // Remove trailing newline
        command[strcspn(command, "\n")] = 0;

        // Handle commands
        if (strncmp(command, "ab", 2) == 0) {  // Allocate bytes
            size_t num;
            if (sscanf(command + 2, "%zu", &num) == 1) {
                extend_buffer(&buffer, &buffer_size, buffer_size + num);
                printf("Allocated %zu bytes. New size: %zu bytes\n", num, buffer_size);
            }
        } else if (strncmp(command, "adb", 3) == 0) {  // Allocate double bytes (16 bytes)
            size_t num;
            if (sscanf(command + 3, "%zu", &num) == 1) {
                size_t alloc_size = num * 16;
                extend_buffer(&buffer, &buffer_size, buffer_size + alloc_size);
                printf("Allocated %zu double bytes (16 * %zu). New size: %zu bytes\n", num, num, buffer_size);
            }
        } else if (strncmp(command, "abs", 3) == 0) {  // Allocate bits (8 bits = 1 byte)
            size_t num;
            if (sscanf(command + 3, "%zu", &num) == 1) {
                size_t alloc_size = (num + 7) / 8;  // Convert bits to bytes
                extend_buffer(&buffer, &buffer_size, buffer_size + alloc_size);
                printf("Allocated %zu bits. New size: %zu bytes\n", num, buffer_size);
            }
        } else if (strcmp(command, "p") == 0) {  // Preview entire file
            printf("Previewing entire file:\n");
            for (size_t i = 0; i < buffer_size; i++) {
                if (i % 16 == 0 && i != 0) {
                    printf("\n");
                }
                printf("%02x ", buffer[i]);
            }
            printf("\n");
        } else if (strncmp(command, "p ", 2) == 0) {  // Preview specific location
            size_t preview_loc;
            if (sscanf(command + 2, "%zx", &preview_loc) == 1) {
                preview_memory(buffer, buffer_size, preview_loc, 1); // Show one byte at specific location
            }
        } else if (strncmp(command, "c ", 2) == 0) {  // Change location
            size_t new_loc;
            if (sscanf(command + 2, "%zx", &new_loc) == 1) {
                if (new_loc < buffer_size) {
                    loc = new_loc;
                    printf("Location changed to 0x%zx\n", loc);
                } else {
                    printf("Location out of bounds\n");
                }
            }
        } else if (strncmp(command, "e ", 2) == 0) {  // Edit at current location
            unsigned char edit_bytes[256];
            int count = 0;
            char *ptr = command + 2;
            while (sscanf(ptr, "%2hhx", &edit_bytes[count]) == 1) {
                ptr += 2;
                count++;
            }

            for (int i = 0; i < count; i++) {
                if (loc + i < buffer_size) {
                    buffer[loc + i] = edit_bytes[i];
                } else {
                    extend_buffer(&buffer, &buffer_size, loc + i + 1);
                    buffer[loc + i] = edit_bytes[i];
                }
            }
            loc += count;  // Move location forward after editing
            printf("Edited %d bytes starting from 0x%zx\n", count, loc - count);
        } else if (strcmp(command, "q") == 0) {  // Quit
            char save_choice;
            printf("Save buffer? y/n [yes]: ");
            if (scanf(" %c", &save_choice) != 1) {
                break;
            }

            if (save_choice == 'y' || save_choice == 'Y') {
                save_file(argv[1], buffer, buffer_size);
                printf("Saving\n");
            } else {
                printf("Not saving changes\n");
            }
            break;
        } else if (strcmp(command, "h") == 0) {
            show_help();
        } else {
            printf("Unknown command. Type 'h' for help.\n");
        }
    }

    free(buffer);
    return 0;
}

