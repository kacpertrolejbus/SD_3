#include <iostream>
#include <math.h>
#include "HashFunc.hpp"

using namespace std;

HashFunctions::HashFunctions() {}

HashFunctions::~HashFunctions() {}

int HashFunctions::nearestPrime(int tableSize)
{
	int j = 0;
	for (int i = tableSize; i >= 2; i--)
	{
		for (j = 2; j <= sqrt(i); j++)
		{
			if (i % j == 0)
			{
				break;
			}
		}

		if (j > sqrt(i))
		{
			return i;
		}
	}
}

 int HashFunctions::counter(int keyN)
{
	int count = 0;
	while (keyN != 0)
	{
		count++;
		keyN /= 10;
	}
	return count;
}

 int HashFunctions::divisionMethod(int key, int tableSize)
{
	return key % HashFunctions::nearestPrime(tableSize);
}

 int HashFunctions::midSquareMethod(int key, int tableSize)
{
	long long keyN = (long long)key;
	int tableDS = HashFunctions::counter(tableSize - 1);
	int counterKey = counter(keyN);
	int e = (2 * counterKey - tableDS) / 2;
	int r = pow(10, e);
	keyN = (keyN * keyN) / r;
	int mod = pow(10, tableDS);
	return (keyN) % mod;
}

 int HashFunctions::multiplicationMethod(int key, int tableSize) {
	const double A = 0.6180339887;
	double temp = abs(key) * A;
	temp = temp - floor(temp);
	return (int)(tableSize * temp);
}