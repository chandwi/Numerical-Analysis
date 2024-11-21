#include <stdio.h>
#include <math.h>

#define MIN_POINTS 6
#define MAX_POINTS 21
#define N 100             // Number of interpolation points
#define m 3               // Order of polynomial (m = 3 or m = 5)
#define a -10.0           // Lower bound of x
#define b 10.0            // Upper bound of x

// Function f(x) = tanh(x)
double f(double x) {
    return tanh(x);
}

// Lagrange Polynomial Interpolation for m = 3 (Cubic) and m = 5 (Quintic)
double lagrange_interpolation(double X_j, double x[], double y[], int n) {
    double result = 0.0;
    int i, j;
    int order = (m == 3) ? 4 : 6;  // If m=3, use 4 points, if m=5, use 6 points
    
    // Determine the range for interpolation
    int start = 0, end = n - 1;
    
    if (X_j < x[1]) {
        // Use first 4 points if X_j is near the start
        start = 0;
        end = 3;
    } else if (X_j > x[n-2]) {
        // Use last 4 points if X_j is near the end
        start = n - order;
        end = n - 1;
    } else {
        // Find the range where X_j lies and use 4 or 6 points for interpolation
        for (i = 1; i < n - 1; i++) {
            if (X_j > x[i-1] && X_j <= x[i+1]) {
                start = i - 1;
                end = i + 2;
                break;
            }
        }
    }

    // Perform Lagrange interpolation for the selected range of points
    for (i = start; i <= end; i++) {
        double term = y[i];
        for (j = start; j <= end; j++) {
            if (i != j) {
                term *= (X_j - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    
    return result;
}

int main() {
    double x[MAX_POINTS], y[MAX_POINTS];
    double X_j, result;

    // Generate equispaced x values from [-10, 10]
    int n = MAX_POINTS;  // You can adjust this to any value between 6 and 21
    double h = (b - a) / (n - 1);  // Step size
    for (int i = 0; i < n; i++) {
        x[i] = a + i * h;
        y[i] = f(x[i]);  // y_i = tanh(x_i)
    }

    // Output interpolated values at X_j (j in [0, N])
    printf("Interpolated values:\n");
    for (int j = 0; j < N; j++) {
        X_j = a + j * (b - a) / (N - 1);  // Equally spaced query points
        result = lagrange_interpolation(X_j, x, y, n);
        printf("X_j = %.2f, Interpolated value = %.6f\n", X_j, result);
    }

    return 0;
}
