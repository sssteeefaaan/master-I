package main

import (
	"fmt"
)

func rightFactoriel(n int) int{
	if n < 2 {
		return 1
	}
	return n * rightFactoriel(n - 1)
}

func leftFactoriel(n int) int {
	var i, sum = 0, 0
	for i < n {
		sum += rightFactoriel(i)
		i += 1
	}
	return sum
}

func gcd(a int, b int) int{
	var min = a
	if a > b{
		min = b
	}
	var i = 1
	for i < min {
		if(a % (i + 1) == 0 && (b % (i + 1) == 0)){
			i += 1
		}else{
			break
		}
	}
	return i
}

func main() {
	var i, n = 1, 12
	for i <= n {
		var lf = leftFactoriel(i)
		var rf = rightFactoriel(i)
		fmt.Println(i, lf, rf, gcd(lf, rf))
		i += 1
	}
}