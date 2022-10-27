package main

import (
	"fmt"
)

func rightFactoriel(n int) int {
	if n < 2 {
		return 1
	}
	return n * rightFactoriel(n-1)
}

func leftFactoriel(n int) int {
	var sum = 0
	for i := 0; i < n; i++ {
		sum += rightFactoriel(i)
	}
	return sum
}

func gcd(a int, b int) int {
	if a == 0 {
		return b
	}
	return gcd(b%a, a)
}

func main() {
	const n = 12
	for i := 1; i <= n; i++ {
		var lf = leftFactoriel(i)
		var rf = rightFactoriel(i)
		fmt.Println(i, lf, rf, gcd(lf, rf))
	}
}
