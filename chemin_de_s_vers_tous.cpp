#include <stdio.h>
#include <stdlib.h>
#define NB_SOMMET 10
/// structure d une pile dynamique
typedef struct pile
{
    int val;           // le champ de donnee
    struct pile*suivant;//le pointeur suivant

} PPile;
//creation et initialisation du pile
/*-------------------------------------------------------------------
 * creer et initialiser un element d'une pile
 * @param elem un sommet
 * @return une pile empiler crée et initialisée
 */
PPile*creer_initialiser_PPile(int elem)
{
    PPile*sommet=(PPile*)malloc(sizeof(PPile));
    if(!sommet){
        printf("erreur d'allocation de la memoire\n");
        exit(-1);//erreur d'allocation de la memoire
    }
    sommet->suivant=NULL;
    sommet->val=elem;//remplir le champ de donnee
    return((PPile*)sommet);
}
/*-------------------------------------------------------------------
 * empiler une pile
 * @param pile 
 * @param elem un sommet
 * @return une pile empiler
 */
PPile*empiler_PPile(PPile*Mapile,int elem)
{
    PPile*NE;
    NE=creer_initialiser_PPile(elem);
    //si la pile vide ou nexiste pas on va returner l element creer
    if(!Mapile) return((PPile*)NE);
    NE->suivant=Mapile;
    return((PPile*)NE);
}

/*-------------------------------------------------------------------
 * dépiler une pile
 * @param pile remplis par des sommets d'un chemin
 * @return une pile dépilée
 */
PPile* Depiler_PPile(PPile*Mapile)
{
    PPile*elem_supp=Mapile;//pointe sur l'element a supprimer
    if(!Mapile) return((PPile*)NULL);//pile vide ou n existe pas
    Mapile=Mapile->suivant;
    free(elem_supp);
    return((PPile*)Mapile);
}
/*-------------------------------------------------------------------
 * fichier txt remplis par un chemin récursivement
 * @param pile remplis par les sommets d'un chemin
 * @param y un sommet du graphe
 * @param Nom_fichier le nom du fichier txt
 */
void print_pile(PPile*pile,char *Nom_fichier)
{
    PPile*tmp = pile;
    FILE *Mon_fichier ;
    //Ouverture du fichier s'il existe
    Mon_fichier=fopen(Nom_fichier,"a");
    if(!Mon_fichier) {//Ouverture du fichier s'il n'existe pas
        Mon_fichier=fopen(Nom_fichier,"w");
        if(!Mon_fichier) exit(-1);
    }
    if(tmp)
    {
        while(tmp->suivant)
        {
            fprintf(Mon_fichier,"%d <-- ", tmp->val);
            tmp = tmp->suivant;
        }
        fprintf(Mon_fichier,"%d\n", tmp->val);
    }
    fclose(Mon_fichier); // Fermeture du fichier
}
/*-------------------------------------------------------------------
 * fichier txt remplis par un chemin récursivement
 * @param pile remplis par les sommets d'un chemin
 * @param y un sommet du graphe
 * @param Nom_fichier le nom du fichier txt
 */
