////////////////////////// TP10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prod.h"

int lireProchaineCommande() //pour lire l'int contenu dans nextFact
{
FILE *f;
int N;
f=fopen("nextFact","r");
fread(&N,sizeof(int),1,f);
fclose(f);
//printf("\n--->lu N=%d",N);
return N;
}

void convertirNenChaine4(int N,char *N4) //convertit l'int N en une chaine de 4 caracteres
{ // si N=1234 alors N4 sera égal à "1234"
int m,c,d,u;
char cm,cc,cd,cu;
m=N/1000;cm=48+m;
N%=1000;
c=N/100;cc=48+c;
N%=100;
d=N/10;cd=48+d;
u=N%10;
cu=48+u;
N4[0]=cm;N4[1]=cc;N4[2]=cd;N4[3]=cu;N4[4]='\0';
}

void rechercherProduit(char * refWanted,FILE* Produit, char* libele, float* prix){
	char ref[10];
	fseek(Produit, 0, SEEK_SET);
	do{
		fscanf(Produit, "%s %s %f", ref,libele,prix);
	}while(strcmp(refWanted,ref)!=0);
}









void lireCommande(FILE* fic,char* NNNN,FILE* Produit){
	char nom[100];
	char ref[10];
	int num;
	char chemin[50];
	char libele[50]; 
	float prix,ttl = 0;
	FILE* fact = NULL;
	sprintf(chemin, "./factures/facture%s.txt",NNNN);
	fact = fopen(chemin, "w");
	fscanf(fic, "%s",nom);
	printf("nom = \"%s\"\n",nom);
	fprintf(fact, "Client : %s\n", nom);
	do{
		fscanf(fic,"%s %d", ref,&num);
		rechercherProduit(ref,Produit, libele, &prix);
		ttl += prix*num;
		printf("ref=\"%s\" num = %d libélé = \"%s\" prix = %.2f prix ttl = %.2f\n",ref, num,libele, prix,prix*num);
		fprintf(fact, "%d %s  (PU=%.2f€) :: %.2f€\n",num,libele,prix,prix*num);
	}while(!feof(fic));
	fprintf(fact, "\n			TOTAL = %.2f€ ", ttl);
	fclose(fact);
}

void lireLesCommandes() //cette fonction ouvre tous les fichiers commandeXXX.txt avec XXXX démarrant à N
{
FILE *ficCommande=NULL;
int FINI=0;
int N = lireProchaineCommande(); //numero de la premiere commande qui sera lue et traitee
char NNNN[5];
char nomCommande[29];
FILE* Produit = fopen("produits.txt", "r");
if(Produit==NULL) return;

do //ce do while prend fin dès que fichier commandeXXXX.txt est absent 
	{
	strcpy(nomCommande,"./commandes/commande");
	convertirNenChaine4(N,NNNN); 
	//printf("\n==>%s<==",NNNN);
	ficCommande=NULL;
	strcat(nomCommande,NNNN);
	strcat(nomCommande,".txt");
	
	printf("\ntraitement de  %s\n",nomCommande);
	
	ficCommande=fopen(nomCommande,"rt");
	if (ficCommande!=NULL)
		{ // le fichier commandeNNNN.txt existe
			printf("fichier %s present\n",nomCommande);
			lireCommande(ficCommande,NNNN, Produit); // à vous de coder cette fonction lors de ce TP9
			fclose(ficCommande);
		}
	else
		{
			printf("\ntoutes les commandes presentes ont ete traitees.\n");
			FILE *f=fopen("nextFact","w"); // on va ecrire la valeur de N dans enxtFact 
			// pour 
			fwrite(&N,1,sizeof(int),f);
			fclose(f);
			fclose(Produit);
			FINI=1;			
		}

	N++;
	}while(FINI==0);		

}







void stock_alert(char* new_ref,FILE* alert){
	fseek(alert, 0, SEEK_SET);
	char ref[10];//*
	do{
		fscanf(alert, "%s", ref);
		//printf("%s %s \n",ref,new_ref);
		if(strcmp(ref,new_ref)==0){
			printf("Warning reminder\n");
			return;
		}
	}while(!feof(alert));
	if(ftell(alert) >0) fprintf(alert, "\n%s", new_ref);
	else fprintf(alert, "%s", new_ref);
	printf("Warning\n"); //*/
}

