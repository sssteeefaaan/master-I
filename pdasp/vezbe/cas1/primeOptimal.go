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
		if i == 2 || i == 3{
			fmt.Printf("%d ", i)
			numb += 1
		}else{
			flag = i % 2 != 0 && i % 3 != 0
			if flag {
				var t = 6 * j
				for flag && (t + 1) * (t + 1) <= i {
					flag = i % (t + 1) != 0
					flag = i % (t - 1) != 0
					j += 1
				}
			}
			if flag{
				fmt.Printf("%d ", i)
				numb += 1
			}
		}
		i += 1
	}
	fmt.Println()
}