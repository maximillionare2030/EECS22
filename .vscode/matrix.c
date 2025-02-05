#include <stdio.h>


void getUserInput(int array[2][2], int row, int column, const char* name) {
    int input = 0;
    int result = 0; // Result indicator for input

    while (result != 1) { // Loop until valid input is entered: EXTRA CREDIT
        printf("%s%d%d = ", name, row + 1, column + 1);
        result = scanf("%d", &input);

        if (result == 1) {
            array[row][column] = input;
            break;
        } else {
            printf("ERROR: Invalid input.  Please enter numbers only!\n");
            while (getchar() != '\n');
        }
    }
}

int main() {
    // Declaring matrices
    int a[2][2] = {{0,0}, {0,0}};
    int b[2][2] = {{0,0}, {0,0}};

    // Prompting for first matrix
    printf("Enter the first matrix(a) that will be multiplied:\n");

    // Looping through rows and columns of matrix
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            getUserInput(a, i, j,"a");
        }
    }
    printf("\n\n");

    // Repeating process above for Matrix B
    printf("Enter the first matrix(b) that will be multiplied:\n");

    for(int i =0; i < 2; i++) {
        for (int j =0; j< 2; j++) {
            getUserInput(b, i, j, "b");
        }
    }

    int result[2][2] = {{0,0}, {0,0}};

    // Providing Matrix Multiplication Results
    printf("Result: \n");

    result[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
    result[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1];
    result[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0];
    result[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1];

    printf("%d %d\n", result[0][0], result [0][1]);
    printf("%d %d\n", result[1][0], result [1][1]);

    return 0;
}
