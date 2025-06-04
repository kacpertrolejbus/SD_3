#ifndef HASHFUNC_HPP
#define HASHFUNC_HPP

#include <cmath>
#include <string>

class HashFunctions
{
private:
	int nearestPrime(int tableSize);
	int counter(int keyN);
public:
	HashFunctions();
	~HashFunctions();
	int divisionMethod(int key, int tableSize);
	int multiplicationMethod(int key, int tableSize);
	int midSquareMethod(int key, int tableSize);
};

#endif
