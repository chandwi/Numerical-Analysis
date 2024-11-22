#include <stdio.h>
#include <math.h>

#define M 15    // Number of data points (11 ≤ M ≤ 21)
#define N 5     // Order of the polynomial (n ≤ 5)
#define P 100   // Number of interpolation points

// Define the nonlinear function
double f(double x) {
    return sin(x) - (1 / sqrt(10)) * sin(sqrt(10) * x) + 0.1 * sin(10 * x);
}

// Function to calculate the Vandermonde matrix
void vandermonde_matrix(double x[], double V[][N + 1], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= N; j++) {
            V[i][j] = pow(x[i], j);
        }
    }
}

// Function to multiply matrices
void matrix_multiply(double A[][N + 1], double B[], double result[], int rows, int cols) {
    for (int i = 0; i < cols; i++) {
        result[i] = 0;
        for (int j = 0; j < rows; j++) {
            result[i] += A[j][i] * B[j];
        }
    }
}

// Solve for polynomial coefficients using normal equations
void least_squares(double V[][N + 1], double y[], double coefficients[]) {
    double Vt[N + 1][M], VtV[N + 1][N + 1], Vty[N + 1];
    
    // Transpose of Vandermonde matrix
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j < M; j++) {
            Vt[i][j] = V[j][i];
        }
    }

    // Calculate VtV = V^T * V
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            VtV[i][j] = 0;
            for (int k = 0; k < M; k++) {
                VtV[i][j] += Vt[i][k] * V[k][j];
            }
        }
    }

    // Calculate Vty = V^T * y
    for (int i = 0; i <= N; i++) {
        Vty[i] = 0;
        for (int j = 0; j < M; j++) {
            Vty[i] += Vt[i][j] * y[j];
        }
    }

    // Solve for coefficients using back-substitution
    for (int i = 0; i <= N; i++) {
        coefficients[i] = Vty[i] / VtV[i][i];
    }
}

// Evaluate the Lagrange polynomial
double lagrange_poly(double coefficients[], double x) {
    double result = 0;
    for (int i = 0; i <= N; i++) {
        result += coefficients[i] * pow(x, i);
    }
    return result;
}

int main() {
    double x_data[M], y_data[M];
    double V[M][N + 1];
    double coefficients[N + 1];
    double x_interp[P], y_interp[P];
    double x_min = -M_PI / 2, x_max = 3 * M_PI / 2;

    // Generate equispaced input data points
    for (int i = 0; i < M; i++) {
        x_data[i] = x_min + i * (x_max - x_min) / (M - 1);
        y_data[i] = f(x_data[i]);
    }

    // Create Vandermonde matrix
    vandermonde_matrix(x_data, V, M);

    // Solve for coefficients using least squares
    least_squares(V, y_data, coefficients);

    // Generate equispaced interpolation points
    for (int i = 0; i < P; i++) {
        x_interp[i] = x_min + i * (x_max - x_min) / (P - 1);
        y_interp[i] = lagrange_poly(coefficients, x_interp[i]);
    }

    // Output interpolated values
    printf("Interpolated values:\n");
    for (int i = 0; i < P; i++) {
        printf("(%.4f, %.4f)\n", x_interp[i], y_interp[i]);
    }

    return 0;
}
