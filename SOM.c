#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int row_size = 150;
int col_size = 4;

typedef struct node
{
    double *w;
    double activ;
    char *etiquet;
}node;

typedef struct vecteur
{
    double *x;
    double norme;
    char *id;
}vecteur;

typedef struct bmu
{
    int indice_l;
    int indice_c;
    struct bmu *next;
}bmu;

typedef struct N_G
{
    int ligne;
    int colonne;
    node **matrice;
    bmu *bmu;
}N_G;

// fonction créée pour récupérer les données du fichier iris.data et les sauvgarder dans un tableau de vecteur
vecteur *recuperer(vecteur *vect){ 
    int i = 0 ,j = 0;
    char tab[60];
    char *str;
    FILE *f = fopen("iris.data", "r"); 
    if (!f){
        printf("fichier n'existe pas");
    }
    
    while ((fgets(tab, 60, f) != NULL) &&  i<row_size){  
        j = 0;
        str = strtok(tab, ",");
        while(j < col_size){
            vect[i].x[j] = atof(str);
            str = strtok(NULL, ",");
            j++;
        }
        vect[i].id = strdup(str); // on copie la chaine avec '\n' 
        vect[i].id[strlen(vect[i].id) -1]='\0'; // on enlève le retour à la ligne '\n' 
        
        i++;
    }
    fclose(f);
    return vect;
}

void affiche_vecteur(vecteur *vect)
{
    for (int i = 0; i < row_size; i++)
    {
        printf("vecteur:%d ", i);
        for (int j = 0; j < 4; j++)
        {
            printf("%f ", vect[i].x[j]);
        }
        printf("%s ", vect[i].id);
        printf("%f", vect[i].norme);
        printf("\n");
    }
}

// fonction qui prends le tableau de vecteur et ajoute la norme à chaque vecteur de ce tableau
void norm(vecteur *vect)
{ 
    double distance = 0;
    for (int i = 0; i < row_size; i++)
    {
        distance = 0;
        for (int j = 0; j < col_size; j++)
        {
            distance += vect[i].x[j] * vect[i].x[j];
        }
        vect[i].norme = sqrt(distance);
    }
}

void normalisation(vecteur *vect)
{
    for (int i = 0; i < row_size; i++)
    {
        for (int j = 0; j < col_size; j++)
        {
            vect[i].x[j] = vect[i].x[j] / vect[i].norme;
        }
    }
}
// fonction qui calcule la moyenne (on en aura besoin pour remplir la matrice aprés)
double *moyenne(vecteur *vect)
{
    double *moyen_tab = (double *)calloc(col_size, sizeof(double));
    if (moyen_tab == NULL)
    {
        printf("null");
    }
    for (int j = 0; j < col_size; j++)
    {
        for (int i = 0; i < row_size; i++)
        {
            moyen_tab[j] = moyen_tab[j] + vect[i].x[j];
        }
        moyen_tab[j] = moyen_tab[j] / row_size;
    }
    return moyen_tab;
}

int *melanger_indice(int *tab_indice)
{

    int random = 0;
    int temp = 0;
    srand(time(NULL));
    for (int i = 0; i < row_size; i++)
    {
        tab_indice[i] = i;
    }
    for (int i = 0; i < row_size; i++)
    {
        random = rand() % row_size;
        temp = tab_indice[i];
        tab_indice[i] = tab_indice[random];
        tab_indice[random] = temp;
    }
    return tab_indice;
}

void init(N_G *network, vecteur *vect)
{
    //initialisation du vecteur

    for (int i = 0; i < row_size; i++)
    {
        ///allocation mémoires aux 4 éléments du vecteur
        vect[i].x = (double *)malloc(col_size*sizeof(double));
        if (vect[i].x == NULL)
        {
            printf("1");
        }

        
        vect[i].id = (char *)calloc(16, sizeof(char));
        if (vect[i].id == NULL)
        {
            printf("2");
        }
    }

    //initialisation du réseau
    network->ligne = 10;
    network->colonne = 6;
    network->bmu = (bmu *)calloc(row_size, sizeof(bmu));
    if (network == NULL)
    {
        printf("3");
    }

    network->matrice = (node **)calloc(network->ligne, sizeof(node));
    if (network->matrice == NULL)
    {
        printf("4");
    }
    for (int i = 0; i < network->ligne; i++)
    {
        network->matrice[i] = (node *)calloc(network->colonne, sizeof(node));
        if (network->matrice[i] == NULL)
        {
            printf("5");
        }

        for (int j = 0; j < network->colonne; j++)
        {
            network->matrice[i][j].w = (double *)calloc(col_size, sizeof(double));
            if (network->matrice[i][j].w == NULL)
            {
                printf("6");
            }
        }
    }
}


