#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define NUM_AGENTS 30       // So luong chuon chuon
#define MAX_ITER 100        // So vong lap toi da

// Ham tinh toan makespan (thoi gian hoan thanh cong viec lau nhat tren may)
int calculate_makespan(int *tasks, int num_tasks, int num_machines, int *task_times) {
    int machines[num_machines];
    for (int i = 0; i < num_machines; i++) {
        machines[i] = 0;  // Khoi tao thoi gian cho moi may la 0
    }
    // Phan cong cong viec va cong don thoi gian vao may tuong ung
    for (int i = 0; i < num_tasks; i++) {
        machines[tasks[i]] += task_times[i];
    }

    // Makespan la thoi gian hoan thanh cong viec tren may co thoi gian dai nhat
    int makespan = 0;
    for (int i = 0; i < num_machines; i++) {
        if (machines[i] > makespan) {
            makespan = machines[i];
        }
    }
    return makespan;
}

// Ham khoi tao cac ca the (chuon chuon) ngau nhien
void initialize_agents(int agents[NUM_AGENTS][100], int num_tasks, int num_machines) {
    for (int i = 0; i < NUM_AGENTS; i++) {
        for (int j = 0; j < num_tasks; j++) {
            agents[i][j] = rand() % num_machines;  // Gan cong viec j vao mot may ngau nhien
        }
    }
}

// Ham tim ra giai phap tot nhat
void find_best_solution(int agents[NUM_AGENTS][100], int num_tasks, int num_machines, int *best_solution, int *best_fitness, int *task_times) {
    *best_fitness = INT_MAX; // Khoi tao voi gia tri vo cung lon

    for (int i = 0; i < NUM_AGENTS; i++) {
        int fitness = calculate_makespan(agents[i], num_tasks, num_machines, task_times);
        if (fitness < *best_fitness) {
            *best_fitness = fitness;
            for (int j = 0; j < num_tasks; j++) {
                best_solution[j] = agents[i][j];
            }
        }
    }
}

// Cap nhat vi tri cua cac chuon chuon (thuc hien thuat toan Chuon Chuon Kim)
void update_agents(int agents[NUM_AGENTS][100], int num_tasks, int num_machines, int best_solution[100], int max_iter) {
    // Cap nhat theo cac quy tac cua thuat toan (su dung cac thong so di chuyen ngau nhien)
    for (int iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < NUM_AGENTS; i++) {
            for (int j = 0; j < num_tasks; j++) {
                if (rand() % 2) {
                    agents[i][j] = best_solution[j]; // Di chuyen gan voi giai phap tot nhat
                } else {
                    agents[i][j] = rand() % num_machines; // Di chuyen ngau nhien
                }
            }
        }
    }
}

int main() {
    srand(time(NULL));

    // Nhap thong tin dau vao tu nguoi dung
    int num_tasks, num_machines;

    printf("Nhap so luong cong viec: ");
    scanf("%d", &num_tasks);
    printf("Nhap so luong may: ");
    scanf("%d", &num_machines);

    int task_times[num_tasks];
    printf("Nhap thoi gian xu ly cho moi cong viec:\n");
    for (int i = 0; i < num_tasks; i++) {
        printf("Cong viec %d: ", i + 1);
        scanf("%d", &task_times[i]);
    }

    int agents[NUM_AGENTS][100];  // Mang luu cac ca the chuon chuon
    int best_solution[100];        // Giai phap tot nhat
    int best_fitness;              // Gia tri fitness tot nhat

    // Khoi tao cac ca the ngau nhien
    initialize_agents(agents, num_tasks, num_machines);

    // Tim giai phap tot nhat ban dau
    find_best_solution(agents, num_tasks, num_machines, best_solution, &best_fitness, task_times);

    // Cap nhat cac ca the theo thuat toan Chuon Chuon Kim
    update_agents(agents, num_tasks, num_machines, best_solution, MAX_ITER);

    // In ket qua
    printf("\nGiai phap tot nhat (phan cong cong viec):\n");
    for (int i = 0; i < num_tasks; i++) {
        printf("Cong viec %d -> May %d\n", i + 1, best_solution[i]);
    }
    printf("Best Makespan: %d\n", best_fitness);

    return 0;
}

