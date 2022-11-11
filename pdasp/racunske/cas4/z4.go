package main

import (
	"fmt"
	"os"
	"strconv"
	"sync"
	"time"
)

type Fib struct {
	i     uint
	value uint64
}

type Cache struct {
	data map[uint]uint64
	lock sync.Mutex
}

func Fibonacci(n uint, ch chan<- *Fib, cache *Cache) {
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

func FibonacciN(n uint, out *os.File, runTime time.Duration) {
	out.WriteString(fmt.Sprintf("Fibonacci %d\n\n", n))

	fibCh, cache := make(chan *Fib), Cache{make(map[uint]uint64), sync.Mutex{}}
	//defer close(fibCh)

	for i := uint(0); i < n; i++ {
		go Fibonacci(i, fibCh, &cache)
	}

	out.WriteString("Calculations started!\n\n")

	count, doneCh := uint(0), make(chan bool, 1)
	defer close(doneCh)

	// failsafe mehanizam => iife koja zatvara kanal nakon X sekundi
	go func(c chan *Fib, runTime time.Duration) {
		time.Sleep(runTime)
		doneCh <- false
	}(fibCh, runTime)

loop:
	for {
		select {
		case val, ok := <-fibCh:
			if ok {
				out.WriteString(fmt.Sprintf("Fibonacci(%d): %d\n", val.i, val.value))
				count++
				if count == n {
					doneCh <- true
				}
			} else {
				break loop
			}
		case val := <-doneCh:
			if val {
				out.WriteString("\nCalculations complete!\n")
			} else {
				out.WriteString("\nFailsafe!\n")
			}
			break loop
		}
	}

	out.WriteString("\nThread return\n")
}

func main() {
	n := uint(20)
	filename := "fib.txt"

	switch len(os.Args) {
	case 3:
		filename = os.Args[2]
		fallthrough
	case 2:
		val, err := strconv.ParseUint(os.Args[1], 10, 64)
		if err != nil {
			fmt.Printf("Cannot use '%s' as an uint value. Using default n = %d\n", os.Args[1], n)
		} else {
			n = uint(val)
		}
	}

	f, err := os.OpenFile(filename, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, 0777)
	if err != nil {
		fmt.Printf("Couldn't open the file '%s'!\n", filename)
		os.Exit(-1)
	}

	FibonacciN(n, f, time.Second)
}
