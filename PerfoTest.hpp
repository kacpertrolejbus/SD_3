#ifndef PERFOTEST_HPP
#define PERFOTEST_HPP
#include <vector>
#include <string>
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

public:
	PerfoTest();
	~PerfoTest();
	vector<int> genRandomData(int count);
	vector<int> genSequentialData(int count);
	vector<int> genClusteredData(int count);
	TestR testHF(HashTable::HashF func, const vector<int>& testData, int tableSize);
	void runCompleteTest(int tableSize = 1009, int testSize = 500);
	void testCase(const string& caseName, const vector<int>& data, int tableSize);
	void printTR(const string& functionNaame, const string& caseName, const TestR& results);
	void compareHF(int tableSize = 1009, int testSize = 500);
};


#endif 
