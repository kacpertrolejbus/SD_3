#ifndef PERFOTEST_HPP
#define PERFOTEST_HPP
#include <vector>
#include <string>
#include <fstream>
#include "HashTable.hpp"

using namespace std;

class PerfoTest
{
private:
	struct TestR
	{
		double avgIS;
		double avgRS;
		long long insertTime;
		long long removeTime;
		int successI;
		int successR;
		double loadFactor;
	};

	// Predefiniowane rozmiary tablicy
	vector<int> tableSizes = { 101, 503, 1009, 2003, 5003 };
	ofstream outputFile;

	void writeHeader();
	void writeResults(const string& functionName, const string& caseName,
		int tableSize, int testSize, const TestR& results);
	void writeSeparator();

public:
	PerfoTest();
	~PerfoTest();

	vector<int> genRandomData(int count);
	vector<int> genSequentialData(int count);
	vector<int> genClusteredData(int count);

	TestR testHF(HashTable::HashF func, const vector<int>& testData, int tableSize);

	void runCompleteTest(int testSize = 500);
	void testCase(const string& caseName, const vector<int>& data, int tableSize);
	void printTR(const string& functionName, const string& caseName, const TestR& results);
	void compareHF(int testSize = 500);

	
	void runMultiSizeTest(int testSize = 500);
	void compareHFMultiSize(int testSize = 500);

	
	bool openOutputFile(const string& filename = "wyniki_testow.txt");
	void closeOutputFile();
};

#endif