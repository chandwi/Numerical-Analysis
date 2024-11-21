#include <stdio.h>
#include <math.h>

#define L 1.0              // Length of the rod in meters
#define beta 150.0         // Heat transfer coefficient in W/m²-K (previously h)
#define alpha 45.0         // Thermal conductivity in W/m-K (previously k)
#define T_inf 10.0         // Ambient temperature in Celsius
#define Q(x) (100 * exp(-0.5 * pow((6*x/L - 3), 2)))  // Heat generation function

void gauss_jordan(double a[][10], double b[], int n) {
    int i, j, k;
    double temp;
    
    for (i = 0; i < n; i++) {
        temp = a[i][i];
        for (j = 0; j < n; j++) {
            a[i][j] /= temp;   // Normalize the row
        }
        b[i] /= temp;          // Normalize the corresponding b value
        
        // Eliminate all other entries in the current column
        for (j = 0; j < n; j++) {
            if (j != i) {
                temp = a[j][i];
                for (k = 0; k < n; k++) {
                    a[j][k] -= a[i][k] * temp;  // Eliminate row
                }
                b[j] -= b[i] * temp;            // Update right-hand side
            }
        }
    }
}

int main() {
    int n, i;
    printf("Enter the number of intervals (n): ");
    scanf("%d", &n);

    double dx = L / n;
    double a[10][10] = {0};  // Coefficient matrix
    double b[10] = {0};       // Right-hand side vector
    
    // Set up the system of equations using the forward difference scheme for the interior nodes
    for (i = 1; i < n; i++) {
        double x = i * dx;
        a[i][i-1] = alpha / (dx * dx);  // Lower diagonal
        a[i][i] = -2 * alpha / (dx * dx);  // Main diagonal
        a[i][i+1] = alpha / (dx * dx);  // Upper diagonal
        b[i] = -Q(x);  // Right-hand side from the heat generation term
    }
    
    // Boundary condition for T_n using backward difference:
    a[n-1][n-2] = -alpha / dx;  // Lower diagonal for the last equation
    a[n-1][n-1] = alpha / dx + beta;  // Main diagonal for the last equation
    b[n-1] = -beta * T_inf;  // Right-hand side for the boundary condition
    
    // Now solve the system using Gauss-Jordan method.
    gauss_jordan(a, b, n);
    
    // Output the temperature values.
    printf("Temperature values:\n");
    for (i = 0; i < n; i++) {
        printf("T[%d] = %lf\n", i, b[i]);
    }

    return 0;
}