N_G *remplir_matrice(N_G *network, vecteur *vect)
{
    double *moyen_tab = moyenne(vect);
    for (int i = 0; i < network->ligne; i++)
    {
        for (int j = 0; j < network->colonne; j++)
        {
            for (int k = 0; k < col_size; k++)
            {
                network->matrice[i][j].w[k] = moyen_tab[k] - 0.02 + (double)rand() / (double)RAND_MAX * (moyen_tab[k] + 0.02 - moyen_tab[k] + 0.02);
            }
            network->matrice[i][j].activ = 0;
            network->matrice[i][j].etiquet = "***";
        }
    }
    return network;
}

void affiche_matrice(N_G *network)
{
    for (int i = 0; i < network->ligne; i++)
    {
        for (int j = 0; j < network->colonne; j++)
        {
            if(strcmp(network->matrice[i][j].etiquet ,"***")== 0){
                printf("\033[0;30m");
                printf("%s ",network->matrice[i][j].etiquet); 
                printf("\033[0m");
            }
            if(strcmp(network->matrice[i][j].etiquet ,"ver")== 0){
                printf("\x1B[31m");
                printf("%s ",network->matrice[i][j].etiquet); 
                printf("\x1B[0m"); 
            }
            if(strcmp(network->matrice[i][j].etiquet ,"vir")== 0){
                printf("\x1B[34m");
                printf("%s ",network->matrice[i][j].etiquet); 
                printf("\x1B[0m");
            }
            if(strcmp(network->matrice[i][j].etiquet ,"set")== 0){
                printf("\x1B[32m");
                printf("%s ",network->matrice[i][j].etiquet); 
                printf("\x1B[0m");
            }
        }
        printf("\n");
    }
    
}

double distance_eucledienne(double *x, double *w)
{
    double distance=0;
    for (int i = 0; i < col_size; i++)
    {
        distance = distance + (w[i] - x[i])*(w[i] - x[i]);
    }
    return sqrt(distance);
}

bmu *insertion(bmu *liste, int ligne, int colonne)
{
    bmu *nouveau = malloc(sizeof(*nouveau));
    if (nouveau == NULL)
    {
        printf("null");
    }
    nouveau->indice_l = ligne;
    nouveau->indice_c = colonne;
    nouveau->next = NULL;

    liste->next = nouveau;
    return liste;
}

void delete_bmu(bmu *liste) {
    if (liste) {
        bmu *elementASupprimer = liste;
        liste = liste->next;
        free(elementASupprimer);
    }
}

double distance_min(N_G *network,vecteur vect)
{   
    double distance = 100;
    for (int i = 0; i < network->ligne; i++)
    {
        for (int j = 0; j < network->colonne; j++)
        {
            if( distance_eucledienne(network->matrice[i][j].w,vect.x)  < distance )
            {
                distance = distance_eucledienne(network->matrice[i][j].w,vect.x);
            }
        }
        
    }
    return distance;
}

int bmu_taille(bmu *liste)
{
    int taille = 0;
    while(liste != NULL)
    {
        taille++;
        liste = liste->next;
    }
    return taille;
}


