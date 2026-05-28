#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>
#include "Vector.h"
#include "mylib.h"

namespace chr = std::chrono;

auto ms(auto d) {
    return chr::duration<double, std::milli>(d).count();
}

// Skaitymas į bet kokį konteinerį
template<typename Container>
void skaityti(Container& A, const std::string& failas) {
    std::ifstream fin(failas);
    std::string line;
    std::getline(fin, line); // antraštė
    while (std::getline(fin, line)) {
        studentas temp;
        std::istringstream iss(line);
        iss >> temp;
        A.push_back(std::move(temp));
    }
}

// Skaičiavimai
template<typename Container>
void skaiciuoti(Container& A) {
    for (auto& s : A)
        s.setGal(0.4 * s.vid() + 0.6 * s.getEgz());
}

// Rūšiavimas
template<typename Container>
void rusiuoti_bench(Container& A) {
    std::sort(A.begin(), A.end(), pagalGal);
}

// Skirstymas (partition strategija)
template<typename Container>
void skirstyti_bench(Container& studentai, Container& nevykeliai) {
    auto it = std::partition(studentai.begin(), studentai.end(),
        [](const studentas& s){ return s.getGal() >= 5; });
    nevykeliai.assign(std::make_move_iterator(it),
                      std::make_move_iterator(studentai.end()));
    studentai.erase(it, studentai.end());
}

// Vienas benchmark paleidimas
template<typename Container>
void benchmark(const std::string& failas, const std::string& label) {
    Container A, nevykeliai;

    auto t0 = chr::high_resolution_clock::now();
    skaityti(A, failas);
    auto t1 = chr::high_resolution_clock::now();
    skaiciuoti(A);
    auto t2 = chr::high_resolution_clock::now();
    rusiuoti_bench(A);
    auto t3 = chr::high_resolution_clock::now();
    skirstyti_bench(A, nevykeliai);
    auto t4 = chr::high_resolution_clock::now();

    std::cout << std::left
              << std::setw(14) << label
              << std::setw(14) << std::fixed << std::setprecision(2) << ms(t1-t0)
              << std::setw(14) << ms(t2-t1)
              << std::setw(14) << ms(t3-t2)
              << std::setw(14) << ms(t4-t3)
              << std::setw(14) << ms(t4-t0)
              << "\n";
}

int main() {
    // Failai yra vienu lygiu aukščiau nei CMakeLists.txt
    std::vector<std::pair<std::string,std::string>> failai = {
        {"../../100000.txt",    "100 000"},
        {"../../1000000.txt",   "1 000 000"},
        {"../../10000000.txt",  "10 000 000"},
    };

    std::string header = std::string(82, '-');

    for (auto& [failas, label] : failai) {
        std::cout << "\nStudentų: " << label << "\n";
        std::cout << header << "\n";
        std::cout << std::left
                  << std::setw(14) << "Konteineris"
                  << std::setw(14) << "Nuskaitymas"
                  << std::setw(14) << "Skaičiavimai"
                  << std::setw(14) << "Rūšiavimas"
                  << std::setw(14) << "Skirstymas"
                  << std::setw(14) << "Iš viso"
                  << "\n";
        std::cout << header << "\n";

        benchmark<std::vector<studentas>>(failas, "std::vector");
        benchmark<Vector<studentas>>     (failas, "Vector");
    }

    return 0;
}
