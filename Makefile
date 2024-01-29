# Répertoires
DOSSIER_DATA = data
DOSSIER_C = progc

# Fichiers
FICHIER_DATA = $(DOSSIER_DATA)/data.csv
FICHIER_PROJET = projet

all: script

clean:
	rm $(FICHIER_PROJET)

build:
	gcc $(DOSSIER_C)/main.c $(DOSSIER_C)/fonctionsAVL.c $(DOSSIER_C)/fonctionsT.c $(DOSSIER_C)/fonctionsS.c -o $(FICHIER_PROJET)

run:
	./$(FICHIER_PROJET)

script:
	sh shell.sh $(FICHIER_DATA) $(ARG)
