package main

import (
	"fmt"
)

func isPrime(n int) bool {
	if n < 2 {
		return false
	}

	if n < 4 {
		return true
	}

	for i := 2; i*i <= n; i++ {
		if n%i == 0 {
			return false
		}
	}

	return true
}

func main() {

	const n int = 25
	var i int = 2

	for numb := 0; numb < n; i++ {
		if isPrime(i) {
			fmt.Printf("%d ", i)
			numb++
		}
	}
	fmt.Println()
}
