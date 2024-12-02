#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <float.h>   
#include <time.h> 
#include <math.h>  

extern void MCO2_asm(int vector_length, double* input_X, double* input_Y, double* output_Z_asm, double scalar_A);

void MCO2_C(double scalar_A, double *input_X, double *input_Y, double *output_Z_c, int vector_length) {
    for (int i = 0; i < vector_length; i++) {
        output_Z_c[i] = scalar_A * input_X[i] + input_Y[i];
    }
}

int read_double(double *result) {
    char buffer[100];
    char *parse_end;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

        if (strlen(buffer) == 0 || strchr(buffer, '.') == NULL) {
            return 0; // Invalid double format
        }

        *result = strtod(buffer, &parse_end);

        // Check for valid double conversion within range
        if ((*parse_end == '\0' || *parse_end == '\n') && *result >= DBL_MIN && *result <= DBL_MAX) {
            return 1;
        }
    }
    return 0;
}

int read_int(int *result) {
    char buffer[100];
    char *parse_end;

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

        if (strlen(buffer) == 0) {
            return 0; // Empty input
        }

        *result = strtol(buffer, &parse_end, 10);

        return (*parse_end == '\0' || *parse_end == '\n'); // Successful conversion
    }
    return 0;
}

int compare_double(double a, double b, double tolerance) {
    return fabs(a - b) <= tolerance;
}

void check_results(double *output_Z_c, double *output_Z_asm, int vector_length) {
    double tolerance = 1e-9;  // Increased precision for double
    int match = 1;           // Assume results match initially

    for (int i = 0; i < vector_length; i++) {
        if (!compare_double(output_Z_c[i], output_Z_asm[i], tolerance)) {
            printf("Difference at index %d: C = %.12f, ASM = %.12f\n", i, output_Z_c[i], output_Z_asm[i]);
            match = 0;
        }
    }

    if (match) {
        printf("All results match within tolerance.\n");
    } else {
        printf("Discrepancies found in results.\n");
    }
}

int main() {
    int vector_length;
    double scalar_A;

    printf("Enter scalar (double-precision float): ");
    while (!read_double(&scalar_A)) {
        printf("Invalid input for scalar. Please enter a valid double-precision float: ");
    }

    printf("Enter size of the vectors: ");
    while (!read_int(&vector_length) || vector_length <= 0) {
        printf("Invalid input for size. Please enter a positive integer: ");
    }

    double *input_X = (double *)malloc(vector_length * sizeof(double));
    double *input_Y = (double *)malloc(vector_length * sizeof(double));
    double *output_Z_c = (double *)malloc(vector_length * sizeof(double));  // Results from C implementation
    double *output_Z_asm = (double *)malloc(vector_length * sizeof(double)); // Results from ASM implementation

    if (!input_X || !input_Y || !output_Z_c || !output_Z_asm) {
        printf("Memory allocation failed!\n");
        return 0;
    }

    printf("Enter elements for vector X (size %d):\n", vector_length);
    for (int i = 0; i < vector_length; i++) {
        printf("X[%d] = ", i);
        while (!read_double(&input_X[i])) {
            printf("Invalid input for X[%d]. Please enter a valid double: ", i);
        }
    }

    printf("Enter elements for vector Y (size %d):\n", vector_length);
    for (int i = 0; i < vector_length; i++) {
        printf("Y[%d] = ", i);
        while (!read_double(&input_Y[i])) {
            printf("Invalid input for Y[%d]. Please enter a valid double: ", i);
        }
    }

    // Measure and execute assembly implementation
    clock_t start_asm = clock();
    MCO2_asm(vector_length, input_X, input_Y, output_Z_asm, scalar_A);
    clock_t end_asm = clock();
    printf("ASM DAXPY completed in %.6f seconds.\n", (double)(end_asm - start_asm) / CLOCKS_PER_SEC);

    // Measure and execute C implementation
    clock_t start_c = clock();
    MCO2_C(scalar_A, input_X, input_Y, output_Z_c, vector_length);
    clock_t end_c = clock();
    printf("C DAXPY completed in %.6f seconds.\n", (double)(end_c - start_c) / CLOCKS_PER_SEC);

    // Print first few elements of both results
    printf("\nFirst 10 results from ASM DAXPY:\n");
    for (int i = 0; i < (vector_length < 10 ? vector_length : 10); i++) {
        printf("%.1f ", output_Z_asm[i]);
    }
    printf("\n");

    printf("First 10 results from C DAXPY:\n");
    for (int i = 0; i < (vector_length < 10 ? vector_length : 10); i++) {
        printf("%.1f ", output_Z_c[i]);
    }
    printf("\n");

    // Validate results
    check_results(output_Z_c, output_Z_asm, vector_length);

    free(input_X);
    free(input_Y);
    free(output_Z_c);
    free(output_Z_asm);

    return 0;
}
