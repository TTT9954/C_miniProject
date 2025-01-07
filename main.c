#include "include/wordle.h"

#define FILENAME "E:\\FPT_documents\\C\\Words.txt"

int main(int, char**);

int main(int agv, char *argv[])
{
    uint8_t failedTime = 5;
    char *guessWord = "";
    char *correctWord = "";
    bool res = false;
    char exit = 'Y';
    seed();
    p_WordleGame WordleGame = readFile(FILENAME, 100);
    printf("\n");

    do
    {
        randomWord(WordleGame);
        while(failedTime)
        {
            printf("\nEnter your guess word: ");
            inputGuessWord(WordleGame);    
            fflush(stdin);      
            checkWord(WordleGame->guessWord, WordleGame);
            prompt(WordleGame);
            printResult(WordleGame->result, WordleGame->guessWord, WordleGame->correctWord);
            res = checkRight(WordleGame->result);
            if(!res)
            {
                failedTime--;             
                printf("\nWrong answer ! You have %d time(s) left.\n", failedTime);
                if(!failedTime)
                {
                    printf("\nYou lose ! The correct word is '%s'.\n", WordleGame->correctWord);
                }
            }
            else 
            {
                printf("You are right!");
                failedTime = 0;
            }                      
        }
        printf("\nDo you want to play anymore (Y/N) ?: ");
        scanf("%c", &exit);
        fflush(stdin);
        failedTime = 5;
    }while((exit == 'Y'));

    return 0;
 }
