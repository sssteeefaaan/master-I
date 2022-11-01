package main

import (
	"fmt"
	"math"
)

type Order int8

const (
	ascending Order = iota
	descending
)

func merge(a1 []float64, a2 []float64) []float64 {
	n, m := len(a1), len(a2)
	i, j, k := 0, 0, 0
	ret := make([]float64, n+m)
	for i < n || j < m {
		if i < n && j < m {
			if a1[i] < a2[j] {
				ret[k] = a1[i]
				i++
			} else {
				ret[k] = a2[j]
				j++
			}
		} else if i < n {
			ret[k] = a1[i]
			i++
		} else {
			ret[k] = a2[j]
			j++
		}
		k++
	}
	return ret
}

func mergeSort(array []float64) []float64 {
	size := len(array)
	if size <= 1 {
		return array
	}
	half := int(math.Ceil(float64(size) / 2.0))
	a1 := mergeSort(array[:half])
	a2 := mergeSort(array[half:])
	return merge(a1, a2)
}
func swap(array []float64, i int, j int) {
	temp := array[i]
	array[i] = array[j]
	array[j] = temp
}

func bubbleSort(array []float64) []float64 {
	n := len(array)
	ret := copyArray(array)
	for {
		test := false
		for i := 0; i < n-1; i++ {
			if ret[i] > ret[i+1] {
				swap(ret, i, i+1)
				test = true
			}
		}
		if !test {
			break
		}
	}
	return ret
}

func copyArray(array []float64) []float64 {
	ret := make([]float64, len(array))
	for i := 0; i < len(array); i++ {
		ret[i] = array[i]
	}
	return ret
}

func selectionSort(array []float64) []float64 {
	n := len(array)
	ret := copyArray(array)
	for i := 0; i < n; i++ {
		imin := i
		for j := i + 1; j < n; j++ {
			if ret[j] < ret[imin] {
				imin = j
			}
		}
		swap(ret, i, imin)
	}
	return ret
}

func partition(array []float64, low int, high int) int {
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

func quick(array []float64, low int, high int) {
	if low < high {
		mid := partition(array, low, high)
		quick(array, low, mid-1)
		quick(array, mid+1, high)
	}
}

func quickSort(array []float64) []float64 {
	ret := copyArray(array)
	quick(ret, 0, len(ret)-1)
	return ret
}

func main() {
	k := []float64{1.1, 2.2, 0.3, -4.3, 0.4, 5.5, 10.12, -10.2}
	fmt.Println("Unsorted array", k)
	fmt.Println("Merge-sorted array", mergeSort(k))
	fmt.Println("****************************************")
	fmt.Println("Unsorted array", k)
	fmt.Println("Selection-sorted array", selectionSort(k))
	fmt.Println("****************************************")
	fmt.Println("Unsorted array", k)
	fmt.Println("Bubble-sorted array", bubbleSort(k))
	fmt.Println("****************************************")
	fmt.Println("Unsorted array", k)
	fmt.Println("Quick-sorted array", quickSort(k))
	fmt.Println("****************************************")
	fmt.Println("Unsorted array", k)
	return
}
