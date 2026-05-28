#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Vector.h"

using ms = std::chrono::duration<double, std::milli>;

template <typename Container>
double benchmark_push_back(unsigned int sz) {
    auto start = std::chrono::high_resolution_clock::now();

    Container v;
    for (unsigned int i = 1; i <= sz; ++i)
        v.push_back(i);

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ms>(end - start).count();
}

// Perskirstymų skaičiavimas – commit #10
template <typename Container>
int count_reallocations(unsigned int sz) {
    Container v;
    int reallocations = 0;
    size_t lastCap = v.capacity();

    for (unsigned int i = 1; i <= sz; ++i) {
        v.push_back(i);
        if (v.capacity() != lastCap) {
            ++reallocations;
            lastCap = v.capacity();
        }
    }
    return reallocations;
}

int main() {
    const int RUNS = 5; // vidurkis iš kelių paleidimų

    std::vector<unsigned int> sizes = {
        10000, 100000, 1000000, 10000000, 100000000
    };

    // --- Spartos lentelė ---
    std::cout << std::string(62, '-') << "\n";
    std::cout << std::left
              << std::setw(14) << "Elementų sk."
              << std::setw(16) << "std::vector(ms)"
              << std::setw(16) << "Vector(ms)"
              << std::setw(10) << "Santykis"
              << "\n";
    std::cout << std::string(62, '-') << "\n";

    for (unsigned int sz : sizes) {
        double stdTime = 0.0, myTime = 0.0;

        for (int r = 0; r < RUNS; ++r) {
            stdTime += benchmark_push_back<std::vector<int>>(sz);
            myTime  += benchmark_push_back<Vector<int>>(sz);
        }
        stdTime /= RUNS;
        myTime  /= RUNS;

        std::cout << std::left
                  << std::setw(14) << sz
                  << std::setw(16) << std::fixed << std::setprecision(3) << stdTime
                  << std::setw(16) << myTime
                  << std::setw(10) << std::setprecision(2) << (myTime / stdTime)
                  << "\n";
    }

    // --- Perskirstymų lentelė ---
    std::cout << "\n" << std::string(50, '-') << "\n";
    std::cout << std::left
              << std::setw(14) << "Elementų sk."
              << std::setw(18) << "std::vector realoc"
              << std::setw(18) << "Vector realoc"
              << "\n";
    std::cout << std::string(50, '-') << "\n";

    std::vector<unsigned int> realocSizes = { 100000000 };
    for (unsigned int sz : realocSizes) {
        int stdR = count_reallocations<std::vector<int>>(sz);
        int myR  = count_reallocations<Vector<int>>(sz);
        std::cout << std::left
                  << std::setw(14) << sz
                  << std::setw(18) << stdR
                  << std::setw(18) << myR
                  << "\n";
    }

    return 0;
}
