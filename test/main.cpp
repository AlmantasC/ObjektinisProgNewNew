#include <gtest/gtest.h>
#include "mylib.h"
#include <sstream>

// ============================================================
//  Pagalbinė funkcija: sukuria studentą su pažymiais
// ============================================================
studentas sukurtiStudenta(const std::string& v, const std::string& p,
                          std::vector<int> pazymiai, int egz) {
    studentas s;
    s.setVardas(v);
    s.setPavarde(p);
    for (int x : pazymiai) s.addPaz(x);
    s.setEgz(egz);
    return s;
}

// ============================================================
//  1. Numatytasis konstruktorius
// ============================================================
TEST(StudentasKonstruktorius, Numatytasis) {
    studentas s;
    EXPECT_EQ(s.getVardas(),  "");
    EXPECT_EQ(s.getPavarde(), "");
    EXPECT_EQ(s.getEgz(),     0);
    EXPECT_DOUBLE_EQ(s.getGal(), 0.0);
    EXPECT_TRUE(s.getPaz().empty());
}

// ============================================================
//  2. Kopijavimo konstruktorius
// ============================================================
TEST(StudentasKonstruktorius, Kopijavimo) {
    studentas a = sukurtiStudenta("Jonas", "Jonaitis", {6, 8, 10}, 7);
    studentas b(a);

    EXPECT_EQ(b.getVardas(),  a.getVardas());
    EXPECT_EQ(b.getPavarde(), a.getPavarde());
    EXPECT_EQ(b.getEgz(),     a.getEgz());
    EXPECT_EQ(b.getPaz(),     a.getPaz());

    // Nepriklausomumas — b keitimas neturi įtakos a
    b.setVardas("Petras");
    EXPECT_EQ(a.getVardas(), "Jonas");
}

// ============================================================
//  3. Kopijavimo priskyrimo operatorius
// ============================================================
TEST(StudentasKonstruktorius, KopijavimoPreskyrimasOperatorius) {
    studentas a = sukurtiStudenta("Jonas", "Jonaitis", {6, 8}, 9);
    studentas b;
    b = a;

    EXPECT_EQ(b.getVardas(),  "Jonas");
    EXPECT_EQ(b.getPavarde(), "Jonaitis");
    EXPECT_EQ(b.getEgz(),     9);
    EXPECT_EQ(b.getPaz(),     a.getPaz());

    // Savipriskyrimas neturi sugadinti
    b = b;
    EXPECT_EQ(b.getVardas(), "Jonas");
}

// ============================================================
//  4. Perkėlimo konstruktorius
// ============================================================
TEST(StudentasKonstruktorius, Perkelimo) {
    studentas a = sukurtiStudenta("Jonas", "Jonaitis", {5, 7, 9}, 8);
    studentas b(std::move(a));

    EXPECT_EQ(b.getVardas(),  "Jonas");
    EXPECT_EQ(b.getPavarde(), "Jonaitis");
    EXPECT_EQ(b.getEgz(),     8);
    EXPECT_EQ(b.getPaz().size(), 3u);

    // a turi būti tuščias po perkėlimo
    EXPECT_EQ(a.getVardas(),  "");
    EXPECT_EQ(a.getPavarde(), "");
    EXPECT_EQ(a.getEgz(),     0);
}

// ============================================================
//  5. Perkėlimo priskyrimo operatorius
// ============================================================
TEST(StudentasKonstruktorius, PerkelimoPreskyrimasOperatorius) {
    studentas a = sukurtiStudenta("Jonas", "Jonaitis", {4, 6, 8}, 7);
    studentas b;
    b = std::move(a);

    EXPECT_EQ(b.getVardas(),  "Jonas");
    EXPECT_EQ(b.getPavarde(), "Jonaitis");
    EXPECT_EQ(b.getEgz(),     7);
    EXPECT_EQ(b.getPaz().size(), 3u);

    EXPECT_EQ(a.getVardas(),  "");
    EXPECT_EQ(a.getPavarde(), "");
    EXPECT_EQ(a.getEgz(),     0);
}

// ============================================================
//  Vidurkis
// ============================================================
TEST(Skaiciavimai, VidurklisVienas) {
    studentas s;
    s.addPaz(8);
    EXPECT_DOUBLE_EQ(s.vid(), 8.0);
}

