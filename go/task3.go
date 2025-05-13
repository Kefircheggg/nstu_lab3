package main

import (
	"fmt"
	"math"
)

// НОД по расширенному алгоритму Евклида
func gcd(a, b int) int {
	X := [2]int{a, 0}
	Y := [2]int{b, 1}

	for Y[0] != 0 {
		q := X[0] / Y[0]
		T := [2]int{X[0] - q*Y[0], X[1] - q*Y[1]}
		X = Y
		Y = T
	}
	return X[0]
}

// считаем сумму ряда
func findSeriesSum(a, b int) float64 {
	cur := 1.0 / float64(b)
	sumPast := cur
	sum := cur

	for n := 2; n < 1000; n++ {
		cur = math.Pow(float64(n), float64(a)) / math.Pow(float64(b), float64(n))
		sum += cur
		if math.Abs(sum-sumPast) < 0.000001 {
			return sum
		}
		sumPast = sum
	}
	return -1
}

// пытаемся представить как рациональное
func isPossiblyRational(x float64) bool {
	for b := 1; b <= 10000; b++ {
		a := int(math.Round(x * float64(b)))
		if math.Abs(x-float64(a)/float64(b)) < 0.000001 {
			commonDiv := gcd(a, b)
			fmt.Printf("Сумма — рациональное число: %d/%d\n", a/commonDiv, b/commonDiv)
			return true
		}
	}
	return false
}

func main() {
	var a, b int
	fmt.Print("Введите целые a и b: ")
	fmt.Scan(&a, &b)

	serSum := findSeriesSum(a, b)

	if serSum == -1 {
		fmt.Println("Ряд расходится (бесконечная сумма)")
	} else if !isPossiblyRational(serSum) {
		fmt.Printf("Сумма — иррациональное число: %f\n", serSum)
	}
}
