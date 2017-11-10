#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include <string.h>
int main() {
	char *message;
	char *msg;
	int taille;
     char deconnection[5];
	if(Initialisation("localhost") != 1) {
		printf("Erreur d'initialisation\n");
		return 1;
	}

	if(Emission("GET /s.html HTTP/1.1\n")!=1) {
		printf("Erreur d'emission\n");
		return 1;
	}


/*message=malloc(100*sizeof(char));
do
{
  	
    
taille=ReceptionBinaire(message,100);
printf("recu egal %d \n",taille);
	if(message != NULL) {
		
		printf("J'ai recu: \n");
         	     
		free(message);
	} 
	else
	 {

		printf("Erreur de reception\n");
		return 1;
	}
	 
     

 }while(message!=NULL);
   

	Terminaison();

	return 0;*/
}
