package main

import (
	"fmt"
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

type Fork struct {
	pipe chan interface{}
}

type Philosopher struct {
	name                string
	leftFork, rightFork *Fork
	room                chan interface{}
	aTimes, tTimes      int
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
		case <-p.room:
		room:
			for {
				select {
				case <-p.leftFork.pipe:
				left_fork:
					for {
						select {
						case <-p.rightFork.pipe:
							p.Eat()
							break room
						default:
							break left_fork
						}
					}
					p.leftFork.pipe <- true
					p.Think()
				case <-p.rightFork.pipe:
				right_fork:
					for {
						select {
						case <-p.leftFork.pipe:
							p.Eat()
							break room
						default:
							break right_fork
						}
					}
					p.rightFork.pipe <- true
					p.Think()
				default:
					p.Think()
				}
			}
			p.leftFork.pipe <- true
			p.rightFork.pipe <- true
			p.room <- true
			//p.Think()
		case <-done:
			return
		default:
			p.Think()
		}
	}
}

func main() {
	size := 10
	room := make(chan interface{}, size-1)

	forks, philosophers := make([]*Fork, size), make([]*Philosopher, size)

	for i := 0; i < size; i++ {
		forks[i] = &Fork{make(chan interface{}, 1)}
		if i > 0 {
			room <- true
			philosophers[i] = &Philosopher{names[i], forks[i-1], forks[i], room, 0, 0}
		}
	}
	philosophers[0] = &Philosopher{names[0], forks[size-1], forks[0], room, 0, 0}

	done := make(chan interface{}, size)
	for _, p := range philosophers {
		go p.Exist(done)
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
		fmt.Printf("%s: ate(%d) thought(%d) => %.3f\n", p.name, p.aTimes, p.tTimes, p.Statistic())
	}
}
