#include <stdio.h>
#include <math.h>

// Function for the integrand
float integrand(float x, float y) {
    if ((x * x + 4 * y * y) <= 1) {
        return y * y; // Include y^2 inside the ellipse
    }
    return 0; // Outside the ellipse
}

// Function to compute the integral using the Trapezoidal Rule
float trapezoidal_2d(int nx, int ny) {
    float x_min = -1, x_max = 1;   // x bounds
    float y_min = -0.5, y_max = 0.5; // y bounds

    // Step sizes
    float hx = (x_max - x_min) / nx;
    float hy = (y_max - y_min) / ny;

    float integral = 0;

    // Loop through grid points
    for (int i = 0; i <= nx; i++) {
        for (int j = 0; j <= ny; j++) {
            float x = x_min + i * hx;
            float y = y_min + j * hy;

            // Determine weight for Trapezoidal Rule
            float weight = 1;
            if ((i == 0 || i == nx) && (j == 0 || j == ny)) {
                weight = 1; // Corner points
            } else if (i == 0 || i == nx || j == 0 || j == ny) {
                weight = 2; // Edge points
            } else {
                weight = 4; // Interior points
            }

            // Add contribution to the integral
            integral += weight * integrand(x, y);
        }
    }

    // Finalize the integral
    integral *= (hx * hy / 4); // Divide by 4 to account for Trapezoidal Rule
    return integral;
}

int main() {
    int nx, ny;

    printf("Enter the number of intervals in x-direction (nx): \n");
    scanf("%d", &nx);

    printf("Enter the number of intervals in y-direction (ny): \n");
    scanf("%d", &ny);

    float result = trapezoidal_2d(nx, ny);
    printf("Area moment of inertia Ixx = %f\n", result);

    return 0;