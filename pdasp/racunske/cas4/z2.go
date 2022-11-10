package main

import (
	"fmt"
	"sync"
)

type Counter2 struct {
	mu              sync.Mutex
	value, maxValue int
	done            chan bool
}

func (c *Counter2) increment() {
	for {
		c.mu.Lock()
		if c.value == c.maxValue {
			c.done <- true
			return
		}
		c.value++
		c.mu.Unlock()
	}
}

func main() {
	const threads int = 4
	c := &Counter2{sync.Mutex{}, 0, 1e6, make(chan bool)}
	for i := 0; i < threads; i++ {
		go c.increment()
	}
	<-c.done
	fmt.Printf("Value of the counter is: %d\n", c.value)
}