void print_circuit(int y , PPile*pile,char *Nom_fichier)
{
    PPile*tmp = pile;
    FILE *Mon_fichier ;
    //Ouverture du fichier
    Mon_fichier=fopen(Nom_fichier,"a");
    if(!Mon_fichier) {
        Mon_fichier=fopen(Nom_fichier,"w");
        if(!Mon_fichier) exit(-1);
    }
    fprintf(Mon_fichier,"%d <-- ", y);
    if(tmp)
    {
        while(tmp->suivant)
        {
            fprintf(Mon_fichier,"%d <-- ", tmp->val);
            tmp = tmp->suivant;
        }
        fprintf(Mon_fichier,"%d\n", tmp->val);
    }
    fclose(Mon_fichier); // Fermeture du fichier
}
/*-------------------------------------------------------------------*/
void Enumerer(int MatriceAdj[NB_SOMMET][NB_SOMMET], int s,int nb_sommets,
              int* etat,PPile**chemin,char *Nom_fichier ){
    int y;
    for (y = 0; y < nb_sommets; ++y) {
        if (MatriceAdj[s][y]==1) {
            if (etat[y]==0){
                *chemin= empiler_PPile(*chemin,y);
                print_pile(*chemin,Nom_fichier);
                etat[y]=1;
                Enumerer( MatriceAdj,y,nb_sommets,etat,chemin,Nom_fichier);
            }else{
                print_circuit(y,*chemin,Nom_fichier) ;
            }
        }
    }
    *chemin=Depiler_PPile(*chemin);
    etat[s]=0;
}
/*-------------------------------------------------------------------
 * fichier txt remplis par tous les chemin possible à partir du s
 * @param MatriceAdj matrice d'adjacence
 * @param s un sommet du graphe
 * @param nb_sommets nombre des sommets du graphe
 * @param Nom_fichier le nom du fichier txt
 */
void Enumeration_recursive(int MatriceAdj[NB_SOMMET][NB_SOMMET], int s,int nb_sommets,char*Nom_fichier )
{
    PPile *chemin=NULL;
    int etat[nb_sommets];
    int i;
    for(i=0;i<nb_sommets; etat[i++]=0);
    etat[s]=1;
    chemin= empiler_PPile(chemin,s);
    Enumerer(MatriceAdj,s,nb_sommets,etat,&chemin,Nom_fichier);
}
/*-------------------------------------------------------------------
 * remplir la matrice d'adjacence à partir d'un fichier txt
 * @param Nom_fichier le nom du fichier txt
 * @param MatriceAdj matrice d'adjacence
 * @return la taille de la matrice d'adjacence = nbr de sommets
 */
int remplir_matrice(char *Nom_fichier,int MatriceAdj[NB_SOMMET][NB_SOMMET])
{
    FILE *Mon_fichier ;
    int nb_sommets;//nombre de sommets
    //Ouverture du fichier
    Mon_fichier=fopen(Nom_fichier,"r");
    if(!Mon_fichier) exit(-1);
    // lire le nombre de sommets
    fscanf(Mon_fichier,"%d\n",&nb_sommets);
    int ind=0,ind1=0;
    /*- Tant que la fin du fichier texte n'est pas atteint */
    while( !feof(Mon_fichier))
    {
        fscanf(Mon_fichier,"%d\t",&MatriceAdj[ind][ind1]);
        ind1++;
        //si on arrive a un retour chariot == nb colonne de la matrice
        if(ind1==nb_sommets)
        {
            fscanf(Mon_fichier,"\n");
            ind++;
            ind1=0;
        }
    }
    fclose(Mon_fichier); // Fermeture du fichier
    return nb_sommets;
}
/*-------------------------------------------------------------------
 * afficher la matrice d'adjacence
 * @param MatriceAdj matrice d'adjacence
 * @return l'affichage de la matrice d'adjacence
 */
void print_matrice( int MatriceAdj[NB_SOMMET][NB_SOMMET],int nb_sommets){
    int i,j;
    for(i=0;i<nb_sommets;i++){
        for(j=0;j<nb_sommets;j++)
            printf("%d ",MatriceAdj[i][j]);
        printf("\n");
    }
}
int main(){

    int MatriceAdj[NB_SOMMET][NB_SOMMET];
    int nb_sommets;
    //remplire la matrice et retourner sa taille = nbr sommets
    nb_sommets=remplir_matrice("matrice.txt",MatriceAdj);
    //afficher la matrice
    print_matrice(MatriceAdj,nb_sommets);
    //remplir un fichier txt par tous les chemins a partir  d'un sommet ( 0 )
    Enumeration_recursive(MatriceAdj,0,nb_sommets,"tousLesChemins.txt");
    return 0;
}

