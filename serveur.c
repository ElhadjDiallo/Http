#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <errno.h>

#include "serveur.h"

#define TRUE 1
#define FALSE 0
#define LONGUEUR_TAMPON 4096


#ifdef WIN32
#define perror(x) printf("%s : code d'erreur : %d\n", (x), WSAGetLastError())
#define close closesocket
#define socklen_t int
#endif

/* Variables cachees */

/* le socket d'ecoute */
int socketEcoute;
/* longueur de l'adresse */
socklen_t longeurAdr;
/* le socket de service */
int socketService;
/* le tampon de reception */
char tamponClient[LONGUEUR_TAMPON];
int debutTampon;
int finTampon;


/* Initialisation.
 * Creation du serveur.
 */



 /*

fonction qui lit le 
fichier en binaire 
puis l'envoie à partir d'emission binaire 

 */
int envoyerContenuFichierBinaire(char *nomFichier)
{

int nblus;
FILE *fichier;
 char chaine[100];


fichier=fopen(nomFichier,"rb");
if(fichier!=NULL)
{
	
    envoyerReponse200IMAGEHTML(nomFichier);
  do
    {
      nblus=fread(chaine,1,sizeof(chaine),fichier); 
    if(nblus!=0)
    {
            
      
    	
         EmissionBinaire(chaine,nblus);
    }
       
    }while(!feof(fichier));
    
        
    
   
   fclose(fichier);
   
}
else
envoyerReponse404(nomFichier);

}
/*
fonction qui lit le fichier par chaine de caractère et l'envoie à 
partir de emission  

et envoie les messages d'erreurs necessaires si on arrive pas à 


ouvrir le fichier 

*/
int envoyerContenuFichier(char *fichier)
{   char contenu[100];
	FILE *monfichier;
	monfichier=fopen(fichier,"r");

	
      
	if(monfichier!=NULL)
	{
		
	     
	    envoyerReponse200HTML(fichier);
	
    while(fgets(contenu,100,monfichier)!=NULL)
       {
      
         Emission(contenu);  
       }
       fclose(monfichier);
	}
	else
	{
		
      envoyerReponse404(fichier);		
	}

	
}
/*

Entête pour emission de l'image 

*/
int envoyerReponse200IMAGEHTML(char *fichier)
{

	unsigned long t;
	char *chaine,*ch;
	
	ch=malloc(sizeof(char));
    if(Emission("HTTP/1.1 200 OK\n")!=1)
    	printf("Erreur d'emision de HTTP/1.1 200 ok \n");
 

	Emission("Content-type: image/jpg\n");
	t=longueur_fichier(fichier);
	sprintf(ch,"%ld",t);
	chaine=malloc(strlen("content-length ")+strlen(ch)+1*sizeof(char));
	strcpy(chaine,"Content-length: ");

	strcat(chaine,ch);
	strcat(chaine,"\n");
    Emission(chaine);
    Emission("\n");
}
/*
fonction qui envoie une reponse 200 ok 
puis les autres informations necessaires tel que 
content-length et content-type 
puis on a utilisé la fonction sprintf pour transformer l'entier qui recupère la taille 
du fichier en chaine de caractère  pour pouvoir l'envoyer 
à partir de la fonction emission 

*/
int envoyerReponse200HTML(char *fichier)
{

	unsigned long t;
	char *chaine,*ch;
	
	ch=malloc(sizeof(char));
    if(Emission("HTTP/1.1 200 OK\n")!=1)
    	printf("Erreur d'emision de HTTP/1.1 200 ok \n");
 

	Emission("Content-type: text/html\n");
	t=longueur_fichier(fichier);
	sprintf(ch,"%ld",t);
	chaine=malloc(strlen("content-length ")+strlen(ch)+1*sizeof(char));
	strcpy(chaine,"Content-length: ");

	strcat(chaine,ch);
	strcat(chaine,"\n");
    Emission(chaine);
    Emission("\r\n\r\n");
}
/*

identique que la fonction precedente 
envoie la chaine de caractère et recupère la chaine du caractère 

*/
int envoyerReponse500(char*monfichier)
{
	unsigned long t;
	char *chaine,*ch;
	ch=malloc(sizeof(char));
	chaine=malloc(sizeof(char));
	Emission("HTTP/1.1 500 Server Error\n");
	Emission("Content-type: text/html\n");
     
	 t=0;
	sprintf(ch,"%ld",t);
    chaine=malloc(strlen("content-length ")+strlen(ch)+1*sizeof(char));
	strcpy(chaine,"Content-length:");
	strcat(chaine,ch);
	strcat(chaine,"\n");
    Emission(chaine);
    
	Emission("<HTML>\n");
    Emission("<HEAD>\n");
    Emission("<TITLE>\n");
    Emission("Error syntaxe \n");
     free(ch);
    Emission("</BODY>\n"); 
    Emission("</HTML>\n");

}

