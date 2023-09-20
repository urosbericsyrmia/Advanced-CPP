#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

void printVector(const std::vector<int>& vec) {
    std::for_each(vec.begin(), vec.end(), [](const int& val) {
        std::cout << val << " ";
    });
    std::cout << std::endl;
}

int main() {
    std::vector<int> prviVektor;

    std::cout << "Nakon definisanja vektora:\t\t";
    printVector(prviVektor);
    std::cout << std::endl;

    std::fill_n(std::back_inserter(prviVektor), 20, 5);
    std::for_each(prviVektor.begin(), prviVektor.end(), [counter = 0](int& val) mutable {
        val *= ++counter;
    });

    std::cout << "Nakon dodavanja elemenata u vektor:\t";
    printVector(prviVektor);
    std::cout << std::endl;

    std::for_each(prviVektor.begin(), prviVektor.end(), [](int& val) {
        val *= val;
    });

    std::cout << "Nakon kvadriranja elemenata:\t\t";
    printVector(prviVektor);
    std::cout << std::endl;

    std::vector<int> drugiVektor;
    std::copy_if(prviVektor.begin(), prviVektor.end(), std::back_inserter(drugiVektor), [](const int& val) {
        return val % 7;
    });

    std::cout << "Nakon kopiranja elemenata iz prvog u drugi vektor:\n\n";
    std::cout << "\tPrvi vektor:\t\t";
    printVector(prviVektor);
    std::cout << "\n\tDrugi vektor:\t\t";
    printVector(drugiVektor);
    std::cout << std::endl;

    double reciprocnaSuma = std::accumulate(drugiVektor.begin(), drugiVektor.end(), 0.0, [](double x, double y) {
        return x + 1.0 / y;
    });

    std::cout << "Suma reciprocnih vrednosti elemenata:\t" << reciprocnaSuma << std::endl << std::endl;

    drugiVektor.erase(std::remove_if(drugiVektor.begin(), drugiVektor.end(), [](const int& val) {
        return val % 9 == 0;
    }), drugiVektor.end());

    std::cout << "Nakon brisanja elemenata:\t\t";
    printVector(drugiVektor);
    std::cout << std::endl;

    auto it = std::find_if(drugiVektor.begin(), drugiVektor.end(), [](const int& val) {
        return val % 13 == 0;
    });

    std::cout << "Indeks elementa koji je deljiv sa 13:\t";
    if (it != drugiVektor.end()) {
        std::cout << it - drugiVektor.begin();
    }
    else {
        std::cout << -1;
    }
    std::cout << std::endl << std::endl;

    return 0;
}
