#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <math.h>

extern void MCO2_asm(int n, double* X, double* Y, double* Z, double A);

void MCO2_C(double A, double *X, double *Y, double *Z_c, int n) {
    for (int i = 0; i < n; i++) {
        Z_c[i] = A * X[i] + Y[i];
    }
}

int read_double(double *value) {
    char buffer[100];
    char *endptr;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            return 0;
        }

        if (strchr(buffer, '.') == NULL) {
            return 0;
        }

        *value = strtod(buffer, &endptr);

        if ((*endptr == '\0' || *endptr == '\n') && *value >= DBL_MIN && *value <= DBL_MAX) {
            return 1;
        }
    }
    return 0;
}

int read_int(int *value) {
    char buffer[100];
    char *endptr;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            return 0;
        }

        *value = strtol(buffer, &endptr, 10);

        return (*endptr == '\0' || *endptr == '\n');
    }
    return 0;
}

int compare_double(double a, double b, double tolerance) {
    return fabs(a - b) <= tolerance;
}

void check_correctness(double *Z_c, double *Z, int n) {
    double tolerance = 1e-9;  // Precision threshold
    int correct = 1;          // Start by assuming results match

    for (int i = 0; i < n; i++) {
        if (!compare_double(Z_c[i], Z[i], tolerance)) {
            printf("Difference found at index %d: C result = %.12f, ASM result = %.12f\n", i, Z_c[i], Z[i]);
            correct = 0;
        }
    }

    if (correct) {
        printf("All results match within the specified tolerance.\n");
    } else {
        printf("Discrepancies detected in the results.\n");
    }
}

void run_daxpy_for_size(int n, double A, double *X, double *Y, double *Z_c, double *Z) {
    // Timing the assembly DAXPY kernel
    clock_t start_asm = clock();  // Start time for ASM version
    printf("x86-64 kernel started (clock ticks): %ld\n", start_asm);

    // (ASM version)
    MCO2_asm(n, X, Y, Z, A);

    clock_t end_asm = clock();  // End time for ASM version
    printf("x86-64 kernel finished (clock ticks): %ld\n", end_asm);

    double time_taken_asm = (double)(end_asm - start_asm) / CLOCKS_PER_SEC;
    printf("x86-64 DAXPY kernel completed in %.6f seconds.\n", time_taken_asm);

    printf("\nFirst 10 values of Z (ASM result):\n");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("%.1f ", Z[i]);
    }
    printf("\n");

    // Timing the C DAXPY kernel
    clock_t start_c = clock();  // Start time for C version
    printf("\nC kernel started (clock ticks): %ld\n", start_c);

    // (C version)
    MCO2_C(A, X, Y, Z_c, n);

    clock_t end_c = clock();  // End time for C version
    printf("C kernel finished (clock ticks): %ld\n", end_c);

    double time_taken_c = (double)(end_c - start_c) / CLOCKS_PER_SEC;
    printf("C DAXPY kernel completed in %.6f seconds.\n", time_taken_c);

    printf("\nFirst 10 values of Z_c (C result):\n");
    for (int i = 0; i < (n < 10 ? n : 10); i++) {
        printf("%.1f ", Z_c[i]);
    }
    printf("\n");
    printf("\n");

    // Validate that both implementations produce the same results
    check_correctness(Z_c, Z, n);
}

int main() {
    // Sample scalar A and vector size configurations
    double A = 2.0;  // Adjustable scalar value

    // Define sizes for n (2^20, 2^24, 2^28, etc.)
    int sizes[] = {1 << 20, 1 << 24, 1 << 28}; // Adjust 2^30 if memory limits are reached

    for (int i = 0; i < 3; i++) {
        int n = sizes[i];

        printf("\nExecuting DAXPY operation for vector size n = %d\n", n);

        // Allocate memory for vectors
        double *X = (double *)malloc(n * sizeof(double));
        double *Y = (double *)malloc(n * sizeof(double));
        double *Z_c = (double *)malloc(n * sizeof(double));  // For C result
        double *Z = (double *)malloc(n * sizeof(double));    // For ASM result

        if (!X || !Y || !Z_c || !Z) {
            printf("Error: Memory allocation failed!\n");
            return 0;
        }

        // Initialize vectors with random data
        for (int j = 0; j < n; j++) {
            X[j] = (double)(rand() % 10000000);  // Random values
            Y[j] = (double)(rand() % 10000000);  // Random values
        }

        // Execute and time the DAXPY implementations
        run_daxpy_for_size(n, A, X, Y, Z_c, Z);

        // Free allocated memory
        free(X);
        free(Y);
        free(Z_c);
        free(Z);
    }

    return 0;
}
