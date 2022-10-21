package main

import (
	"fmt"
)

func isPrime(n int) bool {
	if n < 4 {
		return true
	}

	if n%2 == 0 || n%3 == 0 {
		return false
	}

	for i := 5; i*i <= n; i += 6 {
		if n%i == 0 || n%(i+2) == 0 {
			return false
		}
	}
	return true
}

func main() {

	const n int = 25
	var i int = 2

	for numb := 0; numb < n; i++ {
		if isPrime((i)) {
			fmt.Printf("%d ", i)
			numb++
		}
	}

	fmt.Println()
}
