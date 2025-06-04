#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include <ctime>
#include "PerfoTest.hpp"

using namespace std;
using namespace std::chrono;

PerfoTest::PerfoTest() {}

PerfoTest::~PerfoTest() {
    if (outputFile.is_open()) {
        closeOutputFile();
    }
}

bool PerfoTest::openOutputFile(const string& filename) {
    outputFile.open(filename, ios::out | ios::app);
    if (outputFile.is_open()) {
        
        auto t = time(0);
        auto tm = *localtime(&t);
        outputFile << "\n" << string(80, '=') << endl;
        outputFile << "NOWY TEST - " << put_time(&tm, "%Y-%m-%d %H:%M:%S") << endl;
        outputFile << string(80, '=') << endl;
        return true;
    }
    return false;
}

void PerfoTest::closeOutputFile() {
    if (outputFile.is_open()) {
        outputFile << "\n" << string(80, '=') << endl;
        outputFile << "KONIEC TESTU" << endl;
        outputFile << string(80, '=') << endl;
        outputFile.close();
    }
}

void PerfoTest::writeHeader() {
    string header = "Funkcja haszujaca | Przypadek testowy | Rozmiar tablicy | Rozmiar testu | "
        "Sr. kroki (Insert) | Sr. kroki (Remove) | Czas Insert (μs) | Czas Remove (μs) | "
        "Udane Insert | Udane Remove | Load Factor";

    outputFile << header << endl;
    outputFile << string(header.length(), '-') << endl;
}

void PerfoTest::writeResults(const string& functionName, const string& caseName,
    int tableSize, int testSize, const TestR& results) {
    outputFile << left << setw(18) << functionName << " | "
        << setw(17) << caseName << " | "
        << setw(15) << tableSize << " | "
        << setw(13) << testSize << " | "
        << setw(18) << fixed << setprecision(2) << results.avgIS << " | "
        << setw(18) << results.avgRS << " | "
        << setw(16) << results.insertTime << " | "
        << setw(16) << results.removeTime << " | "
        << setw(12) << results.successI << " | "
        << setw(12) << results.successR << " | "
        << setw(11) << results.loadFactor << endl;
}

void PerfoTest::writeSeparator() {
    outputFile << string(200, '-') << endl;
}

vector<int> PerfoTest::genRandomData(int count) {
    vector<int> data;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100000);

    for (int i = 0; i < count; i++) {
        data.push_back(dis(gen));
    }
    return data;
}

vector<int> PerfoTest::genSequentialData(int count) {
    vector<int> data;
    for (int i = 1; i <= count; i++) {
        data.push_back(i);
    }
    return data;
}

vector<int> PerfoTest::genClusteredData(int count) {
    vector<int> data;
    for (int i = 0; i < count; i++) {
        data.push_back(i * 13);
    }
    return data;
}

PerfoTest::TestR PerfoTest::testHF(HashTable::HashF func, const vector<int>& testData, int tableSize) {
    TestR results = { 0, 0, 0, 0, 0, 0, 0 };
    HashTable ht(tableSize, func);
    double totalInsertSteps = 0;
    int successI = 0;

    auto start = high_resolution_clock::now();
    for (int key : testData) {
        auto result = ht.insert(key, key * 2);
        if (result.first) {
            totalInsertSteps += result.second;
            successI++;
        }
    }
    auto end = high_resolution_clock::now();
    auto insertTime = duration_cast<microseconds>(end - start);

    vector<int> toRemove;
    int removeCount = min(successI / 2, (int)testData.size() / 2);
    for (int i = 0; i < removeCount; i++) {
        toRemove.push_back(testData[i]);
    }

    double totalRemoveSteps = 0;
    int successR = 0;
    start = high_resolution_clock::now();
    for (int key : toRemove) {
        auto result = ht.remove(key);
        if (result.first) {
            totalRemoveSteps += result.second;
            successR++;
        }
    }
    end = high_resolution_clock::now();
    auto removeTime = duration_cast<microseconds>(end - start);

    results.avgIS = successI > 0 ? totalInsertSteps / successI : 0;
    results.avgRS = successR > 0 ? totalRemoveSteps / successR : 0;
    results.insertTime = insertTime.count();
    results.removeTime = removeTime.count();
    results.successI = successI;
    results.successR = successR;
    results.loadFactor = ht.getLoadFactor();

    return results;
}

