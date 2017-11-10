#makefile pile 
CC=gcc
OPT= -g
EXEC2=mainServeur
EXEC=mainClient

all : $(EXEC)   $(EXEC2) 
#compile testunitaire de initialiser et ajouter ainsi que afficher et supprimer 

#compile testunitaire du module c de ma listecirculaire.c
serveur.o: serveur.c
	$(CC) -o serveur.o -c serveur.c $(OPT)

#compile mainServeur.c
mainServeur.o : mainServeur.c
	$(CC) -o mainServeur.o -c mainServeur.c $(OPT)

#link 
client.o: client.c
	$(CC) -o client.o -c client.c $(OPT)

$(EXEC):  serveur.o  mainServeur.o
	$(CC) serveur.o mainServeur.o -o $(EXEC2)
$(EXEC2):  client.o  mainClient.o
	$(CC) client.o mainClient.o -o $(EXEC)		
	

clean:
	rm -rf *.o *.exe *.out $(EXEC) $(EXEC2)

