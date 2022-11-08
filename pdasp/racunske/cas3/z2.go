package main

import (
	"fmt"
)

func minimum_par(array []int64, c chan int64) {
	var min int64 = int64(uint(0) >> 1)
	for _, el := range array {
		if min >= el {
			min = el
		}
	}
	c <- min
}

func minimum(array []int64, div int) int64 {
	size := len(array)
	part := size/div + 1
	c := make(chan int64, div)
	for i := 0; i < size-part; i += part {
		go minimum_par(array[i:i+part], c)
	}
	go minimum_par(array[size-part:size], c)
	ret := int64(uint(0) >> 1)
	for i := 0; i < div; i++ {
		val := <-c
		if ret > val {
			ret = val
		}
	}
	return ret
}

func main() {
	size := int64(1e8)
	a := make([]int64, size)
	for i := int64(0); i < size; i++ {
		if i == 12345 {
			a[i] = -i
		}
	}
	fmt.Printf("Minimum is %d.\n", minimum(a, 100))
}
