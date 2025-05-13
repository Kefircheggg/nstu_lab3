#include <cmath>
#include <iostream>

using namespace std;

int gcd(int a, int b) {
    pair<int,int> X = {a,0};
    pair<int,int> Y = {b,1};

    while (Y.first!=0) {
        int q = X.first / Y.first;
        pair<int,int> T = {X.first-q*Y.first, X.second-q*Y.second};
        X = Y; Y = T;
    }
    return X.first;
}

double findSeriesSum(int a, int b) {
  double cur;
  double sum_past = 0, sum = 0;

  cur = 1. / b;
  sum_past = cur;
  sum = cur;

  for (int n = 2; n < 1000; n++) {
    cur = pow(n, a) / pow(b, n);
    sum += cur;
    if (fabs(sum - sum_past) < 0.000001) {
      return sum;
    }
    sum_past = sum;
  }
  return -1;
}

bool is_possibly_rational(double x) {
    for (int b = 1; b <= 10000; b++) {
        int a = (int)round(x * b);
        if (fabs(x - (double)a / b) < 0.000001) {
            int common_divisor = gcd(a, b);
            cout << "Сумма — рациональное число: "
                 << a / common_divisor << "/" << b / common_divisor << endl;
            return true;
        }
    }
    return false;
}

int main(void) {
    int a, b;
    cout << "Введите целые a и b: ";
    cin >> a >> b;

    double serSum = findSeriesSum(a, b);

    if (serSum == -1) {
        cout << "Ряд расходится (бесконечная сумма)" << endl;
    } else if (!is_possibly_rational(serSum)) {
        cout << "Сумма — иррациональное число: " << serSum << endl;
    }

    return 0;
}
