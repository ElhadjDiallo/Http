#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char * extraitFichier(char *requete,int tailleTableauNomFichier);
int rechercher(char *chaine,char *souschaine);
int lectureFichier(char *nomfichier);
int envoyerContenuFichierBinaire(char *nomFichier);
int main()
{

   char ch[40];
   printf("Entrer une requete \n");
   scanf("%s",ch);
     envoyerContenuFichierBinaire(ch);
}
int envoyerContenuFichierBinaire(char *nomFichier)
{
        int i,nblus;
FILE *fichier;
unsigned char chaine;
unsigned char *chaineAenvoyer;
chaineAenvoyer=malloc(4*sizeof(char));
fichier=fopen(nomFichier,"rb");
i=0;
if(fichier!=NULL)
{
  do
    {
        nblus=fread(&chaine,sizeof(chaine),1,fichier); 
    if(nblus!=0)
    {     
   // printf("%d ",chaine);
    chaineAenvoyer[i]=chaine;
    chaineAenvoyer[i+1]='\0';
    //strcat(chaineAenvoyer,"\n");
    //Emission(chaineAenvoyer);
    printf("%d",chaineAenvoyer[i]);
    i=0;
    }
       
    }while(!feof(fichier));

        
    
   
   fclose(fichier);
}
else
fprintf(stderr, "erreur " );




}

int lectureFichier(char *nomfichier)
{
    int i,nblus;
FILE *fichier;
char chaine[40];
fichier=fopen(nomfichier,"rb");  //ouverture du fichier en mode binaire et lecture 
if(fichier!=NULL)
{
    i=0;
    while(!feof(fichier))
  {
    nblus=fread(&chaine,sizeof(chaine),1,fichier); //lecture du fichier par bloc à partir de fread
    
        //affichage à l'ecran de l'aire et du numero de bloc corespondant 
    printf("%s ",chaine);

    
    
   }
}
else
fprintf(stderr, "erreur " );

}
char * extraitFichier(char *requete,int tailleTableauNomFichier)
{
     char *fichier;
     char *chaine;
     chaine=malloc(sizeof(char));
     fichier=malloc(sizeof(char));
     
     
     if(rechercher(requete,"HTTP")!=1)
     {
      printf("Erreur de syntaxe manque HTTP dans votre requête \n");
      return NULL;
     }
     fichier=strtok(requete,"/");
     if(strncmp(fichier,"GET",3)!=0)
     {
      printf("Erreur de syntaxe manque GET dans votre requete \n");
      
     }
     fichier=strtok(NULL," ");

    chaine=strtok(NULL,"/");

     if(strncmp(chaine,"HTTP",4)!=0)
     {

     printf("Erreur de syntaxe manque HTTP dans votre requete \n");
     return NULL;
      
     }
     
     return fichier;
     
    
      

}


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

#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"
#include <string.h>

int main() {
    
    
    
    char deconexion[5];

    char *message = NULL;
  
   char *fichier;
     
    Initialisation();
  strcpy(deconexion,"eof\n");
    while(1) 
    {
        int fini = 0;
    
        AttenteClient();
    
        while(!fini) 
        {
            
            message = Reception();
        
            printf("le me %s",message);             
          
           
            
       
            if(message != NULL) 
            {
                
                
                fichier=extraitFichier(message,10);
                //envoyerContenuFichier(fichier);
                /*else 
                envoyerReponse500("syntaxe");*/

                free(message);
                Emission(deconexion);
                        
                
               } 
            else 
            {
                
                fini = 1;
            }
        
            
            
        }
        
     
     TerminaisonClient();       
    }


    return 0;
}
