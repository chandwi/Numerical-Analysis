#include <stdio.h>
#include <math.h>

#define MAX_ITER 100
#define TOL 1e-6
#define M_PI 3.14159

const double K = 100, n = 10, E = 200000, sigma_yp = 200, C = 10000, gamma = 100, T = 1, epsilon_bar = 0.01;

double strain(double t) {
    return epsilon_bar * sin(2 * M_PI * t / T);
}

double solve_epsilon_p(double epsilon_p0, double alpha0, double t, double epsilon) {
    double h = 0.025;
    double epsilon_p = epsilon_p0;
    double sigma, alpha, f, df, g, dg;
    
    for (int iter = 0; iter < MAX_ITER; iter++) {
        sigma = E * (epsilon - epsilon_p);
        alpha = alpha0 + C * (epsilon_p - epsilon_p0) - gamma * alpha0 * fabs(epsilon_p - epsilon_p0);
        
        if (fabs(sigma - alpha) > sigma_yp) {
            f = pow((fabs(sigma - alpha) - sigma_yp) / K, 1.0 / n);
        } else {
            f = 0;
        }
        
        g = epsilon_p - epsilon_p0 - h * f;
        
        if (fabs(sigma - alpha) > sigma_yp) {
            df = (1.0 / n) * pow((fabs(sigma - alpha) - sigma_yp) / K, (1.0 / n - 1)) * (-E / K);
        } else {
            df = 0;
        }
        
        dg = 1 - h * df;
        
        double delta = g / dg;
        epsilon_p -= delta;
        
        if (fabs(delta) < TOL) {
            return epsilon_p;
        }
    }
    
    return epsilon_p;
}

int main() {
    double t = 0, h = 0.025;
    double epsilon_p0 = 0, alpha0 = 0, epsilon, epsilon_p;

    printf("Time Step\tEpsilon_p\n");
    printf("-----------------------------------\n");
    
    for (t = 0; t <= 10; t += h) {
        epsilon = strain(t);
        epsilon_p = solve_epsilon_p(epsilon_p0, alpha0, t, epsilon);
        alpha0 = alpha0 + C * (epsilon_p - epsilon_p0) - gamma * alpha0 * fabs(epsilon_p - epsilon_p0);
        epsilon_p0 = epsilon_p;
        printf("%f\t%f\n", t, epsilon_p);
    }

    return 0;
}
