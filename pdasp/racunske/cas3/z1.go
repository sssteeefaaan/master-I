package main

import (
	"fmt"
	"math"
)

type Triangle struct {
	a, b, c float64
}

type Rectangle struct {
	a, b float64
}

type Shape interface {
	Area() float64
	Perimeter() float64
}

func (r *Rectangle) Area() float64 {
	return r.a * r.b
}

func (r *Rectangle) Perimeter() float64 {
	return 2.0 * (r.a + r.b)
}

func (t *Triangle) Area() float64 {
	s := t.Perimeter() / 2.0
	return float64(math.Sqrt(s * (s - t.a) * (s - t.b) * (s - t.c)))
}

func (t *Triangle) Perimeter() float64 {
	return t.a + t.b + t.c
}

func Calculate(s Shape) {
	fmt.Printf("Area: %.2f\n", s.Area())
	fmt.Printf("Perimeter: %.2f\n", s.Perimeter())
}

func main() {
	t, r := Triangle{2, 3, 4}, Rectangle{4, 5}
	Calculate(&t)
	Calculate(&r)
}
