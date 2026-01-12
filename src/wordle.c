#include "wordle.h"

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

p_WordleGame readFile(const char* fileName, uint32_t maxWords)
{
    FILE *wordFile = fopen(fileName, "r");
    if (!wordFile) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    p_WordleGame game = malloc(sizeof(WordleGame_t));
    if (!game) {
        perror("Failed to allocate WordleGame");
        exit(EXIT_FAILURE);
    }
    memset(game, 0, sizeof(WordleGame_t));

    char buf[NUMBER_CHAR + 2]; // safe buffer
    uint32_t i = 0;

    while (i < maxWords && fgets(buf, sizeof(buf), wordFile))
    {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';

        strncpy(game->arr[i], buf, NUMBER_CHAR);
        game->arr[i][NUMBER_CHAR] = '\0';
        i++;
    }

    game->n = i;
    fclose(wordFile);
    return game;
}


void randomWord(p_WordleGame game)
{
    if (game->n == 0) return; // prevent crash

    uint8_t idx = rand() % game->n;

    if (game->correctWord) free(game->correctWord); // avoid memory leak
    game->correctWord = malloc(NUMBER_CHAR + 1);
    strncpy(game->correctWord, game->arr[idx], NUMBER_CHAR);
    game->correctWord[NUMBER_CHAR] = '\0';
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

bool inputGuessWord(p_WordleGame WordleGame) {
    if (!WordleGame) return false;

    char buf[128]; // big enough for any user input
    if (!fgets(buf, sizeof(buf), stdin)) return false;

    size_t len = strlen(buf);

    // Remove trailing newline
    if (len > 0 && buf[len-1] == '\n') {
        buf[len-1] = '\0';
        len--;
    }

    // Check if input length is exactly NUMBER_CHAR
    if (len != NUMBER_CHAR) return false;

    // Copy safely
    strncpy(WordleGame->guessWord, buf, NUMBER_CHAR);
    WordleGame->guessWord[NUMBER_CHAR] = '\0';

    return true;
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

void gameLoop() {
    p_WordleGame WordleGame = readFile(FILENAME, 100);
    if (!WordleGame) {
        printf("Failed to load word list.\n");
        return;
    }

    randomWord(WordleGame);
    WordleGame->failedCount = 5;

    while (WordleGame->failedCount) {
        printf("\nEnter your guess word: ");
        inputGuessWord(WordleGame);

        while (!validInput(WordleGame->guessWord)) {
            printf("\nInput guess word not valid (5 characters)!\nPlease enter again: ");
            inputGuessWord(WordleGame);
        }

        checkWord(WordleGame->guessWord, WordleGame);
        prompt(WordleGame);
        printResult(WordleGame->result, WordleGame->guessWord, WordleGame->correctWord);

        if (checkRight(WordleGame->result)) {
            printf("\nYou are right!\n");
            break;
        } else {
            WordleGame->failedCount--;
            if (WordleGame->failedCount == 0) {
                printf("\nYou lose! The correct word is '%s'.\n", WordleGame->correctWord);
                break;
            } else {
                printf("\nWrong answer! You have %d attempt(s) left.\n", WordleGame->failedCount);
            }
        }
    }

    // Free dynamically allocated memory
    free(WordleGame->correctWord);
    free(WordleGame);
}
