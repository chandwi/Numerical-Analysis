#include <stdio.h>
#include <math.h>

#define NUM_POINTS 21     // Number of data points
#define N 100             // Number of interpolation points
#define m 5               // Order of the difference
#define a -10.0           // Lower bound of x
#define b 10.0            // Upper bound of x

// Function f(x) = tanh(x)
double f(double x) {
    return tanh(x);
}

// Factorial function (used for higher powers of p)
int factorial(int n) {
    int fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

// Compute Forward Difference table
void compute_forward_diff(double x[], double y[], double diff[][NUM_POINTS]) {
    for (int i = 0; i < NUM_POINTS; i++) {
        diff[i][0] = y[i];  // First column is just the function values
    }

    for (int j = 1; j < m; j++) {
        for (int i = 0; i < NUM_POINTS - j; i++) {
            diff[i][j] = diff[i+1][j-1] - diff[i][j-1];
        }
    }
}

// Interpolate using Forward Difference method
double forward_difference_interpolation(double X_j, double x[], double diff[][NUM_POINTS]) {
    int i;
    double p, result = 0.0;
    for (i = 0; i < NUM_POINTS - m; i++) {
        if (X_j >= x[i] && X_j < x[i+1]) {
            p = (X_j - x[i]) / (x[i+1] - x[i]);
            result = diff[i][0];
            for (int j = 1; j <= m; j++) {
                result += diff[i][j] * (pow(p, j) / factorial(j));
            }
            break;
        }
    }
    if (i == NUM_POINTS - m) {
        // If X_j is close to the rightmost side, use the last row's coefficients
        p = (X_j - x[NUM_POINTS - m]) / (x[NUM_POINTS - 1] - x[NUM_POINTS - m]);
        result = diff[NUM_POINTS - m][0];
        for (int j = 1; j <= m; j++) {
            result += diff[NUM_POINTS - m][j] * (pow(p, j) / factorial(j));
        }
    }
    return result;
}

int main() {
    double x[NUM_POINTS], y[NUM_POINTS], diff[NUM_POINTS][NUM_POINTS];
    double X_j, result;
    
    // Generate equispaced x values from [-10, 10]
    double h = (b - a) / (NUM_POINTS - 1);  // Step size
    for (int i = 0; i < NUM_POINTS; i++) {
        x[i] = a + i * h;
        y[i] = f(x[i]);  // y_i = tanh(x_i)
    }

    // Compute the Forward Difference table
    compute_forward_diff(x, y, diff);

    // Output interpolated values at X_j (j in [0, N])
    printf("Interpolated values:\n");
    for (int j = 0; j < N; j++) {
        X_j = a + j * (b - a) / (N - 1);  // Equally spaced query points
        result = forward_difference_interpolation(X_j, x, diff);
        printf("X_j = %.2f, Interpolated value = %.6f\n", X_j, result);
    }

    return 0;
}
