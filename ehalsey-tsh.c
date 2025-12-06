#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error() {
    printf("[Error] Oopsie daisies! Please enter the command correctly.\n");
}

// Convert decimal to binary. See the section II-B.
char * d2b(int dec) {
    int quotient = dec, count = 0;
    static char bin[100];

    if (quotient == 0) {
        strcpy(bin, "0"); // if it is 0 just return 0
        return bin;
    }

    while (quotient > 0) {
        bin[count++] = (quotient % 2) + '0'; // remainder is 1 or 0, store as char in array to make binary string
        quotient /= 2; // continuing to next part of binary number by dividing quotient by 2 and looping
    }
    bin[count] = '\0'; // finishing off char array with null pointer

    for (int i = 0; i < count / 2; i++) { // reversing the array to return correctly
        char temp = bin[i];
        bin[i] = bin[count - i - 1];
        bin[count - i - 1] = temp;
    }
    return bin; // only method that returns something because i use it later in gcd
}

// Convert binary to decimal. See the section II-B.
void b2d(char binArr[]) {
    int power = strlen(binArr) - 1, decimal = 0;

    for (int i = 0; binArr[i] != '\0'; i++) {
        decimal += (binArr[i] - '0') * (1 << power); // bit shift for 2^n
        power -= 1; // decrement the exponent
    }
    printf("%i\n", decimal);
}

// Encrypt user’s input based on Caesar’s cipher. See the section II-C.
void enc(char str[]) {
    char encrypted[strlen(str) + 1]; // added 1 to account for null pointer
    for (int i = 0; i < strlen(str); i++) {
        unsigned char ch = str[i]; // unsigned to make CLion stop yelling at me
        encrypted[i] = (ch + 5) % 128; // shift 5 and mod 128 to account for all ASCII characters
    }
    encrypted[strlen(str)] = '\0'; // add in null terminator to end array
    printf("%s\n", encrypted);
}

// Decrypt user’s input based on Caesar’s cipher. See the section II-C.
void dec(char str[]) {
    char decrypted[strlen(str) + 1]; // added 1 to account for null pointer
    for (int i = 0; i < strlen(str); i++) {
        unsigned char ch = str[i]; // unsigned to make CLion stop yelling at me
        decrypted[i] = (ch - 5 + 128) % 128; // shift 5 and mod 128 to account for all ASCII characters, adding 128 to ensure it doesn't go negative
    }
    decrypted[strlen(str)] = '\0'; // add in null terminator to end array
    printf("%s\n", decrypted);
}

// Encode a number into a gamma code. See the section II-D. (extra)
void gcd(int gamma) {
    if (gamma <= 0) {
        error(); return;
    }
    if (gamma==1) {
        printf("0\n"); //  gamma code is 0 for 0
        return;
    }

    char * binary = d2b(gamma); // use other method to get binary number

    char offset[strlen(binary) + 1];
    for (int i = 1; i < strlen(binary); i++) {
        offset[i - 1] = binary[i]; // offset binary number's first character to get unary
    }
    offset[strlen(binary) - 1] = '\0';

    char unary[strlen(offset)+1];
    for (int i = 0; i < strlen(offset); i++) {
        unary[i] = '1'; // get unary code
    }
    unary[strlen(offset)] = '0'; // add 0 to complete code
    unary[strlen(offset) + 1] = '\0'; // add null pointer to complete string

    char gammaCode[strlen(offset)+strlen(unary)+1]; // the final code is the offset and unary combined
    strcpy(gammaCode, unary); // copy over unary first
    strcat(gammaCode, offset); // concat offset with unary that is stored in gammacode already
    printf("%s \n" , gammaCode); // done! :)
}

int main(void) {
    char *loginID = getenv("USER"); // extract username from the environment and set it to a variable
    if (loginID == NULL) {
        loginID = getenv("USERNAME"); // for my Windows computer, I do not own a Linux
        if (loginID == NULL) {
            loginID = "unknownID";  // if username can't be found
        }
    }

    char buffer[100];
    while (1) {
        // display the loginid with a $ in front
        printf("%s $ ", loginID);

        fgets(buffer, 100, stdin);  // get the user input from the command line
        buffer[strcspn(buffer, "\n")] = '\0'; // remove new line at end of string from fgets
        char *cmd = strtok(buffer, "(");   // tokenize the input up to "(" as the first delimiter
        if (cmd != NULL && strcmp(cmd, "exit") == 0) { exit(0); }

        char *arg = strtok(NULL, ")"); // getting next token, which should be the content to run the method on

        if (cmd == NULL || arg == NULL) { error(); continue; }

        // compare the first token to the possible list of commands. if one matches, run that command with the input.
        if (strcmp(cmd,"d2b")==0 || strcmp(cmd,"gcd")==0) { // combined because inputs are ints
            char *endptr;
            long num = strtol(arg, &endptr, 10); // checking to see if arg is a base 10 number
            if (strcmp(cmd, "d2b") == 0) {
                if (*endptr != '\0' || num < 0) { error(); continue; } // does not allow 0
                printf("%s\n", d2b(num)); // have to print it because the method returns a char[]
            } else {
                if (*endptr != '\0' || num <= 0) { error(); continue; } // allows for 0
                gcd(num);
            }
        } else if (strcmp(cmd,"b2d")==0) {
            int valid = 1;
            for (int i = 0; arg[i] != '\0'; i++) {
                if (arg[i] != '0' && arg[i] != '1') { valid = 0; break; }
            }
            if (!valid) { error(); continue; }
            b2d(arg);
        } else if (strcmp(cmd,"enc")==0 || strcmp(cmd,"dec")==0) { // combined because inputs are strings
            if (strlen(arg) == 0) { error(); continue; }
            if (strcmp(cmd,"enc")==0) enc(arg);
            else dec(arg);
        } else {
            error();
        }
    }
}
