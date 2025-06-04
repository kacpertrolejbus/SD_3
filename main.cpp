#include <iostream>
#include "HashTable.hpp"
#include "PerfoTest.hpp"

using namespace std;

void demo() {
    cout << "\n" << string(50, '=') << endl;
    cout << "DEMONSTRACJA PODSTAWOWYCH OPERACJI" << endl;
    cout << string(50, '=') << endl;

    HashTable ht(11, HashTable::DIVISION);

    cout << "\n1. Dodawanie elementów:" << endl;

    vector<pair<int, int>> testData = {
        {10, 100}, {22, 220}, {31, 310}, {4, 40}, {15, 150}
    };

    for (auto& pair : testData) {
        auto result = ht.insert(pair.first, pair.second);
        cout << "Dodanie klucza " << pair.first << ": "
            << (result.first ? "SUKCES" : "BŁĄD")
            << " (kroki: " << result.second << ")" << endl;
    }

    ht.printStats();

    cout << "\n2. Wyszukiwanie elementów:" << endl;

    vector<int> searchKeys = { 10, 22, 99, 4 };

    for (int key : searchKeys) {
        auto result = ht.search(key);
        cout << "Wyszukanie klucza " << key << ": "
            << (result.first ? "ZNALEZIONO" : "NIE ZNALEZIONO")
            << " (kroki: " << result.second << ")" << endl;
    }

    cout << "\n3. Usuwanie elementów:" << endl;

    vector<int> removeKeys = { 22, 99, 4 };

    for (int key : removeKeys) {
        auto result = ht.remove(key);
        cout << "Usunięcie klucza " << key << ": "
            << (result.first ? "SUKCES" : "NIE ZNALEZIONO")
            << " (kroki: " << result.second << ")" << endl;
    }

    cout << "\n4. Stan tablicy po operacjach:" << endl;
    ht.printStats();

    cout << "\n5. Sprawdzenie po usunięciu:" << endl;

    for (int key : removeKeys) {
        auto result = ht.search(key);
        cout << "Wyszukanie usuniętego klucza " << key << ": "
            << (result.first ? "ZNALEZIONO" : "NIE ZNALEZIONO")
            << " (kroki: " << result.second << ")" << endl;
    }
}

void demoHF() {
    cout << "\n" << string(50, '=') << endl;
    cout << "DEMONSTRACJA RÓŻNYCH FUNKCJI HASZUJĄCYCH" << endl;
    cout << string(50, '=') << endl;

    vector<int> testKeys = { 123, 456, 789, 1011, 1213 };
    int tableSize = 13;

    cout << "\nPorównanie funkcji haszujących dla kluczy: ";
    for (int key : testKeys) {
        cout << key << " ";
    }
    cout << endl;
    cout << "Rozmiar tablicy: " << tableSize << endl;

    vector<pair<HashTable::HashF, string>> functions = {
        {HashTable::DIVISION, "Metoda dzielenia"},
        {HashTable::MID_SQUARE, "Metoda środka kwadratu"},
        {HashTable::MULTIPLICATION, "Metoda mnożenia"}
    };

    for (auto& func : functions) {
        cout << "\n--- " << func.second << " ---" << endl;
        HashTable ht(tableSize, func.first);

        for (int key : testKeys) {
            auto result = ht.insert(key, key * 10);
            HashFunctions hf;
            int hashValue;
            switch (func.first) {
            case HashTable::DIVISION:
                hashValue = hf.divisionMethod(key, tableSize);
                break;
            case HashTable::MID_SQUARE:
                hashValue = hf.midSquareMethod(key, tableSize);
                break;
            case HashTable::MULTIPLICATION:
                hashValue = hf.multiplicationMethod(key, tableSize);
                break;
            }

            cout << "Klucz " << key << " -> hash: " << hashValue
                << ", wstawiony na pozycji: " << result.second - 1 << " kroku" << endl;
        }
    }
}

void showMenu() {
    cout << "\n" << string(50, '=') << endl;
    cout << "MENU PROGRAMU - TABLICE HASZUJĄCE" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Demonstracja podstawowych operacji" << endl;
    cout << "2. Porównanie funkcji haszujących" << endl;
    cout << "3. Test wydajności - wszystkie przypadki (zapis do pliku)" << endl;
    cout << "4. Test przypadku optymistycznego" << endl;
    cout << "5. Test przypadku średniego" << endl;
    cout << "6. Test przypadku pesymistycznego" << endl;
    cout << "7. Porównanie funkcji haszujących (jeden rozmiar)" << endl;
    cout << "8. Test wielorozmiarowy (zapis do pliku)" << endl;
    cout << "9. Porównanie funkcji na różnych rozmiarach (zapis do pliku)" << endl;
    cout << "0. Wyjście" << endl;
    cout << string(50, '=') << endl;
    cout << "Wybierz opcję: ";
}

int main() {
    cout << "PROJEKT 3 - TABLICE HASZUJĄCE" << endl;
    cout << "Implementacja i porównanie funkcji haszujących" << endl;
    cout << "z zapisem wyników do pliku" << endl;
    cout << "Rozmiary tablicy: 101, 503, 1009, 2003, 5003" << endl;

    PerfoTest tester;
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            demo();
            break;

        case 2:
            demoHF();
            break;

        case 3:
            cout << "\nUruchamianie kompleksowego testu wydajności..." << endl;
            cout << "Wyniki będą zapisane do pliku 'wyniki_testow.txt'" << endl;
            tester.runCompleteTest(500);
            break;

        case 4: {
            cout << "\nTest przypadku optymistycznego (dane losowe)..." << endl;
            auto data = tester.genRandomData(500);
            tester.testCase("Optymistyczny", data, 1009);
            break;
        }

        case 5: {
            cout << "\nTest przypadku średniego (dane sekwencyjne)..." << endl;
            auto data = tester.genSequentialData(500);
            tester.testCase("Średni", data, 1009);
            break;
        }

        case 6: {
            cout << "\nTest przypadku pesymistycznego (dane klastrowane)..." << endl;
            auto data = tester.genClusteredData(500);
            tester.testCase("Pesymistyczny", data, 1009);
            break;
        }

        case 7:
            cout << "\nSzybkie porównanie funkcji haszujących..." << endl;
            tester.compareHF(500);
            break;

        case 8:
            cout << "\nTest wielorozmiarowy..." << endl;
            cout << "Wyniki będą zapisane do pliku 'test_wielorozmiarowy.txt'" << endl;
            tester.runMultiSizeTest(500);
            break;

        case 9:
            cout << "\nPorównanie funkcji na różnych rozmiarach..." << endl;
            cout << "Wyniki będą zapisane do pliku 'porownanie_funkcji.txt'" << endl;
            tester.compareHFMultiSize(500);
            break;

        case 0:
            cout << "\nDziękujemy za skorzystanie z programu!" << endl;
            break;

        default:
            cout << "\nNieprawidłowa opcja! Spróbuj ponownie." << endl;
            break;
        }

        if (choice != 0) {
            cout << "\nNaciśnij Enter, aby kontynuować...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);

    return 0;
}