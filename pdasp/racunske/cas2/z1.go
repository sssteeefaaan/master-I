package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func main() {
	var filePath string
	if len(os.Args) > 1 {
		filePath = os.Args[1]
	} else {
		filePath = "./Proba1"
	}
	file, err := os.OpenFile(filePath, os.O_RDWR|os.O_CREATE|os.O_APPEND, 0777)
	if err != nil {
		log.Printf("Couldn't open the file %s\n", filePath)
	}
	w := bufio.NewWriter(file)

	for {
		uReader := bufio.NewReader(os.Stdin)
		fmt.Print("Enter text to write or 'break' to exit: ")
		input, err := uReader.ReadString('\n')
		if err != nil {
			log.Printf("Couldn't read user input!")
			continue
		}
		if input == "break" {
			break
		}
		_, err = w.WriteString(input)
		if err != nil || w.Flush() != nil {
			log.Fatal("Couldn't write to the file ", filePath)
		}
	}

	defer file.Close()
}
