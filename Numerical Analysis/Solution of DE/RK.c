#include <stdio.h>
#include <math.h>

#define M1 100.0         
#define M2 50.0          
#define K0 1e5           
#define B 200.0          
#define H 0.002          
#define T_FINAL 2.0      

// Function to calculate k1 and k2
double compute_k(double delta_x) {
    return (K0 / 2.0) * (1.0 + exp(-B * fabs(delta_x)));
}

// Function to define the system of ODEs
void system_of_odes(double t, double y[], double dydt[]) {
    double k = compute_k(y[0] - y[2]);  // k1 = k2
    
    dydt[0] = y[1];  // dx1/dt = x1'
    dydt[1] = (-k * y[0] - k * (y[0] - y[2])) / M1;  // dx1'/dt
    dydt[2] = y[3];  // dx2/dt = x2'
    dydt[3] = (-k * (y[2] - y[0])) / M2;  // dx2'/dt
}

int main() {
    double t = 0.0;             // Initial time
    double y[4] = {0.01, 0.0, 0.01, 0.0};  // Initial conditions [x1, x1', x2, x2']
    double dydt[4], k1[4], k2[4], k3[4], k4[4];
    int steps = (int)(T_FINAL / H);  // Number of time steps
    
    printf("Time(s)\tX1(m)\tX1'(m/s)\tX2(m)\tX2'(m/s)\n");
    
    // Loop over time steps
    for (int step = 0; step <= steps; step++) {
        printf("%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n", t, y[0], y[1], y[2], y[3]);
        
        // Compute k1
        system_of_odes(t, y, dydt);
        for (int i = 0; i < 4; i++) {
            k1[i] = H * dydt[i];
        }
        
        // Compute k2
        double temp_y[4];
        for (int i = 0; i < 4; i++) {
            temp_y[i] = y[i] + 0.5 * k1[i];
        }
        system_of_odes(t + 0.5 * H, temp_y, dydt);
        for (int i = 0; i < 4; i++) {
            k2[i] = H * dydt[i];
        }
        
        // Compute k3
        for (int i = 0; i < 4; i++) {
            temp_y[i] = y[i] + 0.5 * k2[i];
        }
        system_of_odes(t + 0.5 * H, temp_y, dydt);
        for (int i = 0; i < 4; i++) {
            k3[i] = H * dydt[i];
        }
        
        // Compute k4
        for (int i = 0; i < 4; i++) {
            temp_y[i] = y[i] + k3[i];
        }
        system_of_odes(t + H, temp_y, dydt);
        for (int i = 0; i < 4; i++) {
            k4[i] = H * dydt[i];
        }
        
        // Update y
        for (int i = 0; i < 4; i++) {
            y[i] += (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) / 6.0;
        }
        
        t += H;  // Increment time
    }
    
    return 0;
}