/*
fonction qui envoie un message d'erreur pour cela nous envoyons la chaine de caractère qu'il 
faut on utilise sprintf pour transformer l'entier en chaine de caractère 

*/
int envoyerReponse404(char *monfichier)
{
	unsigned long t;
	char *chaine,*ch;
	ch=malloc(sizeof(char));
	Emission("HTTP/1.1 404 Not Found \n");
	t=0;
	sprintf(ch,"%ld",t);
	chaine=malloc(strlen("content-length ")+strlen(ch)+1*sizeof(char));
	strcpy(chaine,"Content-length: ");
	strcat(chaine,ch);
	strcat(chaine,"\n");
    Emission(chaine);
     free(ch);
	Emission("<HTML>\n");
    Emission("<HEAD>\n");
    Emission("<TITLE>\n");
    Emission("impossible de trouver\n");
    ch=malloc(strlen(monfichier)+2*sizeof(char));
    strcpy(ch,monfichier);
    strcat(ch,"\n");
    Emission(ch);
    Emission("</BODY>\n"); 
    Emission("</HTML>\n");


}
/*
Fonction qui calcule la longueur du fichier 

*/
unsigned long longueur_fichier(char *fichier)
{
   unsigned	long  taille;
     FILE* fic;
     fic=fopen(fichier,"r");
      if(fic!=NULL)
     {
	  fseek(fic,0,SEEK_END);
     taille=ftell(fic);
     }

    fclose(fic);
    return taille;

}
/*fonction pour recupererer le fichier à partir de la requête

vu que la fonction strtok modifie la requête dans le programme je cree une autre chaine 
que j'appelle newrequete ou je sauvegarde la requete pour pouvoir faire les test 
necessaire 
on verifie d'abord si la requête HTTP sinon nous ramènons NULL et on sort du programme
si on essaie de recuperer la première partie de la requête separée par le slash qui doit être GET 
si effectivement nous avons GET nous verifions si il existe un fichier 
si c'est pas le cas nous ramenons si tout est bon on ramène le fichier qu'on veut traiter  
*/

char * extraitFichier(char *newrequete,int tailleTableauNomFichier)
{
	   int lg;
     char *fichier;
     char *chaine;
     char *requete;
     chaine=malloc(sizeof(char));
     fichier=malloc(sizeof(char));
     
  
     
     requete=malloc(strlen(newrequete)+1*sizeof(char));

     strcpy(requete,newrequete);
     
     if(rechercher(requete,"HTTP")!=1)
     {
     	
      
      return NULL;
     }

     fichier=strtok(requete,"/");
     if(fichier!=NULL)
     {
     	

     
    if(strncmp(fichier,"GET",3)!=0)
     {
      
      return NULL;
      
     }
     fichier=strtok(NULL," ");

    chaine=strtok(NULL,"/");
     if(chaine!=NULL)
     {
     if(strncmp(chaine,"HTTP",4)!=0)
     return NULL;
     }
     else
     	return NULL;
      
 

     
     return fichier;
 }
 else 
 	return NULL;
     
    
      

}
/* fonction qui recherche une sous chaine dans une chaine de caractère

qui prend comme premier paramètre la chaine dans la quelle on cherche la sous chaine et nous renvoie 1 si ça marche et 0 sinon 

*/ 
int rechercher(char *chaine,char *souschaine)
{
	int long1,long2,i,j;
	long1=strlen(chaine);
	long2=strlen(souschaine);
	int c,trouver,count,bon;
	trouver=0;bon=0;count=0;
	i=0;
	
    while(i<long1 && !trouver)
    {
      
    	j=0;
    	c=i;
    	if(trouver!=1)
    		bon=0;
    	while(j<long2 &&!bon)
    	{
        
    		if(chaine[c]==souschaine[j])
        {
          
          count++;
        }
              
             else 
             	bon=1;
          if(count==long2-1)
          	trouver=1;
          c++;
          j++;

    	}
    	i++;
    }
    
    if(trouver==1)
    	return 1;
    else 
    	return 0;
}



int Initialisation() {
	return InitialisationAvecService("13214");
}

/* Initialisation.
 * Creation du serveur en pr�cisant le service ou num�ro de port.
 * renvoie 1 si �a c'est bien pass� 0 sinon
 */
int InitialisationAvecService(char *service) {
	int n;
	const int on = 1;
	struct addrinfo	hints, *res, *ressave;

	#ifdef WIN32
	WSADATA	wsaData;
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR)
	{
		printf("WSAStartup() n'a pas fonctionne, erreur : %d\n", WSAGetLastError()) ;
		WSACleanup();
		exit(1);
	}
	memset(&hints, 0, sizeof(struct addrinfo));
    #else
	bzero(&hints, sizeof(struct addrinfo));
	#endif

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(NULL, service, &hints, &res)) != 0)  {
     		printf("Initialisation, erreur de getaddrinfo : %s", gai_strerror(n));
     		return 0;
	}
	ressave = res;

	do {
		socketEcoute = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketEcoute < 0)
			continue;		/* error, try next one */

		setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
