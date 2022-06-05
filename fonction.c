#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "fonction.h"

/* this function allows to empty the buffer */

void clearBuffer(){
  int c = 0;
  while (c != '\n' && c != EOF){
    c = getchar();
  }
} 

/* this function replaces the scanf function for strings with spaces */

int lire(char *chaine, int longueur){ /* replaces scanf to read a string from the fgets function*/
  char* positionEntree = NULL;
  if (fgets(chaine, longueur, stdin) != NULL){
    positionEntree = strchr(chaine, '\n');
    if (positionEntree != NULL){
      *positionEntree = '\0';
    }
    else
    {
      clearBuffer();
    }
    return 1;
  }
    else
    {
      clearBuffer();
      return 0;
    }
}

/* this function allows to add a book and to add it to our database */

void add_book(Livre* livreData){ /*adding a book by a teacher*/
  FILE* file1 = NULL;
  FILE* file2 = NULL;
  char chaine[TAILLE_MAX];
  int i;
  int lignes=0;
  int temp;
  
  livreData = malloc(sizeof(*livreData));
  (*livreData).titre=malloc(sizeof(char*));
  (*livreData).auteur=malloc(sizeof(char*));
  printf("quel est est le titre du livre ?\n"); /*ask the user what is the title of the book*/
  lire((*livreData).titre,TAILLE_MAX);
   
  
  printf("quel est l'auteur du livre ?\n"); /*ask the user who is the author of the book*/
  lire((*livreData).auteur,TAILLE_MAX);
  

  printf("Choissisez une catégorie:\n\n"); /*sends all available categories in the nom_category file*/
    
  file1 = fopen("nom_categorie.txt", "r");
  if(file1==NULL){ /*problem with opening file*/
    printf("problème d'ouverture de fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  for(i=0;i<NOMBRE_CAT;i++){
    fgets(chaine,TAILLE_MAX, file1); 
    printf("-%d %s",i+1 , chaine);
  }
  do{  /*retrieve book category */
    printf("quel est la catégorie du livre ?\n");  
    scanf("%d",&(*livreData).categorie);
    clearBuffer();
  }while((*livreData).categorie < 1 || (*livreData).categorie > NOMBRE_CAT);

  fclose (file1);/*close the file*/
  
  file2 = fopen("base_de_donnee_livre.txt","a+"); /*opening the file with all the books*/
  if(file2==NULL){ /*problem with opening file*/
    printf("problème d'ouverture de fichier");
    printf("relancez le programme");
    exit(1);
  }  
  do{
    if (temp=='\n'){
      lignes++;
    }
  }while((temp=fgetc(file2))!= EOF);
      
  (*livreData).Identifiantlivre=lignes/4;
    
  fprintf(file2,"%d\n%s\n%s\n%d\n",(*livreData).Identifiantlivre,(*livreData).titre,(*livreData).auteur,(*livreData).categorie); 
  
  fclose (file2); /*close the file */
  free((*livreData).auteur); /* free the memory */
  free((*livreData).titre);
  
}

/* this function allows to create an account */

void create_account(Personne* p){    
  FILE* file=NULL;/*variable*/
  
  p = malloc(sizeof(*p));
  
  (*p).nom_prenom=malloc(sizeof(char*));

  (*p).nom_prenom=search_creator_personnes();

  printf("\npassword:\n");
  (*p).password=malloc(sizeof(char*));  /*prevents character restriction*/
  lire((*p).password,TAILLE_MAX);
  
  do{
    printf("\nSi vous etes un professeur. Tapez 1 \nSi vous etes un élève. Tapez 2\n");
    scanf("%d",&(*p).prof_ou_pas);
    clearBuffer();
  }while((*p).prof_ou_pas!=1 && (*p).prof_ou_pas!=2);
  
  file = fopen("base_de_donnee_personnes.txt","a+"); /* opens the file where there are all accounts*/
  
  if(file==NULL){
    printf("probleme d'ouverture du fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }

  fprintf(file,"%s\n%s\n%d\n",(*p).nom_prenom,(*p).password,(*p).prof_ou_pas); 
  
  free((*p).nom_prenom);/* free the memory */
  free((*p).password);
  free(p);
  fclose (file); /*close the file*/
}

/* function of displays the application */

void display(){
  FILE* file;
  FILE* file1;
  Livre* livre;
  int connect=0, pos=0, i, menu_student=0 ,n=0 ,menu_prof=0,line=0;
  char id[TAILLE_MAX];
  char tmp[TAILLE_MAX];
  char chaine[TAILLE_MAX];
  Personne* p;
    
  printf("#####################\n");
  printf("   CY-Biblothèque    \n");
  printf("#####################\n\n");
  do{
    printf("1. Se connecter\n2. Créer un compte\n");
    scanf("%d",&connect);
    clearBuffer();  
  }while(connect != 1 && connect != 2); 
  
  if (connect==1){
    
    pos=connection();
    file=fopen("base_de_donnee_personnes.txt","r");
    file1=fopen("base_de_donnee_livre_emprunter.txt","r");
    for(i=0;i<pos+1;i++){ /*skiping lines until get the right login*/
      fgets(id,TAILLE_MAX,file);
    }
    fgets(chaine,TAILLE_MAX,file);/*skiping lines until get the right password*/
    n=fgetc(file); /*retrieve login */
    if(n==49){ /*convert char number to int number with ascii table*/
      n=1;
    }
    if(n==50){ /*convert char number to int number with ascii table*/
      n=2;
    }
    
    if(pos >= -1){
      printf("\nconnexion avec succes\n");
      printf("Bienvenue %s\n",id);
      if(n==1){

        
        while(line!=-3){
            line=search_book_borrow(id,line);
            if(line!=-3){
              fseek(file1,0,SEEK_SET);
              for(i=0;i<line;i++){ 
                fgets(tmp,TAILLE_MAX,file1);
              }
              printf("%s",tmp);
              for(i=0;i<2;i++){ 
                fgets(tmp,TAILLE_MAX,file1);
              }
              line=line+2;
              printf("Rendre le livre avant:  ");
              printf("%s",tmp);
            }
        }    
                   
        while(menu_prof!=1 && menu_prof!=2 && menu_prof!=3 && menu_prof!=4){ /*menu prof*/
        printf("1. Emprunté un livre\n2. rendre un livre\n3. ajouter un livre\n4. Exit\n");
          scanf("%d",&menu_prof);
          clearBuffer();
        }
        if(menu_prof==1){
          borrow_book_teacher(id);
        }
        if(menu_prof==2){
          printf("fonctionnalité indisponible\n");
          exit(1);
        }
        if(menu_prof==3){
          add_book(livre);
        }
        if(menu_prof==4){
          exit(1);
        }
      }
      else if(n==2){
        while(line!=-3){
          line=search_book_borrow(id,line);
          if(line!=-3){
            fseek(file1,0,SEEK_SET);
            for(i=0;i<line;i++){ 
              fgets(tmp,TAILLE_MAX,file1);
            }
            printf("%s",tmp);
            for(i=0;i<2;i++){ 
              fgets(tmp,TAILLE_MAX,file1);
            }
            line=line+2;
            printf("Rendre le livre avant:  ");
            printf("%s\n",tmp);
          }
        }    
        
        while(menu_student!=1 && menu_student!=2 && menu_student!=3){ /*menu etudiant*/
          printf("1. Emprunté un livre\n2. rendre un livre\n3. Exit\n");
          scanf("%d",&menu_student);
          clearBuffer();
        }
        
        if(menu_student==1){
          borrow_book_student(id);
        }
        
        if(menu_student==2){
         printf("fonctionnalité indisponible\n");
         exit(1);
        }
        if(menu_student==3){
         exit(1);
        }
      }
    }
    else{
      printf("probleme\n");
      printf("relancez le programme\n");
      exit (1);
    }
    
  }
  else if(connect==2){
    create_account(p);
  } 
}

/*manages the passwords and launches the function that manages the logins*/

int connection(){  
  FILE* f;
  int pos;
  char chaine[TAILLE_MAX];
  int tmp=0;
  int i=0;
  char password[TAILLE_MAX];
  int length;

  f=fopen("base_de_donnee_personnes.txt","r");
  if (f==NULL){
    printf("probleme à l'ouverture d'un fichier\n");
    printf("relancez le programme\n");
    exit (1);
  }
  pos = search_personnes();
  printf ("Password:\n");
  scanf("%s",password);
  clearBuffer();
 
  while(tmp!=pos+2){
    fgets(chaine,TAILLE_MAX,f);
    tmp++;
  }

  length=strlen(password);

  while(i<length){
    if (chaine[i]==*(password+i)){
      i++;
      if(i==length && chaine[i]=='\n'){
        return(pos);/* line jump to do */
      }
    }
    else{
        i=length;
      }
  }
  
  printf ("Error: password\n\n");
  printf("relancez le programme\n");
  exit(1);
} 

/*checks the login and returns the corresponding line in the file*/

int search_personnes(){

  FILE *f = NULL;
  int tmp2=0;
  int line=0;
  int i;
  char tmp[TAILLE_MAX];
  char chaine[TAILLE_MAX];
  int length=0;
  char* id;
  
  id=malloc(sizeof(char*));
  
  printf("Login:\n");
  lire(id,TAILLE_MAX);
  
  length=strlen(id);
  
  f = fopen("base_de_donnee_personnes.txt","r+");
  if( f == NULL ){
    printf("probleme à l'ouverture d'un fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  
  while(tmp2 != 1){
    i=0;
    fgets(tmp,TAILLE_MAX,f);
    while(i<length){
      if (tmp[i]==*(id+i)){
        i++;
        if(i==length && tmp[i]=='\n'){
          return(line); /* renvoie le nombre de ligne a sauter*/
        }
      }
      else{
        i=length;
      }
    }
    fgets(chaine,TAILLE_MAX,f);
    fgets(chaine,TAILLE_MAX,f);
    line=line+3;
    if(fgetc(f)==EOF){
      printf("error login\n\n");
      printf("relancez le programme\n");
      exit(1);
      }
    fseek(f,-1,SEEK_CUR);
  }
}

/*calculates the number of lines in the file "base_de_donnee_livre.txt"*/

int calcule_ligne(){
  FILE* f;
  int c;
  int nLignes = 0;
  
  f=fopen("base_de_donnee_livre.txt","r");
  if( f == NULL ){
    printf("probleme à l'ouverture d'un fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  
  while((c=fgetc(f)) != EOF){
	  if(c=='\n'){
		  nLignes++;
    }  
  }
  fclose(f);
  return(nLignes);
  }

/*look if the login exists in the file*/

char* search_creator_personnes(){

  FILE *f = NULL;
  int tmp2=0;
  int line=0;
  int i;
  char tmp[TAILLE_MAX];
  char chaine[TAILLE_MAX];
  int length=0;
  char* id;
  
  id=malloc(sizeof(char*));
  
  printf("Login:\n");
  lire(id,TAILLE_MAX);
  
  length=strlen(id);
  
  f = fopen("base_de_donnee_personnes.txt","r+");
  if( f == NULL ){
    printf("probleme à l'ouverture d'un fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  
  while(tmp2 != 1){
    i=0;
    fgets(tmp,TAILLE_MAX,f);
    while(i<length){
      if (tmp[i]==*(id+i)){
        i++;
        if(i==length && tmp[i]=='\n'){
          printf("Error: existing account\n\n");
          printf("relancer le programme\n");
          exit(1);
        }
      }
      else{
        i=length;
      }
    }
    
    fgets(chaine,TAILLE_MAX,f);
    fgets(chaine,TAILLE_MAX,f);
    line=line+3;
    if(fgetc(f)==EOF){
      return id; /*return name of new account*/
      }
    fseek(f,-1,SEEK_CUR);
  }
}

/*written in the file "base_de_donnee_livre_emprunter.txt" the title of the book, the person's ID and the time when he/she has to return it for the teacher*/

int borrow_book_teacher(char* id){
  FILE* file1=NULL;
  FILE* file2=NULL;
  char chaine[TAILLE_MAX];
  char chaine1[TAILLE_MAX];
  char chaine2[TAILLE_MAX];
  char chaine3[TAILLE_MAX];
  char date[64];
  int nb_livre=0;
  int nb_ligne=0;
  int num_livre=0;
  int i=0,n=0,tmp=0;
  
  nb_ligne=calcule_ligne();
  nb_livre=nb_ligne/4;
  
  file1 = fopen("base_de_donnee_livre.txt","r");
  if(file1==NULL){ /*problème avec l'ouverture du fichier*/
    printf("problème d'ouverture de fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  
  fseek(file1,2,SEEK_SET);/* saute la première ligne pour aller la ligne des titre de livre*/
  
  for(i=1;i<=nb_livre;i++){
    fgets(chaine1,TAILLE_MAX, file1);/*recupère le titre du livre*/
    fgets(chaine2,TAILLE_MAX, file1);/*recupère l'auteur du livre*/
    printf("-%d: %s %s\n",i,chaine1 ,chaine2);
    fgets(chaine,TAILLE_MAX, file1);/*saut de ligne*/
    fgets(chaine,TAILLE_MAX, file1);
    n=i;
  }

  do{ 
    printf("Choisir un livre\n");
    scanf("%d",&num_livre);
    clearBuffer();
  }while(num_livre<1 || num_livre>n);
  
  tmp=borrowable((num_livre-1)*4);
  if(tmp!=-1){
    printf("ce livre n'est plus disponible\n");
    fclose(file1);
    return 1;
  } 
  
  file2=fopen("base_de_donnee_livre_emprunter.txt","a+");
  if(file2==NULL){ /*problème avec l'ouverture du fichier*/
    printf("problème d'ouverture de fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  
  fseek(file1,0,SEEK_SET);/*ce place au debut du fichier*/
  for(i=0;i<(num_livre-1)*4;i++){ /*saut des lignes*/
    fgets(chaine,TAILLE_MAX,file1);
  }
  fgets(chaine,TAILLE_MAX,file1);/*saut de ligne*/
  fgets(chaine3,TAILLE_MAX,file1);
  fprintf(file2,"%s",chaine3);/*title of book*/
  fprintf(file2,"%s",id);/*login */
  
  time_t now = time(NULL)+180;
  struct tm * tm = localtime(&now); 
  strftime(date, sizeof date, "%H:%M %A, %d %B", tm);
  fprintf(file2, "%s\n", date);/*date et heure a laquelle rendre le livre*/
  
  fclose(file1);
  fclose(file2);
}

/*allows to know which books are to be returned and time of return*/

int search_book_borrow(char *id, int line){
  FILE *f = NULL;
  int i=0;
  char tmp[TAILLE_MAX];
  char chaine[TAILLE_MAX];
  int length=0;
  i=line;

  f = fopen("base_de_donnee_livre_emprunter.txt","r");
  if( f == NULL ){
    printf("probleme à l'ouverture d'un fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  while(i!=0){
    fgets(tmp,TAILLE_MAX,f);
    i--;
  }
  fgets(tmp,TAILLE_MAX,f);
    
  length=strlen(id);
  line++;
  
  while(0 != 1){
    fgets(tmp,TAILLE_MAX,f);
    i=0;
    while(i<length){
      if (tmp[i]==*(id+i)){
        i++;
        if(i==length-1 && tmp[i]=='\n'){
          return(line); /* renvoie le nombre de ligne a sauter*/
        }
      }
      else{
        i=length;
      }
    }
    
    fgets(chaine,TAILLE_MAX,f);
    fgets(chaine,TAILLE_MAX,f);
    line=line+3;
    if(fgetc(f)==EOF){
      return (-3);
    }
    fseek(f,-1,SEEK_CUR);
  }
}

/*written in the file "base_de_donnee_livre_emprunter.txt" the title of the book, the person's ID and the time when he/she has to return it for the student*/

int borrow_book_student(char* id){
  FILE* file1=NULL;
  FILE* file2=NULL;
  char chaine[TAILLE_MAX];
  char chaine1[TAILLE_MAX];
  char chaine2[TAILLE_MAX];
  char chaine3[TAILLE_MAX];
  char date[64];
  int nb_livre=0;
  int nb_ligne=0;
  int num_livre=0;
  int i=0,n=0,tmp=0;
  
  nb_ligne=calcule_ligne();
  nb_livre=nb_ligne/4;
  
  file1 = fopen("base_de_donnee_livre.txt","r");
  if(file1==NULL){ /*problème avec l'ouverture du fichier*/
    printf("problème d'ouverture de fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  
  fseek(file1,2,SEEK_SET);/* saute la première ligne pour aller la ligne des titre de livre*/
  
  for(i=1;i<=nb_livre;i++){
    fgets(chaine1,TAILLE_MAX, file1);/*recupère le titre du livre*/
    fgets(chaine2,TAILLE_MAX, file1);/*recupère l'auteur du livre*/
    printf("-%d: %s %s\n",i,chaine1 ,chaine2);
    fgets(chaine,TAILLE_MAX, file1);/*saut de ligne*/
    fgets(chaine,TAILLE_MAX, file1);
    n=i;
  }

  do{ 
    printf("Choisir un livre\n");
    scanf("%d",&num_livre);
    clearBuffer();
  }while(num_livre<1 || num_livre>n);

  tmp=borrowable((num_livre-1)*4);
  if(tmp!=-1){
    printf("ce livre n'est plus disponible\n");
    fclose(file1);
    return 1;
  } 
  
  file2=fopen("base_de_donnee_livre_emprunter.txt","a+");
  if(file2==NULL){ /*problème avec l'ouverture du fichier*/
    printf("problème d'ouverture de fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
  
  fseek(file1,0,SEEK_SET);/*ce place au debut du fichier*/
  for(i=0;i<(num_livre-1)*4;i++){ /*saut des lignes*/
    fgets(chaine,TAILLE_MAX,file1);
  }
  fgets(chaine,TAILLE_MAX,file1);/*saut de ligne*/
  fgets(chaine3,TAILLE_MAX,file1);
  fprintf(file2,"%s",chaine3);/* titre du livre*/
  fprintf(file2,"%s",id);/*login de la personne*/
  
  time_t now = time(NULL)+120;
  struct tm * tm = localtime(&now); 
  strftime(date, sizeof date, "%H:%M %A, %d %B", tm);
  fprintf(file2, "%s\n", date);/*date et heure a laquelle rendre le livre */
  
  fclose(file1);
  fclose(file2);
  return 0;
}

/*look in the file "base_de_donnee_livre_emprunter.txt" if the book is already in it*/

int search_book_borrow_taking_book(char id[], int line){
  FILE *f = NULL;
  int i=0;
  char tmp[TAILLE_MAX];
  char chaine[TAILLE_MAX];
  int length=0;

  f = fopen("base_de_donnee_livre_emprunter.txt","r");
  if( f == NULL ){
    printf("probleme à l'ouverture d'un fichier\n");
    printf("relancez le programme\n");
    exit(1);
  }
    
  length=strlen(id)-1;
  
  while(0 != 1){
    fgets(tmp,TAILLE_MAX,f);
    i=0;
    while(i<length){
      if (tmp[i]==*(id+i)){
        i++;
        if(i==length && tmp[i]=='\n'){
          return(line); /* renvoie le nombre de ligne a sauter*/
        }
      }
      else{
        i=length;
      }
    }
    
    fgets(chaine,TAILLE_MAX,f);
    fgets(chaine,TAILLE_MAX,f);
    line=line+3;
    if(fgetc(f)==EOF){
      return (-1);
      }
    fseek(f,-1,SEEK_CUR);
  }
}

/*look in the file "base_de_donnee_livre.txt" if the book is already in it*/

int borrowable(int nligne){
  char chaine[TAILLE_MAX];  
  int i=0;
  FILE* f1=NULL;
  
  f1=fopen("base_de_donnee_livre.txt","r"); 

  for(i=0;i<=nligne+1;i++){ /*va chercher le livre dans la bibliothèque*/
    fgets(chaine,TAILLE_MAX,f1);
  }
  i=search_book_borrow_taking_book(chaine,0); 
  return(i);
}

