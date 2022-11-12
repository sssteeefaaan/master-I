package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"io"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"text/tabwriter"
	"time"

	"golang.org/x/exp/constraints"
)

type Restrictions struct {
	ExactNumberOfGoals      int
	MaxNumberOfGoalsPerPlay int
	MinNumberOfGoalsPerPlay int
	MinNumberOfTeams        int
	MaxNumberOfTeams        int
	MaxNumbersOfPlays       int
}

type Team struct {
	Name, ShortName string
	YearCreated     int
	Goals, Points   int
}

type Score struct {
	HomeTeam  int
	GuestTeam int
}

type Match struct {
	Id                  uint64
	HomeTeam, GuestTeam *Team
	Time                time.Time
	HalfTime            *Score
}

type Championship struct {
	Name          string
	Teams         map[string]*Team
	Matches       []*Match
	GoalNumber    int
	DrawPoints    int
	WinningPoints int
	Restrictions  Restrictions
}

func createTeam() *Team {
	ret := &Team{}
	for {
		fmt.Printf("Input Team Name: ")
		uReader := bufio.NewReader(os.Stdin)
		s, err := uReader.ReadString('\n')
		s = strings.ReplaceAll(strings.ReplaceAll(s, "\r", ""), "\n", "")
		if err != nil || s == "" {
			fmt.Printf("Wrong value for the Team's Name (%s)! Please try again!\n", s)
		} else {
			ret.Name = s
			break
		}
	}
	for {
		fmt.Printf("Input team's short Name: ")
		_, err := fmt.Scanln(&ret.ShortName)
		if err != nil || ret.ShortName == "" {
			fmt.Printf("Wrong value for the Team's short Name (%s)! Please try again!\n", ret.ShortName)
		} else {
			break
		}
	}
	for {
		fmt.Printf("Input team's year of creation: ")
		var year int
		_, err := fmt.Scanf("%d", &year)
		if err != nil || year > time.Now().Year() || year < 1 {
			fmt.Printf("Wrong value for the Team's year of creation (%d)! Please try again!\n", year)
		} else {
			ret.YearCreated = year
			break
		}
	}
	return ret
}

func (ret *Championship) createChampionshipTeams() {
	var n int
	for {
		fmt.Printf("Input the number of Teams in a championship: ")
		_, err := fmt.Scanf("%d", &n)
		if err != nil {
			fmt.Printf("Inputed value '%d' cannot be used for the number of Teams!\n", n)
		} else if n < ret.Restrictions.MinNumberOfTeams {
			fmt.Printf("Minimum number of Teams is %d\n", ret.Restrictions.MinNumberOfTeams)
		} else if n > ret.Restrictions.MaxNumberOfTeams {
			fmt.Printf("Maximum number of Teams is %d\n", ret.Restrictions.MaxNumberOfTeams)
		} else {
			break
		}
	}
	for i := 0; i < n; i++ {
		fmt.Printf("Input the information about the Team number %d:\n", i+1)
		t := createTeam()
		_, ok := ret.Teams[t.Name]
		if ok {
			fmt.Printf("Team with the Name '%s' already exists!\n", t.Name)
			i--
			continue
		}
		ret.Teams[t.Name] = t
	}
}

func (t *Team) display() {
	fmt.Printf("\tName: %s\n", t.Name)
	fmt.Printf("\tShort Name: %s\n", t.ShortName)
	fmt.Printf("\tYear of creation: %d\n", t.YearCreated)
	fmt.Printf("\tPoints: %d\n", t.Points)
	fmt.Printf("\tGoals: %d\n", t.Goals)
}

func (c *Championship) display() {
	for _, t := range c.Teams {
		fmt.Println("=======================================")
		t.display()
	}
	fmt.Println("=======================================")
}

func max[T constraints.Ordered](a, b T) T {
	if a <= b {
		return b
	}
	return a
}

func min[T constraints.Ordered](a, b T) T {
	if a <= b {
		return a
	}
	return b
}

func sumOfN[T constraints.Integer](n T) T {
	return (n * (n + 1)) >> 1
}

func (c *Championship) generateRandomScore(ch chan *Score, gnChanel, pmChanel chan int) {
	var goals1, goals2 int
	goalNumber := <-gnChanel
	playedMatches := <-pmChanel
	minGoals := max(c.Restrictions.MinNumberOfGoalsPerPlay, (c.Restrictions.ExactNumberOfGoals-goalNumber)/(c.Restrictions.MaxNumbersOfPlays-playedMatches))
	maxGoals := min(minGoals, c.Restrictions.MaxNumberOfGoalsPerPlay) + 1
	goals1 = rand.Int() % maxGoals
	maxGoals -= goals1 + 1
	if minGoals < maxGoals {
		goals2 = rand.Int()%(maxGoals-minGoals) + minGoals
	} else {
		goals2 = maxGoals
	}
	ch <- &Score{goals1, goals2}
	gnChanel <- goalNumber + goals1 + goals2
	pmChanel <- playedMatches + 1
}

