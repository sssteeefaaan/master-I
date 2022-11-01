package main

import (
	"fmt"
	"math/rand"
	"time"
)

func swap(array []int64, i int, j int) {
	temp := array[i]
	array[i] = array[j]
	array[j] = temp

}

func partition(array []int64, low int, high int) int {
	pivot := array[high]
	i := low - 1
	for j := low; j < high; j++ {
		if pivot > array[j] {
			i++
			swap(array, i, j)
		}
	}
	i++
	swap(array, i, high)
	return i
}

func quickSort(array []int64, low int, high int) {
	if low < high {
		part := partition(array, low, high)
		quickSort(array, low, part-1)
		quickSort(array, part+1, high)
	}
}

func bubbleSort(array []int64) {
	n := len(array)
	for {
		sorted := true
		for i := 0; i < n-1; i++ {
			if array[i] > array[i+1] {
				swap(array, i, i+1)
				sorted = false
			}
		}
		if sorted {
			break
		}
	}
}

func genRandArray(n int64) []int64 {
	ret := make([]int64, n)
	for i := int64(0); i < n; i++ {
		ret[i] = rand.Int63n(n)
	}
	return ret
}

func copyArray(array []int64) []int64 {
	n := len(array)
	ret := make([]int64, n)
	for i := 0; i < n; i++ {
		ret[i] = array[i]
	}
	return ret
}

func main() {
	arrayq := genRandArray(1e2)
	arrayb := copyArray(arrayq)

	start := time.Now()
	quickSort(arrayq, 0, len(arrayq)-1)
	dt := int(time.Since(start))
	fmt.Printf("Quicksort time: %d.\n", dt)

	start = time.Now()
	bubbleSort(arrayb)
	dt = int(time.Since(start))
	fmt.Printf("Bubblesort time: %d.\n", dt)

	return
}
