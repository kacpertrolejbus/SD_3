#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>
#include "PerfoTest.hpp"

using namespace std;
using namespace std::chrono;

PerfoTest::PerfoTest(){}
PerfoTest::~PerfoTest() {}

vector<int> PerfoTest::genRandomData(int count)
{
	vector<int> data;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, 100000);

	for (int i = 0; i < count; i++)
	{
		data.push_back(dis(gen));
	}
	return data;

}
vector<int> PerfoTest::genSequentialData(int count) {
	std::vector<int> data;

	for (int i = 1; i <= count; i++) {
		data.push_back(i);
	}

	return data;
}

vector<int> PerfoTest::genClusteredData(int count)
{
	vector<int> data;
	for (int i = 0; i < count; i++)
	{
		data.push_back(i * 13);
	}
	return data;
}

PerfoTest::TestR PerfoTest::testHF(HashTable::HashF func, const vector<int>& testData, int tableSize)
{
	TestR results = { 0, 0, 0, 0, 0, 0, 0 };
	HashTable ht(tableSize, func);
	double totalInsertSteps = 0;
	int successI = 0;
	auto start = high_resolution_clock::now();
	for (int key : testData)
	{
		auto result = ht.insert(key, key * 2);
		if (result.first)
		{
			totalInsertSteps += result.second;
			successI++;
		}
	}
	auto end = high_resolution_clock::now();
	auto insertTime = duration_cast<microseconds>(end - start);
	vector<int> toRemove;
	int removeCount = min(successI / 2, (int)testData.size() / 2);
	for (int i = 0; i < removeCount; i++)
	{
		toRemove.push_back(testData[i]);
	}

	double totalRemoveSteps = 0;
	int successR = 0;
	start = high_resolution_clock::now();
	for (int key : toRemove)
	{
		auto result = ht.remove(key);
		if (result.first)
		{
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

void PerfoTest::runCompleteTest(int tableSize, int testSize) {
    std::cout << "=================================" << std::endl;
    std::cout << "KOMPLEKSOWY TEST WYDAJNOŚCI" << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << "Rozmiar tablicy: " << tableSize << std::endl;
    std::cout << "Liczba elementów do testu: " << testSize << std::endl;
    std::cout << "=================================" << std::endl;

    auto randomData = genRandomData(testSize);
    auto sequentialData = genSequentialData(testSize);
    auto clusteredData = genClusteredData(testSize);

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "METODA DZIELENIA"},
        {HashTable::MID_SQUARE, "METODA ŚRODKA KWADRATU"},
        {HashTable::MULTIPLICATION, "METODA MNOŻENIA"}
    };

    vector<pair<std::vector<int>*, string>> testCases = {
        {&randomData, "Optymistyczny (losowe dane)"},
        {&sequentialData, "Średni (dane sekwencyjne)"},
        {&clusteredData, "Pesymistyczny (dane klastrowane)"}
    };

    for (auto& func : functions) {
        std::cout << "\n" << string(50, '=') << std::endl;
        std::cout << "FUNKCJA: " << func.second << std::endl;
        std::cout << string(50, '=') << std::endl;

        for (auto& testCase : testCases) {
            std::cout << "\n--- " << testCase.second << " ---" << std::endl;

            TestR results = testHF(func.first, *testCase.first, tableSize);
            printTR(func.second, testCase.second, results);
        }
    }
}

void PerfoTest::testCase(const string& caseName,
    const vector<int>& data,
    int tableSize) {

    std::cout << "\n" << string(40, '=') << std::endl;
    std::cout << "TEST PRZYPADKU: " << caseName << std::endl;
    std::cout << string(40, '=') << std::endl;

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Metoda dzielenia"},
        {HashTable::MID_SQUARE, "Metoda środka kwadratu"},
        {HashTable::MULTIPLICATION, "Metoda mnożenia"}
    };

    for (auto& func : functions) {
        std::cout << "\n--- " << func.second << " ---" << std::endl;
        TestR results = testHF(func.first, data, tableSize);
        printTR(func.second, caseName, results);
    }
}

void PerfoTest::printTR(const string& functionName,const string& caseName,const TestR& results) {

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

void PerfoTest::compareHF(int tableSize, int testSize) {
    cout << "\n" << string(60, '=') << endl;
    cout << "PORÓWNANIE FUNKCJI HASZUJĄCYCH" << endl;
    cout << string(60, '=') << endl;

    auto randomData = genRandomData(testSize);

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Dzielenie"},
        {HashTable::MID_SQUARE, "Środek kwadratu"},
        {HashTable::MULTIPLICATION, "Mnożenie"}
    };

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