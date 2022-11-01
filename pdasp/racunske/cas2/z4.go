package main

import (
	"fmt"
	"regexp"
	"strconv"
)

var (
	indexPattern, _ = regexp.Compile(`^[1-9][0-9]*$`)
	namePattern, _  = regexp.Compile(`^[A-Z][a-z]+$`)
	yearPattern, _  = regexp.Compile(`^[1-9][0-9]{3}$`)
	gpaPattern, _   = regexp.Compile(`^(0|[1-9][0-9]*)(\.[0-9]+)?$`)
)

type Student struct {
	index        string
	firstName    string
	lastName     string
	yearEnrolled int64
	gpa          float64
}

var students map[string]Student

func newStudent() {
	fmt.Printf("Insert student info\n")
	var input string
	var ret Student
	for {
		fmt.Printf("Index number: ")
		fmt.Scanf("%s", &input)
		if indexPattern.MatchString(input) {
			_, ok := students[input]
			if ok {
				fmt.Println("Student with that index already exists!")
				continue
			}
			ret.index = input
			break
		}
		fmt.Println("Wrong input, please try again!")
	}
	for {
		fmt.Printf("First name: ")
		fmt.Scanf("%s", &input)
		if namePattern.MatchString(input) {
			ret.firstName = input
			break
		}
		fmt.Println("Wrong input, please try again!")
	}

	for {
		fmt.Printf("Last name: ")
		fmt.Scanf("%s", &input)
		if namePattern.MatchString(input) {
			ret.lastName = input
			break
		}
		fmt.Println("Wrong input, please try again!")
	}
	for {
		fmt.Printf("Year of enrollment: ")
		fmt.Scanf("%s", &input)
		if yearPattern.MatchString(input) {
			ret.yearEnrolled, _ = strconv.ParseInt(input, 10, 64)
			break
		}
		fmt.Println("Wrong input, please try again!")
	}
	for {
		fmt.Printf("GPA: ")
		fmt.Scanf("%s", &input)
		if gpaPattern.MatchString(input) {
			ret.gpa, _ = strconv.ParseFloat(input, 64)
			break
		}
		fmt.Println("Wrong input, please try again!")
	}
	students[ret.index] = ret
}

func showStudentDetails(s Student) {
	fmt.Printf("\tIndex: %s\n", s.index)
	fmt.Printf("\tFirst name: %s\n", s.firstName)
	fmt.Printf("\tLast name: %s\n", s.lastName)
	fmt.Printf("\tYear of enrolment: %d\n", s.yearEnrolled)
	fmt.Printf("\tGPA: %.2f\n", s.gpa)
}

func deleteStudent() {
	fmt.Print("Input the index number of a student you wish to delete: ")
	var input string
	fmt.Scanf("%s", &input)
	s, ok := students[input]
	if !ok {
		fmt.Printf("Student with the index number [%s] isn't recorded!\n", input)
	} else {
		delete(students, input)
		fmt.Printf("Deleted the student [%s]!\n", s.index)
		showStudentDetails(s)
	}
}

func listAllStudents() {
	for _, v := range students {
		showStudentDetails(v)
	}
}

func findStudent() {
	fmt.Printf("Insert the index number of a student you wish to find: ")
	var input string
	fmt.Scanf("%s", &input)
	if indexPattern.MatchString(input) {
		s, ok := students[input]
		if ok {
			showStudentDetails(s)
		} else {
			fmt.Printf("Student with the index number (%s) isn't recorded!\n", input)
		}
	} else {
		fmt.Printf("Wrong index input!")
	}
}

func mainMenu() {
	fmt.Println("Welcome to the main menu!")
	exit := false
	for !exit {
		fmt.Println("Main menu:")
		fmt.Println("\t1 - Add a new student")
		fmt.Println("\t2 - Delete a student")
		fmt.Println("\t3 - List all students")
		fmt.Println("\t4 - Find a specific student")
		fmt.Println("\t5 - Exit")
		var input string
		fmt.Scanf("%s", &input)
		m, _ := regexp.MatchString("[1-5]", input)
		if m {
			val, _ := strconv.ParseInt(input, 10, 16)
			switch val {
			case (1):
				newStudent()
			case (2):
				deleteStudent()
			case (3):
				listAllStudents()
			case (4):
				findStudent()
			case (5):
				exit = true
				break
			default:
				fmt.Println("Wrong input! Please try again!")
				continue
			}
		} else {
			fmt.Println("Wrong input! Please try again!")
		}
	}
	fmt.Println("Exiting...")
}

func main() {
	students = make(map[string]Student)
	mainMenu()
}