func (c *Championship) playRandom(t1, t2 *Team, randomScore *Score) {
	c.GoalNumber += randomScore.GuestTeam + randomScore.HomeTeam
	t1.Goals += randomScore.HomeTeam
	t2.Goals += randomScore.GuestTeam
	if randomScore.HomeTeam < randomScore.GuestTeam {
		t2.Points += int(c.WinningPoints)
	} else if randomScore.HomeTeam > randomScore.GuestTeam {
		t1.Points += int(c.WinningPoints)
	} else {
		t1.Points += int(c.DrawPoints)
		t2.Points += int(c.DrawPoints)
	}
	id := len(c.Matches)
	c.Matches = append(c.Matches, &Match{uint64(id), t1, t2, time.Now(), randomScore})
}

func keys[K comparable, J interface{}](m map[K]J) []K {
	ret := make([]K, len(m))
	i := 0
	for k := range m {
		ret[i] = k
		i++
	}
	return ret
}

func (c *Championship) createMatches() {
	keys := keys(c.Teams)
	ch := make(chan *Score, c.Restrictions.MaxNumbersOfPlays)
	gnChanel, pmChanel := make(chan int, 1), make(chan int, 1)
	gnChanel <- 0
	pmChanel <- 0
	for i := 0; i < c.Restrictions.MaxNumbersOfPlays; i++ {
		go c.generateRandomScore(ch, gnChanel, pmChanel)
	}
	for ind, key := range keys {
		for j := ind + 1; j < len(keys); j++ {
			c.playRandom(c.Teams[key], c.Teams[keys[j]], <-ch)
			c.playRandom(c.Teams[keys[j]], c.Teams[key], <-ch)
		}
	}
}

func trimEnd(s string) string {
	return strings.ReplaceAll(strings.ReplaceAll(s, "\r", ""), "\n", "")
}

func readTeamFromFile(rd *bufio.Reader) (*Team, error) {
	ret := &Team{}
	s, err := rd.ReadString('\n')
	if err != nil {
		fmt.Printf("Couldn't read Name of the Team!\n")
		return nil, err
	}
	ret.Name = trimEnd(s)
	s, err = rd.ReadString('\n')
	if err != nil {
		fmt.Printf("Couldn't read short Name of the Team ('%s')!\n", ret.Name)
		return nil, err
	}
	ret.ShortName = trimEnd(s)
	s, err = rd.ReadString('\n')
	if err != nil && err.Error() != io.EOF.Error() {
		fmt.Printf("Error: %s", err.Error())
		fmt.Printf("Couldn't read the year created of the Team ('%s', '%s')!\n", ret.Name, ret.ShortName)
		return nil, err
	}
	s = trimEnd(s)
	yearCreated, err := strconv.ParseInt(s, 10, 64)
	if err != nil || yearCreated > int64(time.Now().Year()) || yearCreated < 1 {
		fmt.Printf("Invalid year '%s'!\n", s)
		return nil, err
	}
	ret.YearCreated = int(yearCreated)
	return ret, nil
}

func (c *Championship) createChampionshipTeamsFromFile(filePath string) error {
	if c.Teams == nil {
		c.Teams = make(map[string]*Team)
	}

	f, err := os.Open(filePath)
	defer f.Close()

	if err != nil {
		fmt.Printf("Couldn't open the file '%s'!\n", filePath)
		return err
	}

	bytes, err := io.ReadAll(f)
	if err != nil {
		fmt.Printf("Couldn't read form file '%s'!\n", filePath)
		return err
	}

	err = json.Unmarshal(bytes, c)
	if err != nil {
		fmt.Printf("Couldn't unmarshal json '%s'! Please check the format!\n", filePath)
		return err
	}

	n := len(c.Teams)
	matches := sumOfN(n - 1)
	c.Restrictions.MaxNumbersOfPlays = matches << 1
	if c.Restrictions.MaxNumberOfGoalsPerPlay*c.Restrictions.MaxNumbersOfPlays < c.Restrictions.ExactNumberOfGoals {
		s := fmt.Sprintf("Cannot achive %d goals in a championship with %d teams that play %d mathces with the maximum of %d goals per play!\n",
			c.Restrictions.ExactNumberOfGoals,
			n,
			matches,
			c.Restrictions.MaxNumberOfGoalsPerPlay)
		fmt.Printf(s)
		return fmt.Errorf(s)
	} else if c.Restrictions.MinNumberOfGoalsPerPlay*c.Restrictions.MaxNumberOfGoalsPerPlay > c.Restrictions.ExactNumberOfGoals {
		s := fmt.Sprintf("Cannot achive %d goals in a championship with %d teams that play %d mathces with the minimum of %d goals per play!\n",
			c.Restrictions.ExactNumberOfGoals,
			n,
			matches,
			c.Restrictions.MinNumberOfGoalsPerPlay)
		fmt.Printf(s)
		return fmt.Errorf(s)
	}

	return nil
}