int MAJStock(FILE* stock,char* refWanted,int num,FILE* alert){
	char ref[10];
	int ttl;
	fseek(stock, 0, SEEK_SET);
	do{
		fscanf(stock, "%s %5d", ref,&ttl);
	}while(strcmp(refWanted,ref)!=0);
	if(ttl>=num){
		if(ttl == num) stock_alert(refWanted,alert);
		ttl += -num;
		fseek(stock, -5, SEEK_CUR);
		fprintf(stock,"%5d",ttl);
		return 1;
	}else{
		stock_alert(refWanted,alert);
		return 0;
	}
}

void lireCommande2(FILE* fic,char* NNNN,FILE* Produit,FILE* stock,FILE* alert){
	char nom[100];
	char ref[10];
	int num;
	char chemin[50];
	char libele[50]; 
	float prix,ttl = 0;
	FILE* fact = NULL;
	sprintf(chemin, "./factures/facture%s.txt",NNNN);
	fact = fopen(chemin, "w");
	fscanf(fic, "%s",nom);
	printf("nom = \"%s\"\n",nom);
	fprintf(fact, "Client : %s\n", nom);
	do{
		fscanf(fic,"%s %d", ref,&num);
		rechercherProduit(ref,Produit, libele, &prix);
		if(MAJStock(stock,ref,num,alert)){
			ttl += prix*num;
			printf("ref=\"%s\" num = %d libélé = \"%s\" prix = %.2f prix ttl = %.2f\n",ref, num,libele, prix,prix*num);
			fprintf(fact, "%d %s  (PU=%.2f€) :: %.2f€\n",num,libele,prix,prix*num);
		}
	}while(!feof(fic));
	fprintf(fact, "\n			TOTAL = %.2f€ ", ttl);
	fclose(fact);
}

void lireLesCommandes2() //cette fonction ouvre tous les fichiers commandeXXX.txt avec XXXX démarrant à N
{
FILE *ficCommande=NULL;
int FINI=0;
int N = lireProchaineCommande(); //numero de la premiere commande qui sera lue et traitee
char NNNN[5];
char nomCommande[29];
FILE* Produit = fopen("produits.txt", "r");
if(Produit==NULL) return;
FILE* stock = fopen("stock.txt", "r+");
if(stock==NULL) return;
FILE* alert = fopen("alertes.txt", "a+");

do //ce do while prend fin dès que fichier commandeXXXX.txt est absent 
	{
	strcpy(nomCommande,"./commandes/commande");
	convertirNenChaine4(N,NNNN); 
	//printf("\n==>%s<==",NNNN);
	ficCommande=NULL;
	strcat(nomCommande,NNNN);
	strcat(nomCommande,".txt");
	
	printf("\ntraitement de  %s\n",nomCommande);
	
	ficCommande=fopen(nomCommande,"rt");
	if (ficCommande!=NULL)
		{ // le fichier commandeNNNN.txt existe
			printf("fichier %s present\n",nomCommande);
			lireCommande2(ficCommande,NNNN, Produit,stock,alert); // à vous de coder cette fonction lors de ce TP9
			fclose(ficCommande);
		}
	else
		{
			printf("\ntoutes les commandes presentes ont ete traitees.\n");
			FILE *f=fopen("nextFact","w"); // on va ecrire la valeur de N dans enxtFact 
			// pour 
			fwrite(&N,1,sizeof(int),f);
			fclose(f);
			fclose(Produit);
			fclose(alert);
			FINI=1;			
		}

	N++;
	}while(FINI==0);		

}








int main()
{
	//creation d un fichier d'un seul int nommé nextFact et contenant l'int 1
	// code à utiliser pour réinitialiser nextFact à 1 si besoin au cours du TP 
	
	/*
	FILE *f;int N=1;
	f=fopen("nextFact","w");
	fwrite(&N,1,sizeof(int),f);
	fclose(f);
	//*/ 	

	//PARTIE 1 du TP : sans Gestion de stock
	//lireLesCommandes(); //lecture de tous les fichiers commandeXXX.txt (fichiers non traités jusqu'ici)	
	

	//PARTIE 2 du TP : avec Gestion de stock
	//copiez coller votre travail précédent puis modifiez le  
	lireLesCommandes2(); 	

	return 0;
}
