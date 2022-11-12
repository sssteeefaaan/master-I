package main

import (
	"fmt"
	"sync"
)

var names []string = []string{
	"Aristotle",
	"Karl Marks",
	"Immanuel Kant",
	"Plato",
	"René Descartes",
	"Socrates",
	"Friedrich Nietzsche",
	"John Stuart Mill",
	"Pythagoras",
	"Epictetus",
}

type Semaphore struct {
	size, counter uint
	signal        chan interface{}
	guard         sync.Mutex
}

func (s *Semaphore) Init(size uint) {
	s.size = size
	s.counter = 0
	s.signal = make(chan interface{}, s.size)
	for i := uint(0); i < s.size; i++ {
		s.signal <- true
	}
}

func (s *Semaphore) Aquire() {
	for {
		select {
		case <-s.signal:
			s.guard.Lock()
			if s.counter < s.size {
				s.counter++
				s.guard.Unlock()
				return
			}
			s.guard.Unlock()
		}
	}
}

func (s *Semaphore) Release() {
	s.guard.Lock()
	if s.counter > 0 {
		s.counter--
		s.guard.Unlock()
		s.signal <- true
	} else {
		fmt.Printf("Releasing failed!\n")
		s.guard.Unlock()
	}
}

type Fork struct {
	mutex sync.Mutex
}

type Philosopher struct {
	name                string
	leftFork, rightFork *Fork
	aTimes, tTimes      int
	semaphore           *Semaphore
}

func (p *Philosopher) Think() {
	//fmt.Printf("%s: Thinking\n", p.name)
	p.tTimes++
	//time.Sleep(time.Microsecond)
}

func (p *Philosopher) Eat() {
	//fmt.Printf("%s: Eating!\n", p.name)
	p.aTimes++
	//time.Sleep(time.Microsecond)
}

func (p *Philosopher) Statistic() float64 {
	return float64(p.aTimes) / float64(p.tTimes+p.aTimes)
}

func (p *Philosopher) Exist(done chan interface{}) {
	for {
		select {
		case <-done:
			return
		default:
			p.Think()

			p.semaphore.Aquire()
			p.leftFork.mutex.Lock()
			p.rightFork.mutex.Lock()

			p.Eat()

			p.rightFork.mutex.Unlock()
			p.leftFork.mutex.Unlock()
			p.semaphore.Release()
		}
	}
}

func main() {
	size := 10
	room := make(chan interface{}, size-1)
	semaphore := Semaphore{}
	semaphore.Init(uint(size - 1))

	forks, philosophers := make([]*Fork, size), make([]*Philosopher, size)

	for i := 0; i < size; i++ {
		forks[i] = &Fork{sync.Mutex{}}
		if i > 0 {
			room <- true
			philosophers[i] = &Philosopher{names[i], forks[i-1], forks[i], 0, 0, &semaphore}
		}
	}
	philosophers[0] = &Philosopher{names[0], forks[size-1], forks[0], 0, 0, &semaphore}

	done := make(chan interface{}, size)
	for _, p := range philosophers {
		go p.Exist(done)
	}

	var exit int
	fmt.Scanf("%d", &exit)
	for i := 0; i < size; i++ {
		done <- true
	}

	for _, p := range philosophers {
		fmt.Printf("%s: ate(%d) thought(%d) => %.3f\n", p.name, p.aTimes, p.tTimes, p.Statistic())
	}
}
