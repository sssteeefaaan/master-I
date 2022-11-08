package main

import "fmt"

type NodeT struct {
	left  *NodeT
	right *NodeT
	data  int
}

type Tree struct {
	root *NodeT
}

func createBST(sortedArray []int) *Tree {
	return &Tree{root: divideArray(sortedArray)}
}

func divideArray(sortedArray []int) *NodeT {
	size := len(sortedArray)
	middle := size / 2
	if size == 0 {
		return nil
	}
	return &NodeT{divideArray(sortedArray[0:middle]), divideArray(sortedArray[middle+1 : size]), sortedArray[middle]}
}

func preorder(t *Tree) {
	fmt.Printf("Preorder: ")
	if t != nil {
		preorder_rec(t.root)
	}
	fmt.Println()
}

func preorder_rec(n *NodeT) {
	if n != nil {
		fmt.Printf("%d ", n.data)
		preorder_rec(n.left)
		preorder_rec(n.right)
	}
}

func postorder(t *Tree) {
	fmt.Printf("Postorder: ")
	if t != nil {
		postorder_rec(t.root)
	}
	fmt.Println()
}

func postorder_rec(n *NodeT) {
	if n != nil {
		postorder_rec(n.left)
		postorder_rec(n.right)
		fmt.Printf("%d ", n.data)
	}
}

func inorder(t *Tree) {
	fmt.Printf("Inorder: ")
	if t != nil {
		inorder_rec(t.root)
	}
	fmt.Println()
}

func inorder_rec(n *NodeT) {
	if n != nil {
		inorder_rec(n.left)
		fmt.Printf("%d ", n.data)
		inorder_rec(n.right)
	}
}

func main() {
	n := 20
	a := make([]int, n)
	for i := 0; i < n; i++ {
		a[i] = i
	}
	t := createBST(a)
	preorder(t)
	inorder(t)
	postorder(t)
}
