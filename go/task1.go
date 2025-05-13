package main

import (
	"fmt"
)

func f(x float64) float64 {
	if x <= -7 {
		return 0
	}
	if x > -7 && x < -3 {
		return x + 7
	}
	if x >= -3 && x < -2 {
		return 4
	}
	if x >= -2 && x < 2 {
		return x * x
	}
	if x >= 2 && x < 4 {
		return -2*x + 8
	}
	if x >= 4 {
		return 0
	}
	return 0
}

func main() {
	var x0, x1, dx float64
	fmt.Print("Введите x0: ")
	fmt.Scan(&x0)
	fmt.Print("Введите x1: ")
	fmt.Scan(&x1)
	fmt.Print("Введите dx: ")
	fmt.Scan(&dx)

	fmt.Println("  x  y")
	for x := x0; x <= x1; x += dx {
		fmt.Printf("%3.1f  %3.1f\n", x, f(x))
	}
}
