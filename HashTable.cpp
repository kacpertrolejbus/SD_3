#include <iostream>
#include <iomanip>
#include "HashTable.hpp"
#include "HashFunc.hpp"

HashN::HashN() : key(-1), val(-1), del(false) {}
HashN::HashN(int k, int v) : key(k), val(v), del(false) {}
HashTable::HashTable(int tableSize, HashF func) : size(tableSize), count(0), currHF(func)
{
	table.resize(size);
}
HashTable::~HashTable() {}

int HashTable::hash(int key)
{
	switch (currHF)
	{
	case DIVISION:
		return hashFunctions.divisionMethod(key, size);
	case MID_SQUARE:
		return hashFunctions.midSquareMethod(key, size);
	case MULTIPLICATION:
		return hashFunctions.multiplicationMethod(key, size);
	default:
		return hashFunctions.divisionMethod(key, size);
	}
}

int HashTable::findSlot(int key, bool forInsertion)
{
	int index = hash(key);
	int orgIndex = index;

	do {
		if (forInsertion)
		{
			if (table[index].key == -1 || table[index].del)
			{
				return index;
			}
			if (table[index].key == key && !table[index].del)
			{
				return index;
			}
		}
		else
		{
			if (table[index].key == -1)
			{
				return -1;
			}
			if (table[index].key == key && !table[index].del)
			{
				return index;
			}
		}
		index = (index + 1) % size;
	} while (index != orgIndex);
	return -1;
}

pair<bool, int> HashTable::insert(int key, int val)
{
	if(count >= size * 0.75)
	{
		return { false, 0 };
	}
	int steps = 0;
	int index = hash(key);
	int orgIndex = index;

	do {
		steps++;
		if (table[index].key == -1 || table[index].del)
		{
			if (table[index].del)
			{
				table[index].del = false;
			}
			else
			{
				count++;
			}
			table[index].key = key;
			table[index].val = val;
			return { true, steps };
		}
		if (table[index].key == key && !table[index].del)
		{
			table[index].val = val;
			return { true, steps };
		}
		index = (index + 1) % size;
	} while (index != orgIndex);
	return { false, steps };
}

pair<bool, int> HashTable::remove(int key)
{
	int steps = 0;
	int index = hash(key);
	int orgIndex = index;
	do {
		steps++;
		if (table[index].key == -1)
		{
			return { false, steps };
		}
		if (table[index].key == key && !table[index].del)
		{
			table[index].del = true;
			return { true, steps }; 
		}
		index = (index + 1) % size;
	} while (index != orgIndex);
	return { false, steps };
}

pair<bool, int> HashTable::search(int key)
{
	int steps = 0;
	int index = hash(key);
	int orgIndex = index;
	do {
		steps++;
		if (table[index].key == -1)
		{
			return{ false, steps };
		}
		if (table[index].key == key && !table[index].del)
		{
			return { true, steps };
		}
		index = (index + 1) % size;
	} while (index != orgIndex);
	return { false, steps };
}

void HashTable::clear() {
	table.clear();
	table.resize(size);
	count = 0;
}

void HashTable::printTable() {
	std::cout << "\n=== ZAWARTOŚĆ TABLICY ===" << std::endl;
	for (int i = 0; i < size; i++) {
		std::cout << "[" << std::setw(3) << i << "] ";
		if (table[i].key == -1) {
			std::cout << "PUSTE";
		}
		else if (table[i].del) {
			std::cout << "USUNIĘTE (key: " << table[i].key << ")";
		}
		else {
			std::cout << "key: " << table[i].key << ", value: " << table[i].val;
		}
		std::cout << std::endl;
	}
}

void HashTable::printStats() {
	int active = 0, deleted = 0, empty = 0;

	for (const auto& node : table) {
		if (node.key == -1) {
			empty++;
		}
		else if (node.del) {
			deleted++;
		}
		else {
			active++;
		}
	}

	std::cout << "\n=== STATYSTYKI TABLICY ===" << std::endl;
	std::cout << "Rozmiar tablicy: " << size << std::endl;
	std::cout << "Aktywne elementy: " << active << std::endl;
	std::cout << "Usunięte elementy: " << deleted << std::endl;
	std::cout << "Puste miejsca: " << empty << std::endl;
	std::cout << "Load factor: " << std::fixed << std::setprecision(3)
		<< (double)active / size << std::endl;
}

double HashTable::getLoadFactor() {
	return (double)count / size;
}

int HashTable::getSize() {
	return size;
}

int HashTable::getCount() {
	return count;
}

void HashTable::setHF(HashF func) {
	currHF = func;
}

HashTable::HashF HashTable::getCurrHF() {
	return currHF;
}

