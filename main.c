#include "include/wordle.h"

#define FILENAME "E:\\FPT_documents\\C\\Words.txt"

int main(int, char**);

int main(int agv, char *argv[])
{
    uint8_t failedTime = 5;
    char *guessWord = "";
    char *correctWord = "";
    bool winner = false;
    char exit = 'Y';
    seed();
    p_WordleGame WordleGame = readFile(FILENAME, 100);
    printf("\n");

    do
    {      
        gameLoop(WordleGame);
        printf("\nDo you want to play anymore (Y/N) ?: ");
        scanf("%c", &exit);
        fflush(stdin);
        failedTime = 5;
    }while((exit == 'Y'));

    return 0;
 }
