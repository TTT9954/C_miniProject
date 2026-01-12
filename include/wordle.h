#ifndef _WORDLE_H
#define _WORDLE_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILENAME "/home/tutt/Documents/MyGit/C_miniProject/Words.txt"
#define RESULT_GREEN        0
#define RESULT_YELLOW       1
#define RESULT_RED          2
#define NUMBER_WORD         100
#define NUMBER_CHAR         5
#define MAX_COUNTWORDS      NUMBER_CHAR + 1
#define COLOR_GREEN       "\033[0;32m"
#define COLOR_YELLOW      "\033[0;33m"
#define COLOR_RED         "\033[0;31m"
#define COLOR_DEFAULT     "\033[0m"
#define BAD_VALID_INPUT     false
#define NO_SUCH_WORD        false
#define VALID_INPUT         true

typedef struct WordleGame
{
    char arr[NUMBER_WORD][NUMBER_CHAR+1];
    char guessWord[NUMBER_CHAR+1];
    char *correctWord;
    uint8_t result[NUMBER_CHAR];
    uint8_t failedCount;
    uint8_t n;
}WordleGame_t;

typedef WordleGame_t *p_WordleGame;

void seed();
uint8_t getLength(char *word);
void printString(const char str[], uint8_t length);
void copyString(char *dst, const char *src, uint8_t Length);
bool isIn(char guess, char *word);
uint8_t checkCharacter(char guess, uint8_t idx, p_WordleGame WordleGame);
void checkWord(char *guess, p_WordleGame WordleGame);
bool checkRight(uint8_t result[NUMBER_CHAR]);
p_WordleGame readFile(const char* fileName, uint32_t max);
void randomWord(p_WordleGame WordleGame);
void prompt(p_WordleGame WordleGame);
void printResult(char res[NUMBER_CHAR], char guess[NUMBER_CHAR], char correct[NUMBER_CHAR]);
bool inputGuessWord(p_WordleGame WordleGame);
bool validInput(char *inputWord);
void gameLoop();

#endif
