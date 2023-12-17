# Assignment 2 - Monsoon 2023

## Due Date: September 19, 2023, 23:59 Hrs. (Hard Deadline)

### Instructions:

- One submission per group.
- Submit a .zip file named RollNo1_RollNo2.zip containing code and write-up.

---

## Q1 Setting-up Artix OS VM and Compiling the OS kernel (Total points: 30)

- Set up a testbench VM:
  - Specifications: ~4 GB RAM, 2 virtual CPU cores, 20 GB hard drive.
  - Install Artix-base (runit version) following text mode instructions.
  - Enable ArchLinux repositories and install required packages.

- Download the stock Linux kernel, compile it, and ensure successful booting.
  - Use the attached kernel config provided.
  - Follow specific instructions for compiling.

- Additional info for VMware and VirtualBox configurations provided.

### Grading Rubric:

- UEFI-enabled VM booting Artix with correct drive partitioning.
- VM running compiled stock kernel.

---

## Q2 Process Scheduling (Total points: 70)

- Create three processes counting to 2^32 using fork() and execl() system calls.
  - Process 1: SCHED OTHER scheduling with nice:0.
  - Process 2: SCHED RR scheduling with default priority.
  - Process 3: SCHED FIFO scheduling with default priority.

- Benchmark and generate histograms showcasing completion times based on scheduling policies.
  - Use clock_gettime() to time process execution.
  - Plot histograms showing completion times and priorities.

### Grading Rubric:

- Successful compilation of programs.
- Proper system calls for process creation and scheduling.
- Error handling and reproducible behavior.
- Makefile provided.
- One-page README explaining program logic and test outcomes.

---

## Q3 Module Programming (Total points: 50)

- Write a kernel module implementing a system call.
  - Read task_struct entries and print the number of running processes.
  - Module functional only when loaded.

- Use required header files and ensure functionality in Artix or any Linux distro.

### Submission:

1. Functional system call working when module loaded.
2. Proper Makefile for compilation.
3. README explaining program logic.
4. Screenshots of module loading/unloading messages.


