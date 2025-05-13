package main

import (
	"fmt"
	"math"
)

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func findWinner(n, m int, nums []int) bool {
	dp := make([]int, n+1)
	for i := range dp {
		dp[i] = math.MinInt
	}
	dp[n] = 0

	for i := n - 1; i >= 0; i-- {
		currSum := 0
		for k := 1; k <= m && i+k <= n; k++ {
			currSum += nums[i+k-1]
			dp[i] = max(dp[i], currSum-dp[i+k])
		}
	}
	return dp[0] > 0
}

func main() {
	var n, m int
	fmt.Print("Введите n (5 ≤ n ≤ 50000) и m (4 ≤ m ≤ 100): ")
	fmt.Scan(&n, &m)

	nums := make([]int, n)
	fmt.Print("Последовательность: ")
	for i := 0; i < n; i++ {
		fmt.Scan(&nums[i])
	}

	if findWinner(n, m, nums) {
		fmt.Println("1")
	} else {
		fmt.Println("0")
	}
}
