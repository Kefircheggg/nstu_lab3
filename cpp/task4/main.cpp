#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
using namespace std;

bool findWinner(int n, int m, const vector<int> &nums) {
  // dp[i] — макс. разница очков (Павел - Вика),
  vector<int> dp(n + 1, INT_MIN);
  dp[n] = 0; // если все числа уже удалены, разница очков = 0

    // идем от обратного т.к результат предречен
    for (int i = n - 1; i >= 0; --i) {
    int current_sum = 0; // сумма чисел, которые мы возьмём на этом шаге

    // Пробуем взять от 1 до m чисел если они остались
    for (int k = 1; k <= m && i + k <= n; ++k) {
      current_sum += nums[i + k - 1];

      // dp[i + k] — это разница очков, которую получит соперник после нас
      // Т.е. забирает current_sum, потом соперник играет с позиции i + k
      dp[i] = max(dp[i], current_sum - dp[i + k]);
    }
  }

  return dp[0] > 0 ? 1 : 0;
}

int main() {
  int n, m;
  cout << "Введите n (5 ≤ n ≤ 50000) и m (4 ≤ m ≤ 100): ";
  cin >> n >> m;
  
  vector<int> nums(n);
  cout << "Последовательность: ";
  vector<int> nums(n);
  for (int i = 0; i < n; ++i) {
      cin >> nums[i];
  }

  cout << findWinner(n, m, nums);

  return 0;
}
