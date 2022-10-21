package main

import (
	"fmt"
)

func fibonacci(n int) int {
	if n < 1 {return 0}
	if n < 2 {return 1}
	return fibonacci(n - 1) + fibonacci(n - 2)
}

func main() {
	var i, n = 0, 50
	for i <= n {
		fmt.Println(i, fibonacci(i))
		i += 1
	}
}