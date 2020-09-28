#include <iostream>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <utility>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace NTL;
using namespace std::chrono;

const std::pair<ZZ, ZZ> operator%(const std::pair<ZZ, ZZ> a, const std::pair<ZZ, ZZ> b) {
    auto Nb = sqr(b.first) + sqr(b.second);
    
    // Oops, it's actually (a * b_conj) % N(b)
    std::pair<ZZ, ZZ> ab_conj(0, 0);
    ab_conj.first = (a.first * b.first + a.second * b.second) % Nb;
    ab_conj.second = (a.second * b.first - a.first * b.second) % Nb;
    
    if (ab_conj.first > Nb / 2) {
        ab_conj.first -= Nb;
    } else if (ab_conj.first < - Nb / 2) {
        ab_conj.first += Nb;
    }
    
    if (ab_conj.second > Nb / 2) {
        ab_conj.second -= Nb;
    } else if (ab_conj.second < - Nb / 2) {
        ab_conj.second += Nb;
    }
    
    // Now multiply the remainder above by b and divide again by N(b)
    std::pair<ZZ, ZZ> x(0, 0);
    x.first = (ab_conj.first * b.first - ab_conj.second * b.second) / Nb;
    x.second = (ab_conj.second * b.first + ab_conj.first * b.second) / Nb;
    return x;
}

const std::pair<ZZ, ZZ> gcd(const std::pair<ZZ, ZZ> a, const std::pair<ZZ, ZZ> b) {
    auto c = a;
    auto d = b;
    while (d.first != 0 || d.second != 0) {
        c = c % d;
        swap(c, d);
    }
    
    return c;
}


int main() {
    cout << "Enter a prime number: p = ";
    ZZ p;
    cin >> p;
    if (!ProbPrime(p)) {
        cout << "Your choice of p is not prime." << endl;
        return 0;
    }
    
    auto start = steady_clock::now();
    
    cout << "Finding a pair of positive integers (x, y) with x^2 + y^2 = p" << endl;
    
    if (p == 2) {
        cout << "(x, y) = (1, 1)" << endl;
    } else if (p % 4 != 1) {
        cout << "There is no such pair." << endl;
    } else {
        // Else p == 1 mod 4
        const ZZ p_ZZ = ZZ(p);
        ZZ z = SqrRootMod(p_ZZ - 1, p_ZZ);
    
        // Now z = sqrt(-1) so in Z[i], p | N(y + i)
        // The rest is finding gcd(z + i, p)
        auto w = gcd(std::pair<ZZ, ZZ>(z, ZZ(1)), std::pair<ZZ, ZZ>(p_ZZ, ZZ(0)));
        if (sqr(w.first) + sqr(w.second) == p) {
            cout << "(x, y) = (" << abs(w.first) << ", " << abs(w.second) << ")" << endl;
        } else {
            cout << "Error!" << endl;
        }
    }
    
    auto end = steady_clock::now();
    const long total_duration = duration_cast<microseconds>(end - start).count();
    const long seconds = total_duration / 1000000;
    const long microseconds = total_duration % 1000000;
    
    cout << "Run in " << seconds << "." << setw(6) << setfill('0') << microseconds << " seconds." << endl;
}
