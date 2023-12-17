all: shell lswithdate lswithdateR lswithoutdate

shell: shell.c
	gcc -o shell shell.c

lswithdate: lswithdate.sh
	chmod +x lswithdate.sh

lswithdateR: lswithdateR.sh
	chmod +x lswithdateR.sh

lswithoutdate: lswithoutdate.sh
	chmod +x lswithoutdate.sh

clean:
	rm -f shell
