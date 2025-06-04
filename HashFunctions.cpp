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
	 if (keyN == 0) return 1;
	 int count = 0;
	 keyN = abs(keyN);
	 while (keyN != 0) {
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
	 long long keySquared = (long long)abs(key) * abs(key);
	 string squared = to_string(keySquared);
	 int totalDigits = squared.length();
	 int digitsNeeded = counter(tableSize - 1);
	 if (totalDigits <= digitsNeeded) {
		 return keySquared % tableSize;
	 }
	 int startPos = (totalDigits - digitsNeeded) / 2;
	 string middlePart = squared.substr(startPos, digitsNeeded);
	 int result = stoi(middlePart);
	 return result % tableSize;
}

 int HashFunctions::multiplicationMethod(int key, int tableSize) {
	const double A = 0.6180339887;
	double temp = abs(key) * A;
	temp = temp - floor(temp);
	return (int)(tableSize * temp);
}