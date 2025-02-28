#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 10000
#define PRINT_ALL_TERMS 0

// define the bignum struct
typedef struct {
    int bigNumSize;             // holds the size of the bignum in 64-bit blocks
    unsigned long long *num;    // contains the number itsself (format is an array of 64-bit blocks with size of bigNumSize)
    int beginningNumIndex;      // tracks the index of the beginning of the bignum (optimizes speed)
} BigNum;

// format is little endian (Least Significant Bit first)
void addBigNum(BigNum *A, BigNum *B, BigNum *result) {
    // Check which num index is bigger (A or B)
    int maxIndex = 0;
    if (A->beginningNumIndex > B->beginningNumIndex) {
        maxIndex = A->beginningNumIndex;
    } else {
        maxIndex = B->beginningNumIndex;
    }
    
    // Add the two big numbers
    char carry = 0;
    for (int i = 0; i <= maxIndex; i++) {
        result->num[i] = A->num[i] + B->num[i] + carry;
        
        carry = (result->num[i] < A->num[i]);
    }

    // Update beginningNumIndex for the result
    result->beginningNumIndex = maxIndex;

    // Check for extra carry if needed
    if (carry && maxIndex + 1 < result->bigNumSize) {
        result->num[maxIndex + 1] = carry;
        result->beginningNumIndex++; // add another 1 to the beginningNumIndex to account for the new carry
    }
}

// format is little endian (Least Significant Bit first)
void printBigNum(BigNum *number) {
    printf("0x");
    char leadingZero = 1;
    for (int i = number->bigNumSize-1; i >= 0; i--) {
        // start printing after the leading zeros
        if (number->num[i] != 0) {
            leadingZero = 0;
        }
        if (!leadingZero) {
            printf("%llx", number->num[i]);
        }
    }
}

int main(int argc, char **argv) {
    int terms = atoi(argv[1]);

    if (terms < 2) {
        printf("Error: term number must be above 2\n");
        return 1;
    }

    // initialize bignum structs with proper values
    BigNum *t1 = malloc(sizeof(BigNum));
    BigNum *t2 = malloc(sizeof(BigNum));
    BigNum *currentTerm = malloc(sizeof(BigNum));

    t1->bigNumSize = MAX_DIGITS;
    t2->bigNumSize = MAX_DIGITS;
    currentTerm->bigNumSize = MAX_DIGITS;

    t1->num = malloc(MAX_DIGITS * sizeof(unsigned long long));
    t2->num = malloc(MAX_DIGITS * sizeof(unsigned long long));
    currentTerm->num = malloc(MAX_DIGITS * sizeof(unsigned long long));

    t1->beginningNumIndex = 0;
    t2->beginningNumIndex = 0;
    currentTerm->beginningNumIndex = 0;

    // set term 1 and 2 to 0 and 1 respectively
    t1->num[0] = 0;
    t2->num[0] = 1;


    printf("Sequence:\nF(0) = %llu\nF(1) = %llu\n", t1->num[0], t2->num[0]);

    for (int i = 2; i <= terms; i++) {
        // add nums
        addBigNum(t1, t2, currentTerm);

        // print nums
        if (i == terms || PRINT_ALL_TERMS == 1) {
            printf("F(%d) = ", i);
            printBigNum(currentTerm);
            printf("\n");
        }

        // update nums
        BigNum *temp = t1;
        t1 = t2;
        t2 = currentTerm;
        currentTerm = temp;
    }

    printf("END\n");

    // free num in BigNum structs
    free(t1->num);
    free(t2->num);
    free(currentTerm->num);

    // free BigNum structs
    free(t1);
    free(t2);
    free(currentTerm);

    return 0;
}