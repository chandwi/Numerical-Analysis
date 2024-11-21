#include <stdio.h>
#include <math.h>

#define L 1.0
#define k 45.0
#define h 150.0
#define T0 25.0
#define T_inf 10.0
#define Q(x) (100 * exp(-0.5 * pow((6*x/L - 3), 2)))

void jacobi_method(double a[][10], double b[], double T[], int n, int max_iter, double tolerance) {
    double T_new[10];
    int iter, i;
    double diff;
    
    for (iter = 0; iter < max_iter; iter++) {
        for (i = 1; i < n-1; i++) {
            T_new[i] = (b[i] - a[i][i-1] * T[i-1] - a[i][i+1] * T[i+1]) / a[i][i];
        }
        
        diff = 0.0;
        for (i = 1; i < n-1; i++) {
            diff += fabs(T_new[i] - T[i]);
            T[i] = T_new[i];
        }
        
        if (diff < tolerance) {
            break;
        }
    }
}

int main() {
    int n, i, max_iter = 1000;
    double dx, tolerance = 1e-6;
    printf("Enter the number of intervals (n): ");
    scanf("%d", &n);
    
    dx = L / n;
    double a[10][10] = {0};
    double b[10] = {0};
    double T[10];
    
    for (i = 0; i < n; i++) {
        T[i] = T0;
    }
    
    for (i = 1; i < n-1; i++) {
        double x = i * dx;
        a[i][i-1] = k / (dx * dx);
        a[i][i] = -2 * k / (dx * dx);
        a[i][i+1] = k / (dx * dx);
        b[i] = -Q(x);
    }
    

    a[n-1][n-2] = -k / dx;
    a[n-1][n-1] = k / dx + h;
    b[n-1] = -h * T_inf;
    
    
    jacobi_method(a, b, T, n, max_iter, tolerance);
    

    printf("Temperature values:\n");
    for (i = 0; i < n; i++) {
        printf("T[%d] = %lf\n", i, T[i]);
    }

    return 0;
}
