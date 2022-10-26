#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 1000
#define NB_THREAD 10

//Tableau qui stocke les valeurs de la fonction de syracuse
int *syracuse;

pthread_mutex_t mutex;


// structure qui stocke le nombre de début du calcul dans le thread
struct Results
{
    int start;           
    
};

/*! 
 *  \brief calcule la conjecture de syracuse
 *  \param [in] n nombre de début du calcul
 *  \param [in] syracuse tableau de stockage des durées de vol
 *  \return la durée de vol
 */
int Collatz(int n, int * syracuse)
{
    int i = 0;
    int startPosition = n;
    
    while (n > 1)
    {
        if (n <= SIZE && syracuse[n] != 0)
        {
            printf("Pour la case n°%d l'amélioration utilisé est à la case n°%d \n", startPosition, n);
            i += syracuse[n];
            break;
        }
        else if (n % 2 == 0) /* n pair */
        {
            n >>= 1; /* n = n / 2 */
        }
        else if (n % 2 != 2)/* n impair */
        {
            n *= 3; /* n = n * 3 + 1*/
            n++;
        }
        i++;
    }
    
    return i;
}

/*! 
 *  \brief Get length of Collatz sequence and update max length
 *  \param args Pointer to Results struct which store parameters for thread function
 *  \see Results
 *  \return 0
 */
void* updateCollatzSequence(void *args)
{
    struct Results *p = (struct Results*) args;
    while ( p->start > 0 )
    {
        pthread_mutex_lock(&mutex);
        const int startPosition = p->start--;
        pthread_mutex_unlock(&mutex);

        int currentLength = Collatz(startPosition, syracuse);
        syracuse [startPosition] = currentLength;
        

        
    }
    return 0;
}

/*! 
 *  \brief main 
 *  \return Return 0 si tout est ok 
 */
int main(void)
{
    printf("conjecture de Syracuse multi-threade\n\n");
    
    syracuse = (int *) malloc((SIZE+1) * sizeof(int));

    struct Results s = {.start = SIZE};

    pthread_t threads[NB_THREAD];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NB_THREAD; i++)
    {
        int status = pthread_create(&threads[i], NULL, updateCollatzSequence, (void*) &s);
        if (status != 0)
        {
            printf("erreur: pas de creation de thread, statut = %d\n", status);
            exit(status);
        }
    }
    for (int i = 0; i < NB_THREAD; i++)
    {
        int status = pthread_join(threads[i], NULL);
        if (status != 0)
        {
            printf("erreur: thread non joignable, statut = %d\n", status);
            exit(status);
        }
    }
    pthread_mutex_destroy(&mutex);

    printf("veuillez entrer l'indice voulu entre 1 et 1000: ");
    int i;
    scanf("%d",&i);
    if (i <= 0 || i > 1000)
    {
      printf("erreur: indice incompatible \n");
      return -1;
    }
    printf("syracuse[%d] : %d\n", i, syracuse[i]);

    free(syracuse);
    return 0;
}


