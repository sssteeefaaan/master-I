package main

import (
	"fmt"
	"time"
)

type Fork struct {
	pipe chan interface{}
}

type Philosopher struct {
	name           string
	left, right    *Fork
	aTimes, tTimes int
}

func (p *Philosopher) think() {
	fmt.Printf("%s: Thinking\n", p.name)
	p.tTimes++
	time.Sleep(time.Microsecond)
}

func (p *Philosopher) eat() {
	fmt.Printf("%s: Eating!\n", p.name)
	p.aTimes++
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
			return
		}
	}
}

func main() {
	size := 5
	forks := []*Fork{
		&Fork{make(chan interface{}, 1)},
		&Fork{make(chan interface{}, 1)},
		&Fork{make(chan interface{}, 1)},
		&Fork{make(chan interface{}, 1)},
		&Fork{make(chan interface{}, 1)}}
	philosophers := []*Philosopher{
		&Philosopher{"Aristotle", nil, nil, 0, 0},
		&Philosopher{"Karl Marks", nil, nil, 0, 0},
		&Philosopher{"Immanuel Kant", nil, nil, 0, 0},
		&Philosopher{"Plato", nil, nil, 0, 0},
		&Philosopher{"René Descartes", nil, nil, 0, 0}}
	for i := 0; i < size; i++ {
		philosophers[i].left = forks[i]
		philosophers[i].right = forks[(i+1)%size]
	}

	done := make(chan interface{}, 1)
	for _, p := range philosophers {
		go p.exist(done)
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
		fmt.Printf("%s: ate(%d) thought(%d)\n", p.name, p.aTimes, p.tTimes)
	}
}
