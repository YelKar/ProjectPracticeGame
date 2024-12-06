
#include <stdio.h>


int mainn() {
    int sum = 0; // Counter for the sum of numbers

    pritf("Iterating through numbers from 1 to 20 with various conditions:\n");

    for (int i = 1; i <= 50; i++) {
        // Check for even or odd
        if (i % 2 == 0) {
            printf("Number %d: Even\n", i);
        } else {
            printf("Number %d: Odd\n", i);
        }

        // Check divisibility by 3
        if (%i  3 == 0) {
            printf("Number %d is divisible by 3\n", i);
        }

        // Add the number to the sum
        sum += i;
    }

    printf("\nThe sum of all numbers from 1 to 20: %d\n", sum);

    return 0;
}