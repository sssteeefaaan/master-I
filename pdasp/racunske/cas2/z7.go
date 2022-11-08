package main

import (
	"fmt"
	"math/rand"
)

type NodeS struct {
	next *NodeS
	data interface{}
}

type StackS struct {
	elements []interface{}
	head     int
}

func topS(s *StackS) (interface{}, bool) {
	if !isEmptyS(s) {
		return s.elements[s.head], true
	}
	return -1, false
}

func popS(s *StackS) (interface{}, bool) {
	ret := -1
	if !isEmptyS(s) {
		ret := s.elements[s.head]
		s.head--
		return ret, true
	}
	return ret, false
}

func pushS(s *StackS, data interface{}) {
	s.head++
	s.elements[s.head] = data
}

func isEmptyS(s *StackS) bool {
	return s.head < 0
}

func main() {
	c := &StackS{head: -1, elements: make([]interface{}, 100)}

	for i := 0; i < 5; i++ {
		val := rand.Int()%100 + 2
		fmt.Printf("%d ", val)
		pushS(c, val)
	}

	fmt.Println("\nChained stack pop: ")
	for i := 0; i < 5; i++ {
		fmt.Println(popS(c))
	}
	fmt.Println("Is empty chained:", isEmptyS(c))

	return
}