func (t *Team) saveTeam(f *os.File) error {
	s := fmt.Sprintf("\n\tName: %s\n\tShortname: %s\n\tYear of creation: %d\n", t.Name, t.ShortName, t.YearCreated)
	_, err := f.WriteString(s)
	if err != nil {
		fmt.Printf("Error writing team '%s'\n", t.Name)
		return err
	}
	return nil
}

func (c *Championship) saveTeams(filePath string) error {
	f, err := os.OpenFile(filePath, os.O_CREATE|os.O_WRONLY, 0777)
	defer f.Close()

	if err != nil {
		fmt.Printf("Error opening file '%s'!\n", filePath)
		return err
	}

	s := fmt.Sprintf("Teams for the championship '%s':\n", c.Name)
	_, err = f.WriteString(s)
	if err != nil {
		fmt.Printf("Couldn't write to file '%s'!\n", filePath)
		return err
	}

	for _, v := range c.Teams {
		err = v.saveTeam(f)
		if err != nil {
			fmt.Printf("Couldn't write Team '%s' to file '%s'!\n", v.Name, filePath)
			return err
		}
	}

	return nil
}

func (m *Match) saveMatch(f *os.File) error {
	_, err := f.WriteString(m.toString())
	if err != nil {
		fmt.Printf("Couldn't save the match (%d)!\n", m.Id)
		return err
	}
	return nil
}

func (c *Championship) saveMatches(filePath string) error {
	f, err := os.OpenFile(filePath, os.O_CREATE|os.O_WRONLY, 0777)
	defer f.Close()

	if err != nil {
		fmt.Printf("Error opening file '%s'!\n", filePath)
		return err
	}

	_, err = f.WriteString(fmt.Sprintf("Matches for the championship '%s':\n", c.Name))
	if err != nil {
		fmt.Printf("Couldn't write to file '%s'!\n", filePath)
		return err
	}

	for _, v := range c.Matches {
		err = v.saveMatch(f)
		if err != nil {
			fmt.Printf("Couldn't write Match '%d' to file '%s'!\n", v.Id, filePath)
		}
	}

	return nil
}

func swap(array []string, i, j int) {
	temp := array[i]
	array[i] = array[j]
	array[j] = temp
}

func partition(c *Championship, keys []string, low, high int) int {
	pivot := c.Teams[keys[high]]
	i, j := low-1, low
	for j < high {
		curr := c.Teams[keys[j]]
		if pivot.Points < curr.Points ||
			(pivot.Points == curr.Points && pivot.Goals < curr.Goals ||
				pivot.Goals == curr.Goals && pivot.Name > curr.Name) {
			i++
			swap(keys, i, j)
		}
		j++
	}
	i++
	swap(keys, i, high)
	return i
}

func quick_sort(c *Championship, keys []string, low, high int) {
	if low < high {
		p := partition(c, keys, low, high)
		quick_sort(c, keys, low, p-1)
		quick_sort(c, keys, p+1, high)
	}
}

func (c *Championship) quickSortTeams() ([]string, int) {
	keys := keys(c.Teams)
	n := len(keys)
	quick_sort(c, keys, 0, n-1)
	return keys, n
}

func (t *Team) writeTeamTableRow(w io.Writer) error {
	_, err := fmt.Fprintf(w, "|\t%s\t|\t%d\t|\t%d\t|\n", t.Name, t.Goals, t.Points)
	return err
}

func (c *Championship) writeTeamsRanked(w io.Writer) error {
	tw := tabwriter.NewWriter(w, 1, 1, 1, ' ', 0)
	_, err := fmt.Fprintf(tw, "|\t\t\t%s\t\t\t|\n", c.Name)
	if err != nil {
		return err
	}

	_, err = fmt.Fprintf(tw, "|\t%s\t|\t%s\t|\t%s\t|\n", "Team Name", "Goals", "Points")
	if err != nil {
		return err
	}

	sortedKeys, n := c.quickSortTeams()
	for i := 0; i < n; i++ {
		err = c.Teams[sortedKeys[i]].writeTeamTableRow(tw)
		if err != nil {
			return err
		}
	}

	return tw.Flush()
}

func (c *Championship) writeTeamsRankedFile(filePath string) error {
	f, err := os.OpenFile(filePath, os.O_CREATE|os.O_WRONLY, 0777)
	defer f.Close()
	if err != nil {
		fmt.Printf("Couldn't open file '%s'!\n", filePath)
		return err
	}
	return c.writeTeamsRanked(f)
}

