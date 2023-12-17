# Assignment 1 - Monsoon 2023

## Due Date: Aug 29, 2023, 23:59 Hrs. (Hard Deadline)

### Instructions:

- One submission per group.
- Submit a .zip named RollNo1_RollNo2.zip containing code and write-up.

---

## Q1: Process Relationships and Calculation (Total points: 30)

- Create parent-child relationship between two processes.
- Parent prints statements:
  A) Parent (P) is having ID <PID>
  B) ID of P’s Child is <PID_of_Child>
- Child prints statements:
  C) Child is having ID <PID>
  D) My Parent ID is <PID_of_Parent>
- Use vfork() system call.
- Parent calculates Fibonacci series up to 16, child calculates factorial of 4.
- Bonus: Reverse roles where child waits for parent.

### Grading Rubric:

- Source code with Makefile.
- Brief description of program's functionality (<1 page).
- Error handling.

---

## Q2: Unix Commands for Custom Shell (Total points: 50)

### Commands:

1) `word`: Reads word count in a text file.
   - Options: `-n` (ignore new line character), `-d` (difference between word sizes).
2) `dir`: Creates and changes path to a directory.
   - Options: `-r` (remove if directory exists), `-v` (verbose mode).
3) `date`: Returns last modified date and time of a file.
   - Options: `-d` (display time described by STRING), `-R` (output date and time in RFC 5322 format).

### Grading Rubric:

- Source code with Makefile.
- Description of commands' functionality (<1 page).
- Usage of C libraries and system calls.
- Error handling for wrong commands/options/arguments.

---

## Q3: Arithmetic Calculator (Total points: 20)

- Create a bash script acting as a math calculator.
- Read inputs from "input.txt", perform operations, and save results in "output.txt" in "Result" directory.
- Operations: 'xor', 'product', 'compare'.

### Grading Rubric:

- Bash script with Makefile.
- Brief description of the script's functionality (<1 page).
- Read/write operations using bash script only.
- Directory creation through bash script.

---

## Test Cases:

### Q1:
- Flow of expected output is specified.

### Q2:
- Test output based on assumptions provided.

### Q3:
#### Input.txt