void PerfoTest::runCompleteTest(int testSize) {
    if (!openOutputFile()) {
        cerr << "Nie można otworzyć pliku do zapisu!" << endl;
        return;
    }

    cout << "=================================" << endl;
    cout << "KOMPLEKSOWY TEST WYDAJNOŚCI" << endl;
    cout << "=================================" << endl;
    cout << "Liczba elementów do testu: " << testSize << endl;
    cout << "Rozmiary tablicy: ";
    for (int size : tableSizes) {
        cout << size << " ";
    }
    cout << endl << "=================================" << endl;

    outputFile << "KOMPLEKSOWY TEST WYDAJNOŚCI" << endl;
    outputFile << "Liczba elementów do testu: " << testSize << endl;
    outputFile << "Rozmiary tablicy: ";
    for (int size : tableSizes) {
        outputFile << size << " ";
    }
    outputFile << endl << endl;

    writeHeader();

    auto randomData = genRandomData(testSize);
    auto sequentialData = genSequentialData(testSize);
    auto clusteredData = genClusteredData(testSize);

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Dzielenie"},
        {HashTable::MID_SQUARE, "Srodek kwadratu"},
        {HashTable::MULTIPLICATION, "Mnozenie"}
    };

    vector<pair<vector<int>*, string>> testCases = {
        {&randomData, "Optymistyczny"},
        {&sequentialData, "Sredni"},
        {&clusteredData, "Pesymistyczny"}
    };

    for (int tableSize : tableSizes) {
        cout << "\n" << string(50, '=') << endl;
        cout << "ROZMIAR TABLICY: " << tableSize << endl;
        cout << string(50, '=') << endl;

        for (auto& func : functions) {
            for (auto& testCase : testCases) {
                cout << "\nTestowanie: " << func.second << " - " << testCase.second << endl;

                TestR results = testHF(func.first, *testCase.first, tableSize);
                printTR(func.second, testCase.second, results);
                writeResults(func.second, testCase.second, tableSize, testSize, results);
            }
        }
        writeSeparator();
    }

    closeOutputFile();
    cout << "\nWyniki zapisano do pliku 'wyniki_testow.txt'" << endl;
}

void PerfoTest::runMultiSizeTest(int testSize) {
    if (!openOutputFile("test_wielorozmiarowy.txt")) {
        cerr << "Nie można otworzyć pliku do zapisu!" << endl;
        return;
    }

    cout << "\n" << string(60, '=') << endl;
    cout << "TEST WIELOROZMIAROWY" << endl;
    cout << string(60, '=') << endl;

    outputFile << "TEST WIELOROZMIAROWY" << endl;
    outputFile << "Liczba elementów: " << testSize << endl << endl;
    writeHeader();

    auto randomData = genRandomData(testSize);

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Dzielenie"},
        {HashTable::MID_SQUARE, "Srodek kwadratu"},
        {HashTable::MULTIPLICATION, "Mnozenie"}
    };

    cout << left << setw(15) << "Rozmiar"
        << setw(18) << "Funkcja"
        << setw(15) << "Sr. kroki (I)"
        << setw(15) << "Sr. kroki (U)"
        << setw(12) << "Czas (I) μs"
        << setw(12) << "Load Factor" << endl;
    cout << string(87, '-') << endl;

    for (int tableSize : tableSizes) {
        for (auto& func : functions) {
            TestR results = testHF(func.first, randomData, tableSize);

            cout << left << setw(15) << tableSize
                << setw(18) << func.second
                << setw(15) << fixed << setprecision(2) << results.avgIS
                << setw(15) << results.avgRS
                << setw(12) << results.insertTime
                << setw(12) << results.loadFactor << endl;

            writeResults(func.second, "Losowe dane", tableSize, testSize, results);
        }
        cout << string(87, '-') << endl;
    }

    closeOutputFile();
    cout << "\nWyniki zapisano do pliku 'test_wielorozmiarowy.txt'" << endl;
}

void PerfoTest::testCase(const string& caseName, const vector<int>& data, int tableSize) {
    cout << "\n" << string(40, '=') << endl;
    cout << "TEST PRZYPADKU: " << caseName << endl;
    cout << string(40, '=') << endl;

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Metoda dzielenia"},
        {HashTable::MID_SQUARE, "Metoda środka kwadratu"},
        {HashTable::MULTIPLICATION, "Metoda mnożenia"}
    };

    for (auto& func : functions) {
        cout << "\n--- " << func.second << " ---" << endl;
        TestR results = testHF(func.first, data, tableSize);
        printTR(func.second, caseName, results);
    }
}

