package main

import (
	"fmt"
	"sync"
)

type Counter0 struct {
	inc, done sync.Mutex
	value     int
	maxValue  int
}

func (c *Counter0) increment() {
	c.inc.Lock()
	c.value++
	if c.value == c.maxValue {
		c.done.Unlock()
	} else {
		c.inc.Unlock()
	}
}

func (c *Counter0) count() {
	for {
		c.increment()
	}
}

func main() {
	const threads int = 4
	counter := &Counter0{sync.Mutex{}, sync.Mutex{}, 0, 100}
	counter.done.Lock()
	for i := 0; i < threads; i++ {
		go counter.count()
	}
	counter.done.Lock()
	fmt.Printf("Value of the counter is: %d\n", counter.value)
}
