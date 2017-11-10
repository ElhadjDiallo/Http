#include <stdio.h>
#include <stdlib.h>


int main() {
	char *message = NULL;
     char *fichier;
	Initialisation();

	while(1) {
		int fini = 0;
	
		AttenteClient();
	
		
			message = Reception();
			if(message != NULL) {
			
                
				fichier=extraitFichier(message,10);
				
                if(fichier!=NULL)
                {
                	if(rechercher(fichier,"png")==0  &&  rechercher(fichier,"jpg")==0)
                   envoyerContenuFichier(fichier);
                  else 
                   envoyerContenuFichierBinaire(fichier);
			    	
                }
				else 
			    {
			    	
			    envoyerReponse500("fichier");	
			    }
			    	
				free(message);


				
			} 
			

		TerminaisonClient();
	}

	
	return 0;
}



		             
				
		

