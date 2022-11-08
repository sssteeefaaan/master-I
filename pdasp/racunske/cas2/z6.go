package main

import (
	"fmt"
	"math/rand"
)

type Node struct {
	next *Node
	data interface{}
}

type Stack struct {
	head *Node
}

func top(s *Stack) (interface{}, bool) {
	if !isEmpty(s) {
		return s.head.data, true
	}
	return -1, false
}

func pop(s *Stack) (interface{}, bool) {
	ret := -1
	if !isEmpty(s) {
		ret := s.head.data
		s.head = s.head.next
		return ret, true
	}
	return ret, false
}

func push(s *Stack, data int) {
	s.head = &Node{next: s.head, data: data}
}

func isEmpty(s *Stack) bool {
	return s.head == nil
}

func main() {
	c := &Stack{head: nil}

	for i := 0; i < 5; i++ {
		val := rand.Int()%100 + 2
		fmt.Printf("%d ", val)
		push(c, val)
	}

	fmt.Println("\nChained stack pop: ")
	for i := 0; i < 5; i++ {
		fmt.Println(pop(c))
	}
	fmt.Println("Is empty chained:", isEmpty(c))

	return
}
