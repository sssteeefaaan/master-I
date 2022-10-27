package main

import (
	"fmt"
	"strings"
)

func main() {
	var s string = ""
	var a, b int = 0, 0

	fmt.Printf("Input the first operand: ")
	fmt.Scanf("%d", &a)
	fmt.Printf("Input the second operand: ")
	fmt.Scanf("%d", &b)
	fmt.Printf("Input the operation: ")
	fmt.Scanf("%s", &s)

	s = strings.ToUpper(s)

	switch s {
	case "PLUS":
		fmt.Printf("%d + %d = %d\n", a, b, a+b)
	case "MINUS":
		fmt.Printf("%d - %d = %d\n", a, b, a-b)
	case "MULTIPLY":
		fmt.Printf("%d * %d = %d\n", a, b, a*b)
	case "DIVIDE":
		if b == 0 {
			fmt.Printf("Cannot divide by zero!\n")
		} else {
			fmt.Printf("%d / %d = %d\n", a, b, a/b)
		}
	case "REMAINDER":
		fmt.Printf("%d % %d = %d\n", a, b, a%b)
	default:
		fmt.Printf("Error\n")
	}
}
