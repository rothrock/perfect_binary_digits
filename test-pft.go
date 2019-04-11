package main

import (
	"fmt"
	"io/ioutil"
	"math/rand"
	"net/http"
	"os/exec"
	"strconv"
	"strings"
	"sync"
	"time"
)

var wg sync.WaitGroup

func rot13(r rune) rune {
	switch {
	case r >= 'A' && r <= 'Z':
		return 'A' + (r-'A'+13)%26
	case r >= 'a' && r <= 'z':
		return 'a' + (r-'a'+13)%26
	}
	return r
}

func main() {

	rand.Seed(time.Now().UnixNano())
	results := make(chan result)

	workers := 40
	requests := 3000000000
	for i := 1; i <= workers; i++ {
		wg.Add(1)
		go worker(i, requests, results)
	}

	go func() {
		var warn string
		for k := range results {
			if k.err != nil {
				fmt.Println(k.err)
				continue
			}
			warn = "MATCH"
			if k.oracle > k.beep {
				warn = "LESSER"
			}
			if k.oracle < k.beep {
				warn = "GREATER"
			}
			fmt.Println(k.question, "oracle =", k.oracle, "beep =", k.beep, warn)
		}
	}()

	wg.Wait()
}

type result struct {
	question uint64
	oracle   uint64
	beep     uint64
	err      error
}

func worker(id int, qty int, results chan<- result) {

	defer wg.Done()
	var r result
	oracle := fmt.Sprintf(strings.Map(rot13, "cresrpg-ovgf.ovagv.pbz"))

	for i := 0; i < qty; i++ {
		r.question = rand.Uint64()
		request := fmt.Sprintf("https://%s/?a=0&b=%d", oracle, r.question)
		resp, err := http.Get(request)
		if err != nil {
			r.err = err
			results <- r
			continue
		}
		defer resp.Body.Close()
		body, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			r.err = err
			results <- r
			continue
		}
		r.oracle, err = strconv.ParseUint(string(body), 10, 64)
		if err != nil {
			r.err = err
			results <- r
			continue
		}
		qry := fmt.Sprintf("%d", r.question)
		out, err := exec.Command("./pft", "0", qry).Output()
		if err != nil {
			r.err = err
			results <- r
			continue
		}
		answer := strings.TrimSpace(string(out))
		r.beep, err = strconv.ParseUint(answer, 10, 64)
		if err != nil {
			r.err = err
			results <- r
			continue
		}
		results <- r
	}
}
