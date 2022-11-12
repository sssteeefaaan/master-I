package main

import (
	"fmt"
	"sync"
	"time"
)

var t int = 0

type Semaphore struct {
	size, counter uint
	signal        chan interface{}
	guard         sync.Mutex
}

func (s *Semaphore) init(size uint) {
	s.size = size
	s.counter = 0
	s.signal = make(chan interface{}, s.size)
	for i := uint(0); i < s.size; i++ {
		s.signal <- true
	}
}

func (s *Semaphore) aquire() {
	for {
		select {
		case <-s.signal:
			s.guard.Lock()
			s.counter++
			s.guard.Unlock()
			return
		}
	}
}

func (s *Semaphore) release() {
	s.guard.Lock()
	if s.counter > 0 {
		s.counter--
		s.guard.Unlock()
		s.signal <- true
	} else {
		s.guard.Unlock()
	}
}

type Fork struct {
	pipe  chan interface{}
	mutex sync.Mutex
}

type Philosopher struct {
	name           string
	left, right    *Fork
	aTimes, tTimes int
	semaphore      *Semaphore
}

func (p *Philosopher) think() {
	//fmt.Printf("%s: Thinking\n", p.name)
	p.tTimes++
	time.Sleep(time.Microsecond)
}

func (p *Philosopher) eat() {
	//fmt.Printf("%s: Eating!\n", p.name)
	p.aTimes++
}

func (p *Philosopher) use() float64 {
	return float64(p.aTimes) / float64(p.tTimes+p.aTimes)
}

func (p *Philosopher) existM(done chan interface{}) {
	for {
		select {
		case <-done:
			return
		default:
			p.think()
			p.semaphore.aquire()
			p.left.mutex.Lock()
			p.right.mutex.Lock()
			p.eat()
			p.right.mutex.Unlock()
			p.left.mutex.Unlock()
			p.semaphore.release()
		}
	}
}

func (p *Philosopher) exist(done chan interface{}) {
	ll, rl := false, false
	for {
		select {
		case <-p.left.pipe:
			ll = true
			go func() {
				p.think()
				if rl {
					p.eat()
					ll = false
					rl = false
					p.left.pipe <- true
				} else {
					ll = false
					p.left.pipe <- true
				}
			}()
		case <-p.right.pipe:
			rl = true
			go func() {
				p.think()
				if ll {
					p.eat()
					ll = false
					rl = false
					p.right.pipe <- true
				} else {
					rl = false
					p.right.pipe <- true
				}
			}()
		case <-done:
			fmt.Printf("%s: Done!\n", p.name)
			return
		default:
			p.think()
		}
	}
}

func main() {
	size := 5
	semaphore := Semaphore{}
	semaphore.init(uint(size - 1))

	forks, philosophers := make([]*Fork, size), make([]*Philosopher, size)
	names := []string{"Aristotle", "Karl Marks", "Immanuel Kant", "Plato", "René Descartes"}

	for i := 0; i < size; i++ {
		forks[i] = &Fork{make(chan interface{}, 1), sync.Mutex{}}
		if i > 0 {
			philosophers[i] = &Philosopher{names[i], forks[i-1], forks[i], 0, 0, &semaphore}
		}
	}
	philosophers[0] = &Philosopher{names[0], forks[size-1], forks[0], 0, 0, &semaphore}

	done := make(chan interface{}, 1)
	for _, p := range philosophers {
		go p.existM(done)
	}

	for _, f := range forks {
		f.pipe <- true
	}

	var exit int
	fmt.Scanf("%d", &exit)
	for i := 0; i < size; i++ {
		done <- true
	}

	for _, p := range philosophers {
		fmt.Printf("%s: ate(%d) thought(%d) => %.3f\n", p.name, p.aTimes, p.tTimes, p.use())
	}
}
