#define TAILLE_MAX 1000
#define NOMBRE_CAT 23

typedef struct { /*création d'une stucture personne*/
    char* nom_prenom;
    int prof_ou_pas;  /*eleve ou bien prof*/
    char* password;
}Personne;

typedef struct{ /*création d'une stucture livre*/
    int Identifiantlivre;
    char* titre;
    char* auteur;
    int categorie;
}Livre; 

void display();

void clearBuffer();

int lire(char *chaine, int longueur);

void add_book(Livre* livreData);

void create_account(Personne* p);

int search_personnes();

int search_livre_dans_la_base_livre(char *strSearch);

int connection();

int calcule_ligne();

int borrow_book_teacher(char* id);

int borrow_book_student(char* id);

char* search_creator_personnes();

int search_book_borrow(char *id, int line);

int borrowable(int nLigne);

int search_book_borrow_taking_book(char id[],int line);

