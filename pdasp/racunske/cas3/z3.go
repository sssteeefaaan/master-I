package main

import (
	"fmt"
	"time"
)

type Team struct {
	name, shortName string
	yearCreated     time.Time
}

type Score struct {
	homeTeam  uint
	guestTeam uint
}

type Match struct {
	id                  uint64
	homeTeam, guestTeam *Team
	time                time.Time
	score               Score
}

type Championship struct {
	teams   map[string]Team
	matches map[uint64]Match
}

func start() *Championship {
	var n uint
	for {
		fmt.Printf("Input the number of teams in a championship: ")
		_, err := fmt.Scanf("%u", n)
		if err != nil {
			fmt.Println("Inputed value '{}' isn't a whole number greater than 0! Please try again!")
		} else {
			break
		}
	}
	ret := &Championship{make(map[string]Team, n), make(map[uint64]Match, n*(n-1)*2)}
	for i := uint(0); i < n; i++ {
		t := inputTeam()
		if ret.teams.get
	}
}

func main() {

}
