package main

import (
	"fmt"
	"math"
	"math/rand"
	"sort"
	"time"
)

var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))

func uniRand(a, b int) int {
	return rnd.Intn(b-a+1) + a
}

func uniRandD(a, b int) float64 {
	return rnd.Float64()*float64(b-a) + float64(a)
}

// возводим в степень по модулю
func powModDec(a, x, p int) int {
	res := 1
	a %= p
	for x > 0 {
		if x%2 == 1 {
			res = (res * a) % p
		}
		a = (a * a) % p
		x /= 2
	}
	return res
}

// делаем решето эратосфена
func eratSieve(limit int) []int {
	isPrime := make([]bool, limit)
	for i := range isPrime {
		isPrime[i] = true
	}
	isPrime[0], isPrime[1] = false, false

	for i := 2; i*i < limit; i++ {
		if isPrime[i] {
			for j := i * i; j < limit; j += i {
				isPrime[j] = false
			}
		}
	}

	var primes []int
	for i := 2; i < limit; i++ {
		if isPrime[i] {
			primes = append(primes, i)
		}
	}
	return primes
}

func builderTest(primes []int, bitLen int) (int, []int) {
	maxIndex := 0
	for maxIndex < len(primes) && primes[maxIndex] < int(math.Pow(2, float64(bitLen-1))) {
		maxIndex++
	}
	maxPow := 1
	for math.Pow(2, float64(maxPow)) < math.Pow(2, float64(bitLen-1)) {
		maxPow++
	}

	m := 1
	var q []int

	for {
		num := uniRand(0, maxIndex-1)
		power := uniRand(1, maxPow)
		powVal := int(math.Pow(float64(primes[num]), float64(power)))

		if powVal != 0 && float64(m)*float64(powVal) < float64(math.MaxInt32) {
			m *= powVal
			q = append(q, primes[num])
		}

		if float64(m) > math.Pow(2, float64(bitLen-2)) {
			if float64(m) >= math.Pow(2, float64(bitLen-1)) {
				m = 1
				q = nil
			} else {
				break
			}
		}
	}

	n := 2*m + 1
	return n, q
}

func millerTest(n, t int, q []int) bool {
	var a []int
	for len(a) != t {
		aj := uniRand(2, n-1)
		if !contains(a, aj) {
			a = append(a, aj)
		}
	}

	for _, aj := range a {
		if powModDec(aj, n-1, n) != 1 {
			return false
		}
	}

	for _, qi := range q {
		pass := false
		for _, aj := range a {
			if powModDec(aj, (n-1)/qi, n) != 1 {
				pass = true
				break
			}
		}
		if !pass {
			return false
		}
	}
	return true
}

func poklingtonTest(n, t int, q []int) bool {
	var a []int
	for len(a) != t {
		aj := uniRand(1, n)
		if !contains(a, aj) {
			a = append(a, aj)
		}
	}

	for _, aj := range a {
		if powModDec(aj, n-1, n) != 1 {
			return false
		}
	}

	for _, aj := range a {
		flag := true
		for _, qi := range q {
			if qi != 0 && powModDec(aj, (n-1)/qi, n) == 1 {
				flag = false
				break
			}
		}
		if flag {
			return true
		}
	}
	return false
}

func buildNewFromOld(primes []int, bit int) int {
	maxIndex := 0
	for primes[maxIndex] < int(math.Pow(2, float64(bit/2))) {
		maxIndex++
	}

	var q int
	for {
		q = primes[uniRand(0, maxIndex)]
		if q > 0 && q <= int(math.Pow(2, float64(bit/2))-1) {
			break
		}
	}

	for {
		eps := uniRandD(0, 1)
		n0 := math.Pow(2, float64(bit-1))/float64(q) + (math.Pow(2, float64(bit-1))*eps)/float64(q)
		n := int(math.Ceil(n0))
		if n%2 == 1 {
			n++
		}

		for u := 0; ; u += 2 {
			p := (n+u)*q + 1
			if float64(p) > math.Pow(2, float64(bit)) {
				break
			}
			if powModDec(2, p-1, p) == 1 && powModDec(2, n+u, p) != 1 {
				return p
			}
		}
	}
}

func resultsPrint(nums []int, good []bool, declined []int) {
	fmt.Println("Простое число\tРезультат теста\tОтвергнутые")
	for i := range nums {
		res := "-"
		if good[i] {
			res = "+"
		}
		fmt.Printf("%13d%18s%12d\n", nums[i], res, declined[i])
	}
}

func contains(slice []int, x int) bool {
	for _, val := range slice {
		if val == x {
			return true
		}
	}
	return false
}

func main() {
	var lenBit int
	fmt.Print("Введите длину желаемого числа (1-32 бит): ")
	fmt.Scan(&lenBit)

	fmt.Print("Укажите желаемый тип теста (1 – Миллера, 2 – Поклингтона, 3 – ГОСТ): ")
	var testType int
	fmt.Scan(&testType)

	primes := eratSieve(500)

	switch testType {
	case 1:
		fmt.Println("Получение простых чисел заданной длины на основе теста Миллера")
		var nums []int
		var good []bool
		var declined []int
		k := 0
		for len(nums) < 10 {
			n, q := builderTest(primes, lenBit)
			if millerTest(n, 10, q) {
				if !contains(nums, n) {
					nums = append(nums, n)
					good = append(good, millerTest(n, 1, q))
					declined = append(declined, k)
					k = 0
				}
			} else {
				k++
			}
		}
		resultsPrint(nums, good, declined)
	case 2:
		fmt.Println("Получение простых чисел заданной длины на основе теста Поклингтона")
		var nums []int
		var good []bool
		var declined []int
		k := 0
		for len(nums) < 10 {
			n, q := builderTest(primes, lenBit)
			if poklingtonTest(n, 10, q) {
				if !contains(nums, n) {
					nums = append(nums, n)
					good = append(good, poklingtonTest(n, 1, q))
					declined = append(declined, k)
					k = 0
				}
			} else {
				k++
			}
		}
		resultsPrint(nums, good, declined)
	case 3:
		fmt.Println("Генерация простых чисел заданной длины на основе ГОСТ 34.10-94")
		var res []int
		for len(res) < 10 {
			p := buildNewFromOld(primes, lenBit)
			if !contains(res, p) {
				res = append(res, p)
			}
		}
		sort.Ints(res)
		for i, v := range res {
			fmt.Printf("%3d%8d\n", i+1, v)
		}
	default:
		fmt.Println("Так не пойдет, завершаю исполнение")
	}
}
