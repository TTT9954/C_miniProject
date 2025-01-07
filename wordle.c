#include "include/wordle.h"

void seed()
{
    int x;
    
    x = getpid();

    srand(x);

    return;
}

uint8_t getLength(char *word)
{
    uint8_t length = 0;

    while((*word) != '\0')
    {
        length++;
        word++;
    }

    return length;
}

void printString(const char str[], uint8_t length)
{
    for(uint8_t i = 0; i < length; i++)
    {
        printf("%c", str[i]);
    }
}

void copyString(char *dst, const char *src, uint8_t length)
{
    memset(dst, 0, length);
    for(uint8_t i = 0;i < length; i++)
    {
        dst[i] = src[i];
    }
}

bool isIn(char guess, char *word)
{

    for(uint8_t i = 0; i < getLength(word); i++)
    {
        if(word[i] == guess) return true;
    }

    return false;

}

uint8_t checkCharacter(char guess, uint8_t idx, p_WordleGame WordleGame)
{
    const char correct = WordleGame->correctWord[idx];

    if(correct == guess)
    {
        //WordleGame->result[idx] = true;
        return RESULT_GREEN;
    }
    else if(isIn(guess, WordleGame->correctWord)) return RESULT_YELLOW;
     
    return RESULT_RED;
}

void checkWord(char *guess, p_WordleGame WordleGame)
{
    uint8_t i = 0;
    if (guess == NULL || WordleGame == NULL || WordleGame->n == 0) {
        return;
    }

    for(i = 0; i < NUMBER_CHAR; i++)
    {
        WordleGame->result[i] = checkCharacter(guess[i], i, WordleGame);                            
    }
    printf("\n"); 
}

bool checkRight(uint8_t result[NUMBER_CHAR])
{
    uint8_t count = 0;
    for(uint8_t i = 0; i < NUMBER_CHAR; i++)
    {
        if(!result[i])
        {
            count++;
        }
    }
    if(count == 5) return true;

    return false;
}

p_WordleGame readFile(const char* fileName, uint32_t numbOfWords)
{
    FILE *wordFile = NULL;
    char *buf = NULL;
    uint8_t i = 0;
    uint8_t bufReadFile_Lenght = 0;
   
    // Dynamic allocate for Word struct
    p_WordleGame WordleGame = (p_WordleGame)malloc(sizeof(WordleGame_t));
    if (WordleGame == NULL) {
        perror("Failed to allocate memory");
        exit(0);
    }

    // Initialize the structure
    WordleGame->n = 0;

    memset((p_WordleGame)WordleGame, 0 , sizeof(WordleGame)*numbOfWords);
    
    buf = (char *)malloc(sizeof(char)); // Dynamic allocate for buf pointer

    // Check whether res is allocated successfully or not
    if(!buf)
    {
        perror("Allocated unsuccessfully !");
        exit(0);
    }

    // open wordfile with READ mode
    wordFile = fopen(fileName, "r");

    // check whether the file is open successfully or not
    if (wordFile == NULL) {
        printf("Word.txt file failed to open.");
    }
    memset(buf, 0, sizeof(char));
    
    // read from file
    while(fgets(buf, 10, wordFile) && (i < numbOfWords))
    {
        memset(WordleGame->arr[i], 0, sizeof(char));
        bufReadFile_Lenght = strlen(buf)-1;
        buf[bufReadFile_Lenght] = 0;
        copyString(WordleGame->arr[i], buf, bufReadFile_Lenght);
        WordleGame->arr[i][bufReadFile_Lenght] = 0;
        i++;
        memset(buf, 0, sizeof(char));        
    }
    (WordleGame->n) = i;
    i = 0;
    // close to file
    fclose(wordFile);

    return WordleGame;
}

void randomWord(p_WordleGame WordleGame)
{
    uint8_t x = 0;
    static char res[NUMBER_CHAR+1];

    x = rand() % WordleGame->n;

    res[0] =  WordleGame->arr[x][0];
    res[1] =  WordleGame->arr[x][1];
    res[2] =  WordleGame->arr[x][2];
    res[3] =  WordleGame->arr[x][3];
    res[4] =  WordleGame->arr[x][4];
    res[5] =  WordleGame->arr[x][5];

    WordleGame->correctWord = res;
}

void prompt(p_WordleGame WordleGame)
{
    for(uint8_t i = 0; i < 5; i++)
    {
        switch (WordleGame->result[i])
        {
        case RESULT_GREEN:
            printf("%c", WordleGame->correctWord[i]);
            break;
        case RESULT_YELLOW:
            printf("-");
            break;
        case RESULT_RED:
            printf("-");
            break;       
        default:
            printf("Unknown: %d", WordleGame->correctWord[i]);
            break;
        }
    }
    printf("\n>>");
    fflush(stdout);
}

void printResult(char res[NUMBER_CHAR], char guess[NUMBER_CHAR], char correct[NUMBER_CHAR])
{
    for(uint8_t i = 0; i < 5; i++)
    {
        switch (res[i])
        {
        case RESULT_GREEN:
            printf("\033[0;32m%c\033[0m", guess[i]);
            break;
        
        case RESULT_YELLOW:
            printf("\033[0;33m%c\033[0m", guess[i]);
            break;
        
        case RESULT_RED:
            printf("\033[0;31m%c\033[0m", guess[i]);
            break;

        default:
            printf("Unknown: %d", res[i]);
            break;
        }
    }
    printf("\n");
}


void inputGuessWord(p_WordleGame WordleGame){
    int size = 0;

    memset(WordleGame->guessWord, 0, 8);
    fgets(WordleGame->guessWord, 8, stdin);
    fflush(stdin);
    size = strlen(WordleGame->guessWord)-1;
    WordleGame->guessWord[size] = 0;
}

bool validInput(char *inputWord)
{
    uint8_t length = getLength(inputWord);

    if(length != 5)
    {
        return BAD_VALID_INPUT;
    }
    else if(!length)
    {
        return NO_SUCH_WORD;
    }
    else return VALID_INPUT;
}

void gameLoop(p_WordleGame WordleGame)
{
    bool res = false;
    bool checkValid = false;
    randomWord(WordleGame);
    WordleGame->failedCount = 5;
    while(WordleGame->failedCount)
    {     
        printf("\nEnter your guess word: ");
        inputGuessWord(WordleGame);    
        fflush(stdin);     
        checkValid = validInput(WordleGame->guessWord);    
        while(!checkValid)
        {
            printf("\nInput guess word not valid ! Please enter again: ");
            inputGuessWord(WordleGame);
            checkValid = validInput(WordleGame->guessWord);
        }
        checkWord(WordleGame->guessWord, WordleGame);
        prompt(WordleGame);
        printResult(WordleGame->result, WordleGame->guessWord, WordleGame->correctWord);
        res = checkRight(WordleGame->result);    
        if(!res)
        {
            WordleGame->failedCount--;             
            printf("\nWrong answer ! You have %d time(s) left.\n", WordleGame->failedCount);
            if(!WordleGame->failedCount)
            {
                printf("\nYou lose ! The correct word is '%s'.\n", WordleGame->correctWord);
                return;
            }
        }
        else 
        {
            printf("You are right!");
            WordleGame->failedCount = 0;
            return;
        }               
    }



}




