#include <stdio.h>
#include <math.h>

#define TOL 1e-6
#define MAX_ITER 100
#define M_PI 3.14159

const double K = 100, n = 10, E = 200000, sigma_yp = 200, Q = 100, b = 100, T = 1, epsilon_bar = 0.01;

double strain(double t) {
    return epsilon_bar * sin(2 * M_PI * t / T);
}

double compute_sigma(double epsilon, double epsilon_p) {
    return E * (epsilon - epsilon_p);
}

double compute_R(double R0, double epsilon_p0, double epsilon_p) {
    return R0 + b * (Q - R0) * fabs(epsilon_p - epsilon_p0);
}

double compute_f(double sigma, double R) {
    if (fabs(sigma) > sigma_yp + R) {
        return pow((fabs(sigma) - sigma_yp - R) / K, 1.0 / n);
    }
    return 0.0;
}

double compute_g(double epsilon_p, double epsilon_p0, double h, double sigma, double R) {
    double f = compute_f(sigma, R);
    return epsilon_p - epsilon_p0 - h * f;
}

double solve_epsilon_p(double epsilon_p0, double epsilon, double epsilon_prev, double R0, double h) {
    double lower = epsilon_p0;
    double upper = epsilon_p0 + epsilon - epsilon_prev;
    double epsilon_p = 0.0;

    for (int iter = 0; iter < MAX_ITER; iter++) {
        epsilon_p = (lower + upper) / 2.0;

        double sigma = compute_sigma(epsilon, epsilon_p);
        double R = compute_R(R0, epsilon_p0, epsilon_p);
        double g = compute_g(epsilon_p, epsilon_p0, h, sigma, R);

        if (fabs(g) < TOL) {
            break;
        }

        double sigma_lower = compute_sigma(epsilon, lower);
        double R_lower = compute_R(R0, epsilon_p0, lower);
        double g_lower = compute_g(lower, epsilon_p0, h, sigma_lower, R_lower);

        if (g_lower * g < 0) {
            upper = epsilon_p;
        } else {
            lower = epsilon_p;
        }
    }

    return epsilon_p;
}

int main() {
    double h = 0.025;
    double t = 0;
    double epsilon_p0 = 0, R0 = 0, epsilon_prev = 0;

    printf("Time Step\tEpsilon_p\n");

    for (t = 0; t <= 10; t += h) {
        double epsilon = strain(t);
        double epsilon_p = solve_epsilon_p(epsilon_p0, epsilon, epsilon_prev, R0, h);

        R0 = compute_R(R0, epsilon_p0, epsilon_p);
        epsilon_prev = epsilon;
        epsilon_p0 = epsilon_p;

        printf("%f\t%f\n", t, epsilon_p);
    }

    return 0;
}