void PerfoTest::printTR(const string& functionName, const string& caseName, const TestR& results) {
    cout << fixed << setprecision(2);

    cout << "DODAWANIE:" << endl;
    cout << "  Udane operacje: " << results.successI << endl;
    cout << "  Średnia liczba kroków: " << results.avgIS << endl;
    cout << "  Czas wykonania: " << results.insertTime << " μs" << endl;

    cout << "USUWANIE:" << endl;
    cout << "  Udane operacje: " << results.successR << endl;
    cout << "  Średnia liczba kroków: " << results.avgRS << endl;
    cout << "  Czas wykonania: " << results.removeTime << " μs" << endl;

    cout << "Load Factor: " << results.loadFactor << endl;
}

void PerfoTest::compareHF(int testSize) {
    cout << "\n" << string(60, '=') << endl;
    cout << "PORÓWNANIE FUNKCJI HASZUJĄCYCH" << endl;
    cout << string(60, '=') << endl;

    auto randomData = genRandomData(testSize);

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Dzielenie"},
        {HashTable::MID_SQUARE, "Środek kwadratu"},
        {HashTable::MULTIPLICATION, "Mnożenie"}
    };

  
    int tableSize = tableSizes[0];

    cout << "Rozmiar tablicy: " << tableSize << endl;
    cout << left << setw(20) << "Funkcja"
        << setw(15) << "Śr. kroki (I)"
        << setw(15) << "Śr. kroki (U)"
        << setw(12) << "Czas (I) μs"
        << setw(12) << "Czas (U) μs" << endl;
    cout << string(74, '-') << endl;

    for (auto& func : functions) {
        TestR results = testHF(func.first, randomData, tableSize);

        cout << left << setw(20) << func.second
            << setw(15) << fixed << setprecision(2) << results.avgIS
            << setw(15) << results.avgRS
            << setw(12) << results.insertTime
            << setw(12) << results.removeTime << endl;
    }
}

void PerfoTest::compareHFMultiSize(int testSize) {
    if (!openOutputFile("porownanie_funkcji.txt")) {
        cerr << "Nie można otworzyć pliku do zapisu!" << endl;
        return;
    }

    cout << "\n" << string(60, '=') << endl;
    cout << "PORÓWNANIE FUNKCJI NA RÓŻNYCH ROZMIARACH" << endl;
    cout << string(60, '=') << endl;

    outputFile << "PORÓWNANIE FUNKCJI HASZUJĄCYCH NA RÓŻNYCH ROZMIARACH" << endl;
    outputFile << "Liczba elementów: " << testSize << endl << endl;

    auto randomData = genRandomData(testSize);

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Dzielenie"},
        {HashTable::MID_SQUARE, "Srodek kwadratu"},
        {HashTable::MULTIPLICATION, "Mnozenie"}
    };

    for (int tableSize : tableSizes) {
        cout << "\n--- Rozmiar tablicy: " << tableSize << " ---" << endl;
        outputFile << "\nRozmiar tablicy: " << tableSize << endl;
        outputFile << "Funkcja | Sr. kroki Insert | Sr. kroki Remove | Czas Insert | Load Factor" << endl;
        outputFile << string(80, '-') << endl;

        cout << left << setw(20) << "Funkcja"
            << setw(15) << "Śr. kroki (I)"
            << setw(15) << "Śr. kroki (U)"
            << setw(12) << "Load Factor" << endl;
        cout << string(62, '-') << endl;

        for (auto& func : functions) {
            TestR results = testHF(func.first, randomData, tableSize);

            cout << left << setw(20) << func.second
                << setw(15) << fixed << setprecision(2) << results.avgIS
                << setw(15) << results.avgRS
                << setw(12) << results.loadFactor << endl;

            outputFile << left << setw(7) << func.second << " | "
                << setw(16) << results.avgIS << " | "
                << setw(16) << results.avgRS << " | "
                << setw(11) << results.insertTime << " | "
                << setw(11) << results.loadFactor << endl;
        }
    }

    closeOutputFile();
    cout << "\nWyniki zapisano do pliku 'porownanie_funkcji.txt'" << endl;
}