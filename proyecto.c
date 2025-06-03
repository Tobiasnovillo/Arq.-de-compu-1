#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Include termios, unistd, and fcntl only on Linux-like systems
#ifdef __linux__
#include <termios.h> // Required for terminal control
#include <unistd.h>  // Required for read
#include <fcntl.h>   // Required for fcntl
#endif

// Global variable to store original terminal settings (only used on Linux)
#ifdef __linux__
struct termios original_termios;
#endif

// Global variable for the shared delay value
unsigned long int delay_value = 500000000; // Initial shared delay value

// Function to set terminal to non-canonical, no-echo mode
void set_non_canonical_mode() {
#ifdef __linux__
    struct termios new_termios;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &original_termios);

    // Copy settings to new_termios
    new_termios = original_termios;

    // Set to non-canonical mode (ICANON disables line buffering)
    // Set to no-echo mode (ECHO disables echoing input characters)
    new_termios.c_lflag &= ~(ICANON | ECHO);

    // Minimum number of characters to read (VMIN) and timeout (VTIME)
    // Set VMIN = 0 and VTIME = 0 for non-blocking read
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    // Apply new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    // Set stdin to non-blocking mode
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
#else
    // Placeholder for Windows - no terminal control needed for basic stdio
#endif
}

// Function to restore original terminal settings
void restore_canonical_mode() {
#ifdef __linux__
    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);

    // Restore stdin to blocking mode
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK);
#else
    // Placeholder for Windows
#endif
}

// Function to handle password access
bool check_password() {
    char password[6]; // 5 digits + null terminator
    const char correct_password[] = "12345"; // Define your password here
    int attempts = 0;
    const int max_attempts = 3;

    printf("Implementacion del control de acceso al sistema mediante password.\n");

    while (attempts < max_attempts) {
        printf("Ingrese su password de 5 digitos: ");
        fflush(stdout); // Ensure prompt is displayed

#ifdef __linux__
        // Linux: Read password character by character and display '*', handle termios
        int i;
        char ch;
        struct termios password_termios;
        struct termios original;
        tcgetattr(STDIN_FILENO, &original); // Save original settings

        password_termios = original;
        password_termios.c_lflag &= ~(ICANON | ECHO); // Non-canonical, no echo
        password_termios.c_cc[VMIN] = 1; // Read at least 1 character
        password_termios.c_cc[VTIME] = 0; // No timeout

        tcsetattr(STDIN_FILENO, TCSANOW, &password_termios);

        for (i = 0; i < 5; ++i) {
            read(STDIN_FILENO, &ch, 1); // Blocking read for one character
            password[i] = ch;
            printf("*"); // Display '' for the entered character
            fflush(stdout);
        }
        password[i] = '\0'; // Null-terminate the string

        // Restore original terminal settings (non-canonical, non-blocking)
        tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);

        // Clear any remaining input (like newline)
        while (read(STDIN_FILENO, &ch, 1) > 0 && ch != '\n');

        printf("\n"); // Add newline after '' sequence

#else
        // Windows: Use fgets for simpler input (no '' masking)
        if (fgets(password, sizeof(password), stdin) != NULL) {
            // Remove newline character if present
            password[strcspn(password, "\n")] = 0;
        } else {
            // Handle potential input errors
            fprintf(stderr, "Error reading input.\n");
            break;
        }
#endif

        // Check if the entered password matches the correct password
        if (strcmp(password, correct_password) == 0) {
            printf("Bienvenido al Sistema\n");
            return true; // Password is correct
        } else {
            printf("Password no valida\n");
            attempts++;
            if (attempts < max_attempts) {
                printf("Intentos restantes: %d\n", max_attempts - attempts);
            }
        }
    }

    printf("Demasiados intentos. El programa aborta.\n");
    return false; // Password attempts exhausted
}

// Delay function based on decrementing a counter
void retardo(unsigned long int a) {
    while (a) {
        a--;
    }
}

// Function to simulate the "El auto fantastico" light sequence (algorithm)
void run_auto_fantastico() {
    unsigned int lights = 0x80; // Start with the leftmost light on (assuming 8 lights)
    int direction = -1; // -1 for left, 1 for right
    char ch;

#ifdef __linux__
    printf("Press 'q' to return to menu. Use up/down arrows to change speed.\n");
#else
    printf("Running El auto fantastico. Press Ctrl+C to stop.\n");
#endif

    while (1) {
        printf("\r[");
        for (int i = 7; i >= 0; i--) {
            if ((lights >> i) & 1) {
                printf("X");
            } else {
                printf("-");
            }
        }
        printf("] Speed: %lu ", delay_value); // Display current speed/delay
        fflush(stdout); // Update the displayed line

        retardo(delay_value); // Use the global delay value

#ifdef __linux__
        // Linux: Check for keyboard input without blocking and handle speed/exit
        ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
        if (bytesRead > 0) {
            if (ch == 'q') {
                printf("\nReturning to menu...\n");
                break;
            } else if (ch == '\033') {
                char sequence[2];
                if (read(STDIN_FILENO, sequence, 2) == 2) {
                    if (sequence[0] == '[') {
                        if (sequence[1] == 'A') {
                            if (delay_value > 10000) {
                                delay_value -= 10000;
                            }
                        } else if (sequence[1] == 'B') {
                            delay_value += 10000;
                        }
                    }
                }
            }
        }
#else
        // Windows: No interactive input during sequence
#endif

        // Move the light
        if (direction == -1) {
            lights >>= 1;
            if (lights == 0x01) {
                direction = 1;
            }
        } else {
            lights <<= 1;
            if (lights == 0x80) {
                direction = -1;
            }
        }
    }
    printf("\n");
    // TODO: Consider how to handle lights state after exiting a sequence (turn all off?)
}

