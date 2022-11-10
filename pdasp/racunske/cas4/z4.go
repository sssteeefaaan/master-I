package main

import (
	"fmt"
	"sync"
)

type Fib struct {
	i     uint
	value uint64
}

type Cache struct {
	data map[uint]uint64
	lock sync.Mutex
}

func Fibonacci(n uint, ch chan *Fib, cache *Cache) {
	cache.lock.Lock()
	val, ok := cache.data[n]
	cache.lock.Unlock()
	if ok {
		ch <- &Fib{n, val}
		return
	}

	if n < 2 {
		cache.lock.Lock()
		cache.data[n] = 1
		cache.lock.Unlock()
		ch <- &Fib{n, 1}
		return
	}

	ch1, ch2 := make(chan *Fib, 1), make(chan *Fib, 1)

	defer close(ch1)
	defer close(ch2)

	go Fibonacci(n-1, ch1, cache)
	go Fibonacci(n-2, ch2, cache)

	val = (<-ch1).value + (<-ch2).value

	cache.lock.Lock()
	cache.data[n] = val
	cache.lock.Unlock()

	ch <- &Fib{n, val}
}

func main() {
	n, fibCh, cache := uint(1e3), make(chan *Fib), Cache{make(map[uint]uint64), sync.Mutex{}}
	defer close(fibCh)

	for i := uint(0); i < n; i++ {
		go Fibonacci(i, fibCh, &cache)
	}

	count, doneCh := uint(0), make(chan bool, 1)
	defer close(doneCh)
loop:
	for {
		select {
		case val := <-fibCh:
			fmt.Printf("Fibonacci(%d): %d\n", val.i, val.value)
			count++
			if count == n {
				doneCh <- true
			}
		case <-doneCh:
			break loop
		default:
		}
	}

	fmt.Printf("Done\n")
}
