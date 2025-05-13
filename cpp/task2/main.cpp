#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>
#include <iomanip>

using namespace std;

// генератор случайных чисел
mt19937 mt_rand(random_device{}());

// генерация рандомных чисел с равной вероятностью
int uniRand(int a, int b) { return uniform_int_distribution<int>(a, b)(mt_rand); }
double uniRandD(int a, int b) { return uniform_real_distribution<double>(a, b)(mt_rand); }

// возведение в степень по модулю через двоичное разложение
long powModDecopmose(long a, long x, long p) {
    long res = 1; // результат единичка чтобы было на что умножать
    a%=p; // приводим по модулю
    while (x > 0) { // пока от числа вообще что-то осталось
        // если бит (с младшего) - 1 домножаем результат на остаток от деления a^2^i на кольце
        if (x % 2 == 1) { res = (res * a) % p; }
        a = (a * a) % p; // остаток считаем каждый раз чтобы не считать страшные степени
        x /= 2; // ну и делим само число
    }
    return res;
}
// решето эратосфена
vector<int> eratSieve(const int limit) {
    vector<bool> is_prime(limit, true);
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i < limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j < limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    vector<int> primes;
    for (int i = 2; i < limit; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

// Построение числа n
pair<int, vector<int>> builderTest(const vector<int> &primes, int bit_len) {
  int max_index = 0; // индекс используемый для выбора простых чисел из primes
  int max_pow = 1; // степень до которой мы будем возводить числа из primes

  while (max_index < primes.size() && primes[max_index] < pow(2, bit_len - 1))
    max_index++;
  while (pow(2, max_pow) < pow(2, bit_len - 1))
    max_pow++;

  int m = 1;
  vector<int> q;

  while (true) {
    int num = uniRand(0, max_index - 1);
    int power = uniRand(1, max_pow);

    if (pow(primes[num], power) != 0) { // проверка и добавление числа
      if ((double)m * pow(primes[num], power) < INT32_MAX) {
        m *= pow(primes[num], power);
        q.push_back(primes[num]);
      }
    }

    if (m > pow(2, bit_len - 2)) { // проверка размера числа
      if (m >= pow(2, bit_len - 1)) {
        m = 1;
        q.clear();
      } else {
        break;
      }
    }
  }

  int n = 2 * m + 1;
  return make_pair(n, q);
}

// Тест Миллера
int millerTest(int n, int t, const vector<int> &q) {
  vector<int> a; // числа для проверки 1<aj<n
  while (a.size() != t) {
    int aj = uniRand(2, n - 1);
    if (find(a.begin(), a.end(), aj) == a.end()) {
      a.push_back(aj);
    }
  }

  for (int aj : a) { // проверяем степень числа
    if (powModDecopmose(aj, n - 1, n) != 1) {
      return 0;
    }
  }

  for (int qi : q) {
    bool millerCrit = true;
    for (int aj : a) {
      if (powModDecopmose(aj, (n - 1) / qi, n) != 1) {
        millerCrit = false;
        break;
      }
    }
    if (millerCrit) {
      return 0;
    }
  }

  return 1;
}

int poklingtonTest(int n, int t, const vector<int> &q) {
    vector<int> a;
    int aj;

    while (a.size() != t) {
        aj = uniRand(1, n);

        if (find(a.begin(), a.end(), aj) == a.end()) {
            a.push_back(aj);
        }
    }
    // 1ая проверка
    for (int aj : a) {
        if (powModDecopmose(aj, n - 1, n) != 1) {
            return 0;
        }
    }
    // 2ая проверка
    for (int aj : a) {
        bool flag = true;
        for (int qi : q) {
            if (qi != 0 && powModDecopmose(aj, (n - 1) / qi, n) == 1) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return 1;
        }
    }

    return 0;
}

// Алгоритм перехода от меньшего простого числа к большему
int build_new_from_old(vector<int> prime, int bit) {
    int q;
    int max_index = 0;
    int p;

    for (; prime[max_index] < pow(2, bit / 2); max_index++)
        ;

    while (true) {
        q = prime[uniRand(0, max_index)];
        if (q > 0 && q <= pow(2, bit / 2) - 1)
            break;
    }

    while (true) {
        double epsilon = uniRandD(0, 1);
        double n0 = (pow(2, bit - 1) / q) + (pow(2, bit - 1) * epsilon / q);
        int n = ceil(n0);

        if ((int)n % 2 == 1)
            n++;

        for (int u = 0; true; u += 2) {
            p = (n + u) * q + 1;
            if (p > pow(2, bit))
                break;

            if (powModDecopmose(2, p - 1, p) == 1 && powModDecopmose(2, n + u, p) != 1)
                return p;
        }
    }
}

void resultsPrint(const vector<int>& nums, const vector<bool>& nums_good, const vector<int>& declined) {
    cout << "Простое число\tРезультат теста\tОтвергнутые\n";

    for (size_t i = 0; i < nums.size(); ++i) {
        string testRes = nums_good[i] ? "+" : "-";
        cout << setw(13) << nums[i] << setw(18) << testRes << setw(12) << declined[i]
             << endl;
    }
}

int main() {
  int maxPrime = 500;
  vector<int> primes = eratSieve(maxPrime);
  int len_bit;
  cout << "Введите длину желаемого числа (1-32 бит): ";
  cin >> len_bit;
  int testType;
  cout << "Укажите желаемый тип теста (1 – Миллера, 2 – Поклингтона, 3 – ГОСТ): ";
  cin >> testType;
  switch (testType) {
      case 1: {
          cout << "Получение простых чисел заданной длины на основе теста Миллера\n";
          vector<int> q;
          int n;
          int k = 0;
          int prob;
          vector<int> nums;
          vector<bool> nums_good;
          vector<int> declined;
          while (nums.size() < 10) {
              tie(n, q) = builderTest(primes, len_bit);
              prob = millerTest(n, 10, q);

              if (prob == 1) {
                  if (find(nums.begin(), nums.end(), n) == nums.end()) {
                      nums.push_back(n);

                      prob = millerTest(n, 1, q);
                      nums_good.push_back(prob);

                      declined.push_back(k);
                      k = 0;
                  }
              } else {
                  k++;
              }
          }

          resultsPrint(nums,nums_good,declined);
          break;
      }
      case 2: {
          cout << "Получение простых чисел заданной длины на основе теста Поклингтона\n";
          vector<int> q;
          int n;
          int k = 0;
          int probability;
          vector<int> nums;
          vector<bool> nums_good;
          vector<int> declined;
          while (nums.size() != 10) {
              tie(n, q) = builderTest(primes, len_bit);
              probability = poklingtonTest(n, 10, q);

              if (probability == 1) {
                  if (find(nums.begin(), nums.end(), n) == nums.end()) {
                      nums.push_back(n);

                      nums_good.push_back(poklingtonTest(n, 1, q));

                      declined.push_back(k);
                      k = 0;
                  }
              }

              else {
                  k++;
              }
          }
          resultsPrint(nums, nums_good, declined);
          break;
      }
      case 3: {
          cout << "Генерация простых чисел заданной длины на основе ГОСТ 34.10-94\n";
          vector<int> res;
          while (res.size() != 10) {
              int p = build_new_from_old(primes, len_bit);

              if (find(res.begin(), res.end(), p) == res.end()) {
                  res.push_back(p);
              }
          }
          for (int i = 1; i <= res.size(); i++) {
              cout << setw(3) << i << setw(8) << res[i-1] << endl;
          }

          break;
      }
      default: {
          cout << "Так не пойдет, завершаю исполнение\n";
          return 0;
      }
  }
}