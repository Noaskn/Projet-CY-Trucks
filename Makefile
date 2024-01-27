all: script

clean:
	rm projet

build:
	gcc main.c fonctionsAVL.c fonctionsT.c fonctionsS.c -o projet

run:
	./projet

script:
	sh shell.sh data.csv $(ARG)
