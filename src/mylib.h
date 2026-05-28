#pragma once
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <fstream>
#include <algorithm>
#include <iomanip>

#if defined(USE_LIST)
    template<typename T> using Studentai = std::list<T>;
#elif defined(USE_DEQUE)
    template<typename T> using Studentai = std::deque<T>;
#elif defined(USE_VECTOR)
    template<typename T> using Studentai = std::vector<T>;
#else
    #include "Vector.h"
    template<typename T> using Studentai = Vector<T>;
#endif

class Zmogus {
protected:
    std::string vardas;
    std::string pavarde;

public:
    Zmogus() : vardas(""), pavarde("") {}
    Zmogus(const std::string& v, const std::string& p) : vardas(v), pavarde(p) {}

    virtual ~Zmogus() {}

    virtual std::string getVardas()  const = 0;
    virtual std::string getPavarde() const = 0;
    virtual void setVardas(const std::string& v) = 0;
    virtual void setPavarde(const std::string& p) = 0;

    virtual void print(std::ostream& out) const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Zmogus& z) {
        z.print(out);
        return out;
    }
};

class studentas : public Zmogus {
private:
    std::vector<int> paz;
    int egz;
    double rez;
    double gal;

public:
    studentas()
        : Zmogus(), egz(0), rez(0.0), gal(0.0) {}

    studentas(const std::string& v, const std::string& p,
              const std::vector<int>& pazymiai, int e)
        : Zmogus(v, p), paz(pazymiai), egz(e), rez(0.0), gal(0.0) {}

    ~studentas() override {}

    studentas(const studentas& other)
        : Zmogus(other.vardas, other.pavarde),
          paz(other.paz), egz(other.egz), rez(other.rez), gal(other.gal) {}

    studentas& operator=(const studentas& other) {
        if (this == &other) return *this;
        vardas  = other.vardas;
        pavarde = other.pavarde;
        paz     = other.paz;
        egz     = other.egz;
        rez     = other.rez;
        gal     = other.gal;
        return *this;
    }

    studentas(studentas&& other) noexcept
        : Zmogus(std::move(other.vardas), std::move(other.pavarde)),
          paz(std::move(other.paz)), egz(other.egz), rez(other.rez), gal(other.gal) {
        other.vardas = ""; other.pavarde = "";
        other.egz = 0; other.rez = 0.0; other.gal = 0.0;
    }

    studentas& operator=(studentas&& other) noexcept {
        if (this == &other) return *this;
        vardas  = std::move(other.vardas);
        pavarde = std::move(other.pavarde);
        paz     = std::move(other.paz);
        egz     = other.egz; rez = other.rez; gal = other.gal;
        other.vardas = ""; other.pavarde = "";
        other.egz = 0; other.rez = 0.0; other.gal = 0.0;
        return *this;
    }

    std::string getVardas()  const override { return vardas; }
    std::string getPavarde() const override { return pavarde; }
    void setVardas(const std::string& v) override { vardas = v; }
    void setPavarde(const std::string& p) override { pavarde = p; }

    void print(std::ostream& out) const override {
        out << std::left << std::setw(15) << vardas
            << '\t' << std::setw(15) << pavarde
            << '\t' << std::fixed << std::setprecision(2) << gal;
    }

    std::vector<int> getPaz() const { return paz; }
    int    getEgz() const { return egz; }
    double getRez() const { return rez; }
    double getGal() const { return gal; }

    void setPaz(const std::vector<int>& p) { paz = p; }
    void addPaz(int p)                     { paz.push_back(p); }
    void clearPaz()                        { paz.clear(); }
    void setEgz(int e)                     { egz = e; }
    void setRez(double r)                  { rez = r; }
    void setGal(double g)                  { gal = g; }

    friend std::ostream& operator<<(std::ostream& out, const studentas& s) {
        s.print(out);
        return out;
    }

    friend std::istream& operator>>(std::istream& in, studentas& s) {
        s.paz.clear();
        in >> s.vardas >> s.pavarde;
        int x;
        while (in >> x) s.paz.push_back(x);
        s.egz = s.paz.back();
        s.paz.pop_back();
        return in;
    }

    double vid() const {
        if (paz.empty()) return 0.0;
        double suma = 0;
        for (int p : paz) suma += p;
        return suma / paz.size();
    }

    double med() const {
        if (paz.empty()) return 0.0;
        std::vector<int> sorted = paz;
        std::sort(sorted.begin(), sorted.end());
        int n = sorted.size();
        if (n % 2 == 0) return (sorted[n/2-1] + sorted[n/2]) / 2.0;
        else             return sorted[n/2];
    }
};

template<typename Container, typename Comp>
void rusiuoti(Container& c, Comp comp) {
#if defined(USE_LIST)
    c.sort(comp);
#else
    std::sort(c.begin(), c.end(), comp);
#endif
}

std::string randomstr();
bool pagalVard(const studentas& a, const studentas& b);
bool pagalPavard(const studentas& a, const studentas& b);
bool pagalGal(const studentas& a, const studentas& b);
int getInt(int min, int max);
std::string getFile();
void printRez(std::ostream& out, Studentai<studentas>& A, int skaiciavimas);
void ivestiRanka(Studentai<studentas>& A, int& m);
void generuotiPazymius(Studentai<studentas>& A, int& m);
void generuotiViska(Studentai<studentas>& A, int& m);
void skaitytiIsFailo(Studentai<studentas>& A, int& m, std::string& failas);
void generuotiFaila();
void skirstymas(Studentai<studentas>& studentai, Studentai<studentas>& nevykeliai);
