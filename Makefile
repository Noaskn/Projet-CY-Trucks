all : build script

clean :
	rm projet

build :
	gcc main.c -o projet

run :
	./projet

script :
	sh shell.sh data.csv