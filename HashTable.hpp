#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <vector>
#include "HashFunc.hpp"

using namespace std;

struct HashN
{
	int key;
	int val;
	bool del;
	HashN();
	HashN(int k, int v);
};

class HashTable
{
public:
	enum HashF
	{
		DIVISION = 1,
		MID_SQUARE = 2,
		MULTIPLICATION = 3
	};

private:
	vector<HashN> table;
	int size;
	int count;
	HashF currHF;
	HashFunctions hashFunctions;
	int hash(int key);
	int findSlot(int key, bool forInsertion = false);

public:
	HashTable(int tableSize, HashF func = DIVISION);
	~HashTable();
	pair<bool, int> insert(int key, int val);
	pair<bool, int> remove(int key);
	pair<bool, int> search(int key);

	void clear();
	void printTable();
	void printStats();
	double getLoadFactor();
	int getSize();
	int getCount();

	void setHF(HashF func);
	HashF getCurrHF();
};


#endif
