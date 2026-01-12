#include "include/wordle.h"


int main(int, char**);

int main(int agv, char *argv[])
{
    char choice = 'Y';
    seed();
    
    printf("\n");

    do
    {      
        gameLoop();
        printf("\nDo you want to play anymore (Y/N) ?: ");
        scanf(" %c", &choice);  // note the space before %c to skip whitespace
        while ((getchar()) != '\n');  // clear rest of line
    }while((choice == 'Y'));

    return 0;
 }
