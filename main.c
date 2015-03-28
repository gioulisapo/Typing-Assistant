/*
 * main.c
 *
 *  Created on: Apr 30, 2014
 *      Author: gioulisapo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <getopt.h>
#include <error.h>
#include <unistd.h>
#include <termios.h>
#include "TrieStruct.h"
#include "HelpFunct.h"
#define MAXLEN 40
#define LINESIZE 128
#define BACKSPACE 0x7f

int main(int argc, char **argv)
{
	int bflag = 0;
	char path[40];
	int c;
	TriePtr p;
	FILE * fp;
//If -b option is given to the program read from input location else read from default location $HOME/.dict the name of the file must be given.
/********************************************************************************************************************************/
	c = getopt(argc, argv, "bs");
	switch (c)
	{
	case 'b':
		bflag = 1;
		break;
	default:
		break;
	}
	if(bflag)
	{
		strcpy(path,argv[2]);
		if(!(fp=fopen(path,"r")))
		{
			fprintf(stderr,"Wrong path.Default path will be used\n");
			bflag = 0;
		}
	}
	if(!bflag)
	{
		sprintf(path,"%s/.dict/dictionary.txt",getenv("HOME"));
		if(!(fp=fopen(path,"r")))
		{
			fprintf(stderr,"File dictionary.txt was not found in default path %s\n",path);
			fprintf(stderr,"Add custom file with -b option");
			exit(1);
		}
	}
/********************************************************************************************************************************/
	p = InitialiseTrie(fp);														//Intialise trie struct from file fp
	int next;																	//The next char will be stored here
	int isuppercase[MAXLEN];													//Matrix for making sure when the first letter is capital
	/* Keep reading one character at a time */									//the word will print like so "Apostolis" if any other
	char word[MAXLEN];															//letter is capital it will print "APOSTOLIS"
	char line[LINESIZE];
	struct winsize w;
	int j = 0;
	TriePtr result;
	while ((next = getchar_silent()) != EOF)
	{
		if (isalpha(next) || ispunct(next))										//Print Normal chars
		{
			if (j >= MAXLEN)													//Make sure words of length above MAXLEN are not accepted
			{
				fprintf(stderr,"\nWe are sorry words larger than 40 characters are unacceptable");
				putchar('\n');
				j = 0;

			}
			if (next>=65&&next<=90)												//Check if letter typed is capital
			{
				isuppercase[j] = 1;												//If so remember that it is capital
				word[j] = tolower(next);										//but store it as lowercase
			}
			else
			{
				isuppercase[j] = 0;												//Else remeber that it's not
				word[j] = next;													//store it as lowercase as it was typed
			}
			j++;																//counter used in word and isuppercase arrays get prepared for next letter
			putchar(next);														//print letter for user to see
			continue;															//go to read next user input
		}

		/* Treat special characters differently */
		switch (next)
		{
		case ' ':																//if user typed space
			if (j == 0)															//if no other letter was typed between 2 spaces
			{
				printf(" ");													//print space do nothing and get next char
				break;
			}
			word[j] = '\0';														//Else word is now complete
			int flag =0;														//Flag=1 if word already exists in trie and in file
			if (TrieSearch(p, &word[0]) == NULL)								//if not
			{
				fp=fopen(path,"a");												//append it to file
				fprintf(fp, "%s\n", word);
				flag=1;
			}
			AddToTrie(&p, word);												//Add word to trie
			if(flag==1)															//If word was just inserted to trie frequency = 0
				AddToTrie(&p, word);											//so increase it to 1
			strcat(line, word);													//add word and space to line
			strcat(line, " ");
			j = 0;																//new word begins!
			putchar(' ');
			break;
		case '\n':
			putchar('\n');														//print \n new line starts!
			line[0] = '\0';
			break;

		case '\t':

			/* Get terminal window size */
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			word[j] = '\0';													//input is finished
			result = TrieSearch(p, &word[0]);								//return result of search in trie
			if (result != NULL)												//if a result was found
			{
				int k = 0;
				while (k < N && result->Frequent[k] != NULL)				//Find how many suggestions are available for current postfix
				{
					k++;
				}
				char tempString[MAXLEN];
				if (k == 1)													//if only one suggestions automatically fill it in
				{
					int k = 0;
					for (TriePtr temp = result->Frequent[0]->Father;
							temp != result; temp = temp->Father)
					{
						tempString[k] = *temp->key;
						k++;
					}
					tempString[k] = '\0';
					reverse(tempString);
					printf("%s ", &tempString[0]);
					strcat(word, tempString);
					if(strlen(line)+strlen(word) >= LINESIZE-1)			//If line will overflow after insertion print enter, empty line and then print word
					{
						printf("\n");
						line[0] = '\0';
					}
					strcat(line, word);
					strcat(line, " ");
					j = 0;
				}
				else													//if not call PrintFrequent
				{
					char temp[MAXLEN];
					PrintFrequent(result,k,temp);
					if (strlen(temp))									//if PrintFrequent didn't return an empty string
					{
						AddToTrie(&p, temp);							//Increase the frequency of the word selected by the user
						int upper = 0;
						for(int i = 1;i<j;i++)							//If any of the letter except the first was typed uppercase
						{												//we assume all the word was intended to be printed as capital
							if(isuppercase[i])
								upper = 1;
						}
						if(upper)
						{
							for(int i = 0; i<strlen(temp); i++){
							  temp[i] = toupper(temp[i]);
							}
						}
						else if(isuppercase[0])							//if Only the first one was typed as capital print only the first letter
							temp[0] = toupper(temp[0]);					//as capital
						if(strlen(line)+strlen(temp) >= LINESIZE-1)		//If line will overflow after insertion print enter, empty line and then print word
						{
							printf("\n");
							line[0] = '\0';
						}
						strcat(line, temp);
						strcat(line, " ");
					}
					printf("\n%s", &line[0]);
					j = 0;
				}
			}
			break;

		case BACKSPACE: /* Just read a backspace */

			/* Move the cursor one step back, print space, and move
			 * cursor back once more to imitate backward character
			 * deletion.
			 */
			if (j > 0)
			{
				j--;													//delete letter from word matrix
				printf("\b \b");
			}
			break;

		case VEOF: /* Just read EOF (Ctrl-D) */

			/* The EOF character is recognized only in canonical input
			 * mode. In noncanonical input mode, all input control
			 * characters such as Ctrl-D are passed to the application
			 * program exactly as typed. Thus, Ctrl-D now produces
			 * this character instead.
			 */

			printf("\n\nExiting. Bye...");
			goto THE_END;
		default:
			continue;
		}
	}
	free(p);
	fclose(fp);
	THE_END: putchar('\n');
	return EXIT_SUCCESS;
}