#ifdef BSD
		setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
#endif
		if (bind(socketEcoute, res->ai_addr, res->ai_addrlen) == 0)
			break;			/* success */

		close(socketEcoute);	/* bind error, close and try next one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL) {
     		perror("Initialisation, erreur de bind.");
     		return 0;
	}

	/* conserve la longueur de l'addresse */
	longeurAdr = res->ai_addrlen;

	freeaddrinfo(ressave);
	/* attends au max 4 clients */
	listen(socketEcoute, 4);
	printf("Creation du serveur reussie sur %s.\n", service);

	return 1;
}

/* Attends qu'un client se connecte.
 */
int AttenteClient() {
	struct sockaddr *clientAddr;
	char machine[NI_MAXHOST];

	clientAddr = (struct sockaddr*) malloc(longeurAdr);
	socketService = accept(socketEcoute, clientAddr, &longeurAdr);
	if (socketService == -1) {
		perror("AttenteClient, erreur de accept.");
		return 0;
	}
	if(getnameinfo(clientAddr, longeurAdr, machine, NI_MAXHOST, NULL, 0, 0) == 0) {
		printf("Client sur la machine d'adresse %s connecte.\n", machine);
	} else {
		printf("Client anonyme connecte.\n");
	}
	free(clientAddr);
	/*
	 * Reinit buffer
	 */
	debutTampon = 0;
	finTampon = 0;

	return 1;
}

/* Recoit un message envoye par le serveur.
 */
char *Reception() {
	char message[LONGUEUR_TAMPON];
	int index = 0;
	int fini = FALSE;
	int retour = 0;
	while(!fini) {
		/* on cherche dans le tampon courant */
		while((finTampon > debutTampon) &&
			(tamponClient[debutTampon]!='\n')) {
			message[index++] = tamponClient[debutTampon++];
		}
		/* on a trouve ? */
		if ((index > 0) && (tamponClient[debutTampon]=='\n')) {
			message[index++] = '\n';
			message[index] = '\0';
			debutTampon++;
			fini = TRUE;
#ifdef WIN32
			return _strdup(message);
#else
			return strdup(message);
#endif
		} else {
			/* il faut en lire plus */
			debutTampon = 0;
			retour = recv(socketService, tamponClient, LONGUEUR_TAMPON, 0);
			if (retour < 0) {
				perror("Reception, erreur de recv.");
				return NULL;
			} else if(retour == 0) {
				fprintf(stderr, "Reception, le client a ferme la connexion.\n");
				return NULL;
			} else {
				/*
				 * on a recu "retour" octets
				 */
				finTampon = retour;
			}
		}
	}
	return NULL;
}

/* Envoie un message au client.
 * Attention, le message doit etre termine par \n
 */
 
int Emission(char *message) {
	int taille;
	if(strstr(message, "\n") == NULL) {
		fprintf(stderr, "Emission, Le message n'est pas termine par \\n.\n");
		return 0;
	}
	taille = strlen(message);
	if (send(socketService, message, taille,0) == -1) {
        perror("Emission, probleme lors du send.");
        return 0;
	}
	printf("Emission de %d caracteres.\n", taille+1);
	return 1;
}


/* Recoit des donnees envoyees par le client.
 */
int ReceptionBinaire(char *donnees, size_t tailleMax) {
	size_t dejaRecu = 0;
	int retour = 0;
	/* on commence par recopier tout ce qui reste dans le tampon
	 */
	while((finTampon > debutTampon) && (dejaRecu < tailleMax)) {
		donnees[dejaRecu] = tamponClient[debutTampon];
		dejaRecu++;
		debutTampon++;
	}
	/* si on n'est pas arrive au max
	 * on essaie de recevoir plus de donnees
	 */
	if(dejaRecu < tailleMax) {
		retour = recv(socketService, donnees + dejaRecu, tailleMax - dejaRecu, 0);
		if(retour < 0) {
			perror("ReceptionBinaire, erreur de recv.");
			return -1;
		} else if(retour == 0) {
			fprintf(stderr, "ReceptionBinaire, le client a ferme la connexion.\n");
			return 0;
		} else {
			/*
			 * on a recu "retour" octets en plus
			 */
			return dejaRecu + retour;
		}
	} else {
		return dejaRecu;
	}
}

/* Envoie des donn�es au client en pr�cisant leur taille.
 */
int EmissionBinaire(char *donnees, size_t taille) {
	int retour = 0;
	retour = send(socketService, donnees, taille, 0);
	if(retour == -1) {
		perror("Emission, probleme lors du send.");
		return -1;
	} else {
		
		return retour;
	}
}



/* Ferme la connexion avec le client.
 */
void TerminaisonClient() {
	close(socketService);
}

/* Arrete le serveur.
 */
void Terminaison() {
	close(socketEcoute);
}
