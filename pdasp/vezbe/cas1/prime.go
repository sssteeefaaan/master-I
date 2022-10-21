package main

import (
	"fmt"
)

func main() {

	var numb, i, n = 0, 2, 25
	var j = 0
	var flag = true

	for numb < n {
		j = 2
		flag = true
		for flag && j * j <= i {
			flag = i % j != 0
			j += 1
		}
		if flag{
			fmt.Printf("%d ", i)
			numb += 1
		}
		i += 1
	}
	fmt.Println()
}