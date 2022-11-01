package main

import "fmt"

type node struct {
	next *node
	data int
}

type stackChained struct {
	head *node
}

type stackSeq struct {
	elements []int
	head     int
}

func topC(s stackChained) (int, bool) {
	if !isEmptyC(s) {
		return s.head.data, true
	}
	return -1, false
}

func popC(s stackChained) (int, bool) {
	ret := -1
	if !isEmptyC(s) {
		ret = s.head.data
		s.head = s.head.next
		return ret, true
	}
	return ret, false
}

func pushC(s stackChained, data int) {
	s.head = &node{s.head, data}
}

func isEmptyC(s stackChained) bool {
	return s.head == nil
}

func topS(s stackSeq) (int, bool) {
	if !isEmptyS(s) {
		return s.elements[s.head], true
	}
	return -1, false
}

func popS(s stackSeq) (int, bool) {
	ret := -1
	if !isEmptyS(s) {
		ret = s.elements[s.head]
		s.head--
		return ret, true
	}
	return ret, false
}

func pushS(s stackSeq, data int) {
	s.head++
	s.elements[s.head] = data
}

func isEmptyS(s stackSeq) bool {
	return s.head < 0
}

func main() {
	c := stackChained{nil}
	s := stackSeq{make([]int, 100), -1}

	for i := 0; i < 5; i++ {
		fmt.Printf("%d ", i)
		pushC(c, i)
		pushS(s, i)
	}

	fmt.Println("\nChained stack pop: ")
	for i := 0; i < 5; i++ {
		fmt.Println(popC(c))
	}
	fmt.Println("Is empty chained:", isEmptyC(c))

	fmt.Println("Sequenced stack pop: ")
	for i := 0; i < 5; i++ {
		fmt.Println(popS(s))
	}
	fmt.Println("Is empty sequenced:", isEmptyS(s))

	return
}
