all: scheduler sched_fifo sched_rr sched_other count seggs

scheduler: scheduler.c
	gcc -o scheduler scheduler.c 

sched_rr: sched_rr.c
	gcc -o sched_rr sched_rr.c

sched_fifo: sched_fifo.c
	gcc -o sched_fifo sched_fifo.c

sched_other: sched_other.c
	gcc -o sched_other sched_other.c

count: counter.c
	gcc -o count counter.c

seggs: seggs.py
	chmod +x seggs.py

clean:
	rm -f scheduler count sched_other sched_fifo sched_rr

