#include <iostream>
#include <iomanip>

using namespace std;

double func(double x) {
    if (x <= -7) return 0;
    if (x>-7 && x < -3) return x+7;
    if (x>=-3 && x<-2) return 4;
    if (x>=-2 && x < 2) return x*x;
    if (x >= 2 && x < 4) return -2*x+8;
    if (x>=4) return 0;

    return 0;
}

int main() {
    double x0, x1, dx;
    cout << "Введите x0: ";
    cin >> x0;
    cout << "Введите x1: ";
    cin >> x1;
    cout << "Введите dx: ";
    cin >> dx;

    cout << "  x  y\n";
    for (double i = x0; i <= x1; i+=dx) {
        cout << setw(3) << i << setw(3) <<func(i) << endl;
    }
    return 0;
}