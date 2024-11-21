#include <stdio.h>
#include <math.h>

// Function for the integrand
float integrand(float x, float y) {
    if ((x * x + y * y) <= 1) {
        return y * y; // Only include y^2 inside the circle
    }
    return 0; // Outside the circle
}

// Function to compute the integral using Simpson's rule
float simpsons_2d(int nx, int ny) {
    float x_min = -1, x_max = 1; // x bounds
    float y_min = -1, y_max = 1; // y bounds

    // Step sizes
    float hx = (x_max - x_min) / nx;
    float hy = (y_max - y_min) / ny;

    float integral = 0;

    // Loop through grid points
    for (int i = 0; i <= nx; i++) {
        for (int j = 0; j <= ny; j++) {
            float x = x_min + i * hx;
            float y = y_min + j * hy;

            // Determine weight for Simpson's rule
            float weight = 1;
            if ((i == 0 || i == nx) && (j == 0 || j == ny)) {
                weight = 1; // Corner points
            } else if (i == 0 || i == nx || j == 0 || j == ny) {
                weight = 4; // Edge points
            } else if (i % 2 == 0 && j % 2 == 0) {
                weight = 16; // Interior points at even indices
            } else if (i % 2 == 0 || j % 2 == 0) {
                weight = 8; // Interior points on mixed even/odd indices
            }

            // Add contribution to the integral
            integral += weight * integrand(x, y);
        }
    }

    // Finalize the integral
    integral *= (hx * hy / 9);
    return integral;
}

int main() {
    int nx, ny;

    printf("Enter the number of intervals in x-direction (nx): \n");
    scanf("%d", &nx);
    if (nx % 2 != 0) {
        nx++;
        printf("nx adjusted to %d (must be even for Simpson's rule)\n", nx);
    }

    printf("Enter the number of intervals in y-direction (ny): \n");
    scanf("%d", &ny);
    if (ny % 2 != 0) {
        ny++;
        printf("ny adjusted to %d (must be even for Simpson's rule)\n", ny);
    }

    float result = simpsons_2d(nx, ny);
    printf("Area moment of inertia Ixx = %f\n", result);

    return 0;