func (c *Championship) saveJson(filePath string) error {
	bytes, err := json.MarshalIndent(c, "", "\t")
	if err != nil {
		fmt.Printf("Couldn't marshal data '%s'\n", c.Name)
		return err
	}

	f, err := os.OpenFile(filePath, os.O_CREATE|os.O_WRONLY, 0777)
	if err != nil {
		fmt.Printf("Couldn't open the file '%s'!\n", filePath)
		return err
	}

	_, err = f.Write(bytes)
	if err != nil {
		fmt.Printf("Couldn't write to file '%s'\n", filePath)
		return err
	}

	return nil
}

func (m *Match) change() error {
	g1, g2 := m.HalfTime.HomeTeam, m.HalfTime.GuestTeam

	fmt.Printf("You're chaning team '%d':\n", m.Id)
	fmt.Printf("%s\n", m.toString())

	var cg1, cg2 int
	for {

		fmt.Printf("Input goal number for the home team '%s': ", m.HomeTeam.Name)
		_, err := fmt.Scanf("%d", &cg1)
		if err != nil {
			fmt.Printf("Error: %s\n", err.Error())
			continue
		}

		fmt.Printf("Input goal number for the guest team '%s': ", m.GuestTeam.Name)
		_, err = fmt.Scanf("%d", &cg2)
		if err != nil {
			fmt.Printf("Error: %s\n", err.Error())
			continue
		}

		if cg1+cg2 != g1+g2 {
			s := fmt.Sprintf("Sum of goals in the match '%d' changed!\n", m.Id)
			return fmt.Errorf(s)
		} else if cg1 == g1 {
			s := fmt.Sprintf("Nothing to change '%d'!\n", m.Id)
			return fmt.Errorf(s)
		}

		break
	}

	m.HalfTime.HomeTeam = cg1
	m.HalfTime.GuestTeam = cg2

	m.HomeTeam.Goals += cg1 - g1
	m.GuestTeam.Goals += cg2 - g2

	fmt.Printf("Match score changed successfully!\n%s\n", m.toString())

	return nil
}

func (m *Match) toString() string {
	return fmt.Sprintf(
		"Id: %d\nTime: %s\nHome team: %s\nGuest team: %s\nResult: home(%d)-guest(%d)\n",
		m.Id,
		m.Time.Format(time.Kitchen),
		m.HomeTeam.Name,
		m.GuestTeam.Name,
		m.HalfTime.HomeTeam,
		m.HalfTime.GuestTeam)
}

func (c *Championship) displayMatches() {
	fmt.Printf("Matches:\n")
	for _, v := range c.Matches {
		fmt.Printf("%s\n", v.toString())
	}
}

func (c *Championship) changeMatch() {
	for {
		fmt.Printf("Input ID of the match you wish to modify or 'BACK': ")
		var input string
		fmt.Scanf("%s", &input)
		if strings.ToLower(input) == "back" {
			break
		}
		id, err := strconv.ParseInt(input, 10, 64)
		if err != nil {
			fmt.Printf("Error: %s\n", err.Error())
			continue
		}

		if id < 0 || id > int64(len(c.Matches)) || c.Matches[id] == nil {
			fmt.Printf("Match with the ID [%d] doesn't exist!\n", id)
			continue
		}

		c.Matches[id].change()
	}
}

func (c *Championship) modify() {
	work := true
	for work {
		fmt.Printf("Modify one of the matches:\n")
		fmt.Printf("\t1 - Display all matches\n")
		fmt.Printf("\t2 - Modify match by ID\n")
		fmt.Printf("\t3 - Exit\n")

		var opt int
		_, err := fmt.Scanf("%d", &opt)
		if err != nil {
			fmt.Printf("Error: %s\n", err.Error())
			continue
		}

		if opt < 1 || opt > 3 {
			fmt.Printf("Please select option 1-3!\n")
			continue
		}

		switch opt {
		case 1:
			c.displayMatches()
		case 2:
			c.changeMatch()
		case 3:
			work = false
		}
	}
}

func main() {
	rand.Seed(time.Now().UnixMilli())

	c := Championship{}
	c.createChampionshipTeamsFromFile("championship.json")
	c.createMatches()
	c.saveTeams("./output/timovi.txt")
	c.saveMatches("./output/mecevi.txt")
	c.writeTeamsRankedFile("./output/timovi-rangirani.txt")
	c.writeTeamsRanked(os.Stdout)
	c.modify()

	fmt.Printf("Goals: %d/%d\n", c.GoalNumber, c.Restrictions.ExactNumberOfGoals)
	c.saveJson("c.json")
}
