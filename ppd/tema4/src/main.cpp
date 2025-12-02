#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iomanip>

void solve_sequential();
void solve_parallel(int p_r, int p_w);

int main() {
    const int NUM_RUNS = 10;
    const int NUM_CONFIGS = 6;

    // Configurations: {p_r, p_w}
    int configs[NUM_CONFIGS][2] = {
        {1, 3},   // p=4, p_r=1
        {1, 7},   // p=8, p_r=1
        {1, 15},  // p=16, p_r=1
        {2, 2},   // p=4, p_r=2
        {2, 6},   // p=8, p_r=2
        {2, 14}   // p=16, p_r=2
    };

    // Store times
    double seq_times[NUM_RUNS];
    double par_times[NUM_CONFIGS][NUM_RUNS];

    for (int run = 0; run < NUM_RUNS; run++) {
        // Generate new data
        system("python3 src/scripts/generate_files.py");

        // Run sequential
        auto start = std::chrono::high_resolution_clock::now();
        solve_sequential();
        auto end = std::chrono::high_resolution_clock::now();
        seq_times[run] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;

        // Run all parallel configurations
        for (int c = 0; c < NUM_CONFIGS; c++) {
            int p_r = configs[c][0];
            int p_w = configs[c][1];

            start = std::chrono::high_resolution_clock::now();
            solve_parallel(p_r, p_w);
            end = std::chrono::high_resolution_clock::now();
            par_times[c][run] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        }
    }

    // Calculate averages
    double seq_avg = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        seq_avg += seq_times[i];
    }
    seq_avg /= NUM_RUNS;

    double par_avg[NUM_CONFIGS] = {0};
    for (int c = 0; c < NUM_CONFIGS; c++) {
        for (int i = 0; i < NUM_RUNS; i++) {
            par_avg[c] += par_times[c][i];
        }
        par_avg[c] /= NUM_RUNS;
    }

    // Print results table
    std::cout << "\n\n========== RESULTS ==========" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nSequential average: " << seq_avg << " ms\n" << std::endl;

    std::cout << std::setw(10) << "Config"
              << std::setw(15) << "Avg Time (ms)"
              << std::setw(15) << "Speedup" << std::endl;
    std::cout << std::string(40, '-') << std::endl;

    for (int c = 0; c < NUM_CONFIGS; c++) {
        int p_r = configs[c][0];
        int p_w = configs[c][1];
        int p = p_r + p_w;

        std::string config_str = "p=" + std::to_string(p) + ",r=" + std::to_string(p_r);
        double speedup = seq_avg / par_avg[c];

        std::cout << std::setw(10) << config_str
                  << std::setw(15) << par_avg[c]
                  << std::setw(15) << speedup << "x" << std::endl;
    }

    return 0;
}
