#include "include/wordle.h"


int main(int, char**);

int main(int agv, char *argv[])
{
    char exit = 'Y';
    seed();
    
    printf("\n");

    do
    {      
        gameLoop();
        printf("\nDo you want to play anymore (Y/N) ?: ");
        scanf("%c", &exit);
        fflush(stdin);
    }while((exit == 'Y'));

    return 0;
 }
