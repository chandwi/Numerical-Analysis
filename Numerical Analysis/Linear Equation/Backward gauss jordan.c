#include <stdio.h>
#include <math.h>

#define L 1.0
#define alpha 45.0
#define beta 150.0
#define T0 25.0
#define T_inf 10.0

#define Q(x) (100 * exp(-0.5 * pow((6*x/L - 3), 2)))

void gauss_jordan(double a[][10], double b[], int n) {
    int i, j, k;
    double temp;
    
    for (i = 0; i < n; i++) {
        temp = a[i][i];
        for (j = 0; j < n; j++) {
            a[i][j] /= temp;
        }
        b[i] /= temp;
        
        for (j = 0; j < n; j++) {
            if (j != i) {
                temp = a[j][i];
                for (k = 0; k < n; k++) {
                    a[j][k] -= a[i][k] * temp;
                }
                b[j] -= b[i] * temp;
            }
        }
    }
}

int main() {
    int n, i;
    printf("Enter the number of intervals (n): ");
    scanf("%d", &n);
    
    double dx = L / n;
    double a[10][10] = {0};
    double b[10] = {0};

    for (i = 1; i < n; i++) {
        double x = i * dx;
        a[i][i-1] = alpha / (dx * dx);
        a[i][i] = -2 * alpha / (dx * dx);
        a[i][i+1] = alpha / (dx * dx);
        b[i] = -Q(x);
    }
    
    a[n-1][n-2] = -alpha / dx;
    a[n-1][n-1] = alpha / dx + beta;
    b[n-1] = -beta * T_inf;
    
    gauss_jordan(a, b, n);
    
    printf("Temperature values:\n");
    for (i = 0; i < n; i++) {
        printf("T[%d] = %lf\n", i, b[i]);
    }

    return 0;
}