void trouve_bmu(N_G *network,vecteur vect,int nb)
{
    bmu * temp = (bmu*)malloc(sizeof(bmu));
    bmu * liste = temp;
    double min = 100;
    int ran = 0;
    for (int i = 0; i < network->ligne; i++)
    {   
        for (int j = 0; j < network->colonne; j++)
        {
            network->matrice[i][j].activ = distance_eucledienne(network->matrice[i][j].w,vect.x);
            if( network->matrice[i][j].activ  <= min )
            { 
                if(network->matrice[i][j].activ < min )
                {
                    temp = liste;
                    temp->indice_l = i;
                    temp->indice_c = j;
                    temp->next = NULL;
                }
                else
                {
                    temp->next = (bmu*)malloc(sizeof(bmu));
                    temp=temp->next;
                    temp->indice_l = i;
                    temp->indice_c = j;
                    temp->next = NULL;
                }
                min =  network->matrice[i][j].activ;
                
            }
        }
    }
    
    ran = 1 + rand()%bmu_taille(liste);
   
    temp=liste;
    for(int l=1;l<ran;l++){
        liste=liste->next;
    }

    network->bmu[nb].indice_l = liste->indice_l;
    network->bmu[nb].indice_c = liste->indice_c;
    delete_bmu(liste);
}

void desallouer(N_G *network, vecteur *vect)
{
    for (int i = 0; i < row_size; i++)
    {
        free(vect[i].x);
        free(vect[i].id);
    }
    for (int i = 0; i < network->ligne; i++)
    {
        for (int j = 0; j < network->colonne; j++)
        {
            free(network->matrice[i][j].w);
        }
        free(network->matrice[i]);
    }
    free(network->matrice);
    free(vect);
    free(network);
}

void modifier_etiquet(N_G *network,vecteur vect,int nb)
{
    if(strcmp(vect.id ,"Iris-setosa") == 0)
    {
        network->matrice[network->bmu[nb].indice_l][network->bmu[nb].indice_c].etiquet = "set";
    }
    if(strcmp(vect.id ,"Iris-versicolor") == 0)
    {
        network->matrice[network->bmu[nb].indice_l][network->bmu[nb].indice_c].etiquet = "ver";
    }
    if(strcmp(vect.id ,"Iris-virginica") == 0)
    {
        network->matrice[network->bmu[nb].indice_l][network->bmu[nb].indice_c].etiquet = "vir";
    }
}

void apprentissage(N_G *network,vecteur *vect, int iteration , double alpha0,int rayon)
{
    int *tab_indice = (int*)malloc(row_size *sizeof(int));
    double alpha =0;
    for(int t=0; t<iteration; t++)
    {
        alpha = alpha0*(1-(double)(t/iteration));

        tab_indice = melanger_indice(tab_indice);
        for(int i=0;i<row_size;i++ )
        {
            trouve_bmu(network,vect[tab_indice[i]],i);
            modifier_etiquet(network,vect[tab_indice[i]],i);
               
            for(int ligne=0;ligne<network->ligne;ligne++)
            {
                for(int colonne=0;colonne<network->colonne;colonne++)
                {
                    if(  ((network->bmu[i].indice_l-rayon)<=ligne)  && (ligne<=(network->bmu[i].indice_l+rayon)) && ((network->bmu[i].indice_c-rayon)<=colonne) && (colonne<=(network->bmu[i].indice_c+rayon)) ){
                        for(int k=0;k<4;k++){
                            network->matrice[ligne][colonne].w[k] = network->matrice[ligne][colonne].w[k] + alpha * (vect[tab_indice[i]].x[k] - network->matrice[ligne][colonne].w[k]);
                        }
                    }
                }
            }
        }
        if( rayon>1 && (iteration == 166 || iteration == 332))
        {
            rayon--;
        }
    }
    free(tab_indice);
}

int main()
{
    
    vecteur *vect = (vecteur *)calloc(row_size, sizeof(vecteur));
    if (vect == NULL)
    {
        printf("null");
    }


    N_G *network = (N_G *)malloc(sizeof(N_G));
    if (network == NULL)
    {
        printf("null");
    }


    init(network, vect);

    vect = recuperer(vect);

    norm(vect);

    normalisation(vect);

    network =remplir_matrice(network, vect);
    
    apprentissage(network,vect,500,0.9,3);

    apprentissage(network,vect,1500,0.07,1);

    affiche_matrice(network);

    desallouer(network,vect);



    return 0;
}