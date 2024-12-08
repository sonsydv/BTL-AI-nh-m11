#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_ANTENNAS 100 // So luong anten toi da
#define DIM 2            // Khong gian tim kiem la 2D (x, y)

// Cau truc luu thong tin mot anten
typedef struct {
    double position[DIM]; // Vi tri (x, y)
    double fitness;       // Gia tri ham muc tieu
} Antenna;

// Ham tinh khoang cach giua hai diem
double distance(double *a, double *b) {
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2));
}

// Ham muc tieu: toi uu hoa vung phu song va giam nhieu
double calculate_fitness(Antenna *antennas, int num_antennas, double radius) {
    double coverage_area = 0.0; // Tong vung phu song
    double overlap_area = 0.0;  // Tong vung giao thoa (nhieu)

    for (int i = 0; i < num_antennas; i++) {
        // Gia su vung phu song cua moi anten la hinh tron voi ban kinh `radius`
        coverage_area += M_PI * radius * radius;

        // Tinh giao thoa voi cac anten khac
        for (int j = i + 1; j < num_antennas; j++) {
            double dist = distance(antennas[i].position, antennas[j].position);
            if (dist < 2 * radius) {
                overlap_area += M_PI * pow(radius - dist / 2, 2); // Vung giao thoa
            }
        }
    }

    // Ham muc tieu: toi uu hoa vung phu song - giao thoa
    return coverage_area - overlap_area;
}

// Ham sinh so ngau nhien trong khoang [min, max]
double random_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

int main() {
    srand(time(0)); // Khoi tao bo sinh so ngau nhien

    // Tham so cua bai toan
    int num_antennas;       // So luong anten
    int max_iterations;     // So lan lap toi da
    double radius = 10.0;   // Ban kinh phu song cua moi anten
    double x_min = 0.0;     // Gioi han duoi cua x
    double x_max = 100.0;   // Gioi han tren cua x
    double y_min = 0.0;     // Gioi han duoi cua y
    double y_max = 100.0;   // Gioi han tren cua y

    // Nhap so luong anten va so lan lap
    printf("Nhap so luong anten: ");
    scanf("%d", &num_antennas);
    printf("Nhap so lan lap toi da: ");
    scanf("%d", &max_iterations);

    // Quan the ban dau
    Antenna population[MAX_ANTENNAS];
    for (int i = 0; i < num_antennas; i++) {
        // Sinh ngau nhien vi tri cho cac anten
        population[i].position[0] = random_double(x_min, x_max);
        population[i].position[1] = random_double(y_min, y_max);
        population[i].fitness = 0.0;
    }

    // Bat dau toi uu hoa bang IWO
    for (int iter = 0; iter < max_iterations; iter++) {
        // Tinh ham muc tieu cho tung anten
        for (int i = 0; i < num_antennas; i++) {
            population[i].fitness = calculate_fitness(population, num_antennas, radius);
        }

        // Tao quan the moi
        Antenna new_population[MAX_ANTENNAS];
        int count = 0;
        for (int i = 0; i < num_antennas; i++) {
            int num_seeds = random_double(1, 5); // Moi anten tao ra 1-5 hat giong
            for (int j = 0; j < num_seeds; j++) {
                if (count >= MAX_ANTENNAS) break;

                // Sinh ca the moi bang cach dot bien
                for (int d = 0; d < DIM; d++) {
                    double sigma = (x_max - x_min) / 10; // Do dot bien
                    double perturbation = random_double(-sigma, sigma);
                    new_population[count].position[d] = population[i].position[d] + perturbation;

                    // Dam bao vi tri nam trong gioi han
                    if (new_population[count].position[d] < x_min) new_population[count].position[d] = x_min;
                    if (new_population[count].position[d] > x_max) new_population[count].position[d] = x_max;
                }
                count++;
            }
        }

        // Cap nhat quan the
        for (int i = 0; i < count; i++) {
            new_population[i].fitness = calculate_fitness(new_population, num_antennas, radius);
        }

        // Chon cac ca the tot nhat
        for (int i = 0; i < num_antennas; i++) {
            population[i] = new_population[i];
        }
    }

    // Tim vi tri anten tot nhat
    int best_index = 0;
    for (int i = 1; i < num_antennas; i++) {
        if (population[i].fitness > population[best_index].fitness) {
            best_index = i;
        }
    }

    // In ket qua
    printf("\nVi tri anten tot nhat:\n");
    for (int i = 0; i < num_antennas; i++) {
        printf("Anten %d: (x = %.2f, y = %.2f)\n", i + 1, population[i].position[0], population[i].position[1]);
    }
    printf("\nGia tri ham muc tieu cao nhat: %.2f\n", population[best_index].fitness);

    return 0;
}

