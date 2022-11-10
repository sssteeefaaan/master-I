package main

import (
	"fmt"
	"sync"
)

type Counter1 struct {
	inc      sync.Mutex
	value    int
	maxValue int
	wg       sync.WaitGroup
}

func (c *Counter1) count() {
	defer c.inc.Unlock()
	for {
		c.inc.Lock()
		if c.value == c.maxValue {
			c.wg.Done()
			return
		}
		c.value++
		c.inc.Unlock()
	}
}

func main() {
	const threads int = 4
	counter := &Counter1{sync.Mutex{}, 0, 100, sync.WaitGroup{}}
	counter.wg.Add(threads)
	for i := 0; i < threads; i++ {
		go counter.count()
	}
	counter.wg.Wait()
	fmt.Printf("Value of the counter is: %d\n", counter.value)
}
