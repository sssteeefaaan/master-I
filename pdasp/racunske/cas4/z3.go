package main

import "fmt"

func f() {
	defer fmt.Printf("?\n")
	defer fmt.Printf("doing")
	defer fmt.Printf("you ")
	defer fmt.Printf("are ")
	defer fmt.Printf(", ")
	defer fmt.Printf("Hello")
}

func main() {

	f()
}
