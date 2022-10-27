package main

import (
	"fmt"
)

func fibonacci(n int) int {
	if n < 1 {
		return 0
	}
	if n < 2 {
		return 1
	}
	return fibonacci(n-1) + fibonacci(n-2)
}

func main() {
	const n = 30
	for i := 0; i <= n; i++ {
		fmt.Println(i, fibonacci(i))
	}
}