// Data table for "El choque" sequence (assuming 8 lights)
// Each element represents the state of 8 lights (e.g., binary 11000011)
unsigned int el_choque_sequence[] = {
    0x81, // 10000001
    0x42, // 01000010
    0x24, // 00100100
    0x18, // 00011000
    0x18, // 00011000
    0x24, // 00100100
    0x42, // 01000010
    0x81  // 10000001
};

// Size of the "El choque" sequence data table
const int el_choque_sequence_len = sizeof(el_choque_sequence) / sizeof(el_choque_sequence[0]);

// Function to simulate the "El choque" light sequence (data table)
void run_el_choque() {
    char ch;
    int i = 0;

#ifdef __linux__
    printf("Press 'q' to return to menu. Use up/down arrows to change speed.\n");
#else
    printf("Running El choque. Press Ctrl+C to stop.\n");
#endif

    while (1 /* TODO: Add condition to check for exit key press */) {
        printf("\r[");
        for (int j = 7; j >= 0; j--) {
            if ((el_choque_sequence[i] >> j) & 1) {
                printf("X");
            } else {
                printf("-");
            }
        }
        printf("] Speed: %lu ", delay_value); // Display current speed/delay
        fflush(stdout); // Update the displayed line

        retardo(delay_value); // Use the global delay value

        // Move to the next state in the data table (loop back to the beginning)
        i = (i + 1) % el_choque_sequence_len;

#ifdef __linux__
        // Linux: Check for keyboard input without blocking and handle speed/exit
        ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);
        if (bytesRead > 0) {
            if (ch == 'q') {
                printf("\nReturning to menu...\n");
                break;
            } else if (ch == '\033') {
                char sequence[2];
                if (read(STDIN_FILENO, sequence, 2) == 2) {
                    if (sequence[0] == '[') {
                        if (sequence[1] == 'A') {
                            if (delay_value > 10000) {
                                delay_value -= 10000;
                            }
                        } else if (sequence[1] == 'B') {
                            delay_value += 10000;
                        }
                    }
                }
            }
        }
#else
        // Windows: No interactive input during sequence
#endif
        // TODO: Consider how to handle lights state after exiting a sequence (turn all off?)
    }
    printf("\n");
    // TODO: Consider how to handle lights state after exiting a sequence (turn all off?)
}

int main() {
    // Set terminal to non-canonical mode before password check and menu (Linux only)
    set_non_canonical_mode();

    // Ensure terminal is restored on program exit (Linux only, requires atexit from stdlib.h)
    // #ifdef __linux__
    // atexit(restore_canonical_mode);
    // #endif

    if (!check_password()) {
        // Restore terminal and exit if password is incorrect (Linux only)
        restore_canonical_mode();
        return 1; // Indicate failure
    }

    int choice;
    bool running = true;

    while (running) {
        // Display menu options
        printf("\nMenu de Secuencias de Luces:\n");
        printf("1. El auto fantastico\n"); // Common sequence (algorithm)
        printf("2. El choque\n");       // Common sequence (data table)
        // Placeholders for the two group-specific sequences
        printf("3. Secuencia propia (Algoritmo)\n");
        printf("4. Secuencia propia (Tabla de datos)\n");
        printf("5. Salir\n");
        printf("Ingrese su eleccion: ");

#ifdef __linux__
        // Linux: Temporarily restore canonical mode for scanf, then switch back
        restore_canonical_mode();
        if (scanf("%d", &choice) != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            while (getchar() != '\n'); // Clear input buffer
            set_non_canonical_mode(); // Switch back to non-canonical
            continue; // Ask for input again
        }
        while (getchar() != '\n'); // Clear rest of input line
        set_non_canonical_mode(); // Switch back to non-canonical

#else
        // Windows: Use standard scanf
        if (scanf("%d", &choice) != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue; // Ask for input again
        }
        while (getchar() != '\n'); // Clear rest of input line
#endif

        // Use a switch statement to handle the choice
        switch (choice) {
            case 1:
                printf("\nEjecutando: El auto fantastico\n");
                run_auto_fantastico(); // Call the function for El auto fantastico
                break;
            case 2:
                printf("\nEjecutando: El choque\n");
                run_el_choque(); // Call the function for El choque
                break;
            case 3:
                printf("\nEjecutando: Secuencia propia (Algoritmo)\n");
                // TODO: Call function for group's algorithm sequence
                break;
            case 4:
                printf("\nEjecutando: Secuencia propia (Tabla de datos)\n");
                // TODO: Call function for group's data table sequence
                break;
            case 5:
                printf("Saliendo del programa.\n");
                running = false; // Exit the loop
                break;
            default:
                printf("Opcion no valida. Por favor intente de nuevo.\n");
        }
    }

    // Restore terminal settings before exiting (Linux only)
    restore_canonical_mode();
    return 0; // Indicate success
}