TEST(Skaiciavimai, VidurklisKeli) {
    studentas s;
    s.addPaz(6); s.addPaz(8); s.addPaz(10);
    EXPECT_DOUBLE_EQ(s.vid(), 8.0);
}

TEST(Skaiciavimai, VidurklisOhnePazymiu) {
    studentas s;
    EXPECT_DOUBLE_EQ(s.vid(), 0.0);
}

// ============================================================
//  Mediana
// ============================================================
TEST(Skaiciavimai, MedianaNelyginisSkacius) {
    studentas s;
    s.addPaz(4); s.addPaz(10); s.addPaz(6);
    EXPECT_DOUBLE_EQ(s.med(), 6.0);
}

TEST(Skaiciavimai, MedianaLyginisSkacius) {
    studentas s;
    s.addPaz(4); s.addPaz(6);
    EXPECT_DOUBLE_EQ(s.med(), 5.0);
}

TEST(Skaiciavimai, MedianaOhnePazymiu) {
    studentas s;
    EXPECT_DOUBLE_EQ(s.med(), 0.0);
}

// ============================================================
//  Galutinis balas
// ============================================================
TEST(Skaiciavimai, GalutinisVidurkiu) {
    studentas s;
    s.addPaz(10); s.addPaz(10);
    s.setEgz(10);
    s.setGal(0.4 * s.vid() + 0.6 * s.getEgz());
    EXPECT_DOUBLE_EQ(s.getGal(), 10.0);
}

TEST(Skaiciavimai, GalutinisMediana) {
    studentas s;
    s.addPaz(6); s.addPaz(8); s.addPaz(10);
    s.setEgz(6);
    s.setGal(0.4 * s.med() + 0.6 * s.getEgz());
    EXPECT_DOUBLE_EQ(s.getGal(), 0.4*8.0 + 0.6*6.0);
}

// ============================================================
//  Skirstymas
// ============================================================
TEST(Skirstymas, NevykeliaiAtskiriami) {
    Studentai<studentas> visi, nevykeliai;
    for (int g : {3, 7, 2, 8, 4, 9}) {
        studentas s;
        s.setGal(g);
        visi.push_back(s);
    }
    skirstymas(visi, nevykeliai);

    for (const auto& s : visi)
        EXPECT_GE(s.getGal(), 5.0);
    for (const auto& s : nevykeliai)
        EXPECT_LT(s.getGal(), 5.0);
}

TEST(Skirstymas, VisiVykeliai) {
    Studentai<studentas> visi, nevykeliai;
    for (int g : {5, 7, 9}) {
        studentas s; s.setGal(g); visi.push_back(s);
    }
    skirstymas(visi, nevykeliai);
    EXPECT_TRUE(nevykeliai.empty());
    EXPECT_EQ(visi.size(), 3u);
}

TEST(Skirstymas, VisiNevykeliai) {
    Studentai<studentas> visi, nevykeliai;
    for (int g : {1, 2, 3, 4}) {
        studentas s; s.setGal(g); visi.push_back(s);
    }
    skirstymas(visi, nevykeliai);
    EXPECT_TRUE(visi.empty());
    EXPECT_EQ(nevykeliai.size(), 4u);
}

TEST(Skirstymas, TustiKonteineriaiNesugriuna) {
    Studentai<studentas> visi, nevykeliai;
    EXPECT_NO_THROW(skirstymas(visi, nevykeliai));
    EXPECT_TRUE(visi.empty());
    EXPECT_TRUE(nevykeliai.empty());
}

// ============================================================
//  Išvedimas
// ============================================================
TEST(Isvedimas, PrintRezIsvedasTeisingai) {
    Studentai<studentas> A;
    studentas s = sukurtiStudenta("Jonas", "Jonaitis", {8}, 9);
    s.setGal(0.4*8.0 + 0.6*9.0);
    A.push_back(s);

    std::ostringstream oss;
    printRez(oss, A, 1);
    std::string out = oss.str();

    EXPECT_NE(out.find("Jonas"),    std::string::npos);
    EXPECT_NE(out.find("Jonaitis"), std::string::npos);
}
