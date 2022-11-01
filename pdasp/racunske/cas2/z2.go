package main

import (
	"fmt"
	"math"
)

type Triangle struct {
	a float64
	b float64
	c float64
}

func area(t Triangle) float64 {
	perimeter := t.a + t.b + t.c
	s := perimeter / 2
	return math.Sqrt(s * (s - t.a) * (s - t.b) * (s - t.c))
}

func main() {
	t := Triangle{3, 4, 5}
	fmt.Println("Area of the triangle is: ", area(t))
	return
}
