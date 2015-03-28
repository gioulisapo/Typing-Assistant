/*
 * TrieStruct.c
 *
 *  Created on: Apr 30, 2014
 *      Author: gioulisapo
 */
#include "TrieStruct.h"
#include "HelpFunct.h"
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdio.h>
#include <stdio.h>

TriePtr InitialiseTrie(FILE *fp)
{
	TriePtr root;
	root = malloc(sizeof(TrieNode));											//Allocate space for Root
	root->key = "$";
	root->Child = NULL;
	root->Father = NULL;
	char myword[MAXLEN];														//Start Reading file
	while (fscanf(fp, "%s", myword) != EOF)
	{
		AddToTrie(&root, myword);
	}
	fclose(fp);
	return root;
}

void AddToTrie(TriePtr *root, char *w) //Insert word w into tree p
{
	TriePtr pTrav = (*root);													//Use local var in order to avoid changes to existing trie
	if (pTrav->Child == NULL) 													//If reached a point that the letter doesn't exist
	{																			//Start creating Child Nodes until whole word is inserted
		for (pTrav = *root; strlen(w) >= 1; pTrav = pTrav->Child)
		{
			pTrav->Child = TrieCreateNode(w);									//Attach new letter to Trie
			pTrav->Child->Father = pTrav;
			w++;
		}																		//Word is now added to our tree
		pTrav->Leaf->frequency++;												//Increase frequency of leaf for word added(min 0)
		pTrav->Leaf->Father = pTrav;											//Connect Leaf to last letter
		FixFrequency(pTrav->Leaf);
	}
	else if (*w == *pTrav->Child->key)											//If the letter already exists in the order needed
	{
		w++;																	//Check next letter
		if (*w != '\0')															//If the word is not fully inserted in the tree
			AddToTrie(&pTrav->Child, w);										//Call Add for the remaining word
		else																	//If word is fully inserted into tree
		{
			pTrav->Child->Leaf->frequency++;									//Inc frequency of word inserted
			pTrav->Child->Leaf->Father = pTrav->Child;							//Make sure leaf is connected to last letter node
			FixFrequency(pTrav->Child->Leaf);
			return;
		}
	}
	else																		//If the letter does not exist in the required place
	{
		pTrav = pTrav->Child;
		while ((pTrav->Next != NULL))											//Check Next nodes if letter exists
		{
			pTrav = pTrav->Next;
			if (*pTrav->key == *w)												//Letter Found in Next nodes
			{
				w++;
				if (*w == '\0')													//If all letters of the word have been inserted
				{
					pTrav->Leaf->frequency++;									//Inc frequency of word
					pTrav->Leaf->Father = pTrav;								//Make sure leaf is connected to last letter node
					FixFrequency(pTrav->Leaf);
					return;
				}
				AddToTrie(&pTrav, w);											//If word is not fully inserted add remaining letters to trie
				return;
			}
		}
		pTrav->Next = TrieCreateNode(w);										//The programm reaches this point only if the required letter
		pTrav->Next->Father = pTrav->Father;									//was not found in the required place
		w++;																	//So we create a new node for the required letter
		if (*w == '\0')															//If Word is fully inserted into the Trie
		{
			pTrav->Next->Leaf->Father = pTrav->Next;							//Make sure leaf is connected to last letter node
			pTrav->Next->Leaf->frequency++;										//Inc frequencies
			FixFrequency(pTrav->Next->Leaf);
			return;
		}
		AddToTrie(&pTrav->Next, w);												//If word is not fully inserted add remaining letters to trie
	}
	return;
}

TriePtr TrieCreateNode(char* w)
{
	TriePtr node = NULL;
	node = (TriePtr) malloc(sizeof(TrieNode) + 1);
	if (NULL == node)
	{
		printf("Malloc failed\n");
		return node;
	}
	node->key = (char *) malloc(sizeof(char));
	*node->key = *w;
	node->Next = NULL;
	int i;
	for (i = 0; i < N; i++)
		node->Frequent[i] = NULL;												//All pointers to leafs are initialized to NULL
	node->Child = NULL;
	node->Father = NULL;
	node->Leaf = (LeafPtr) malloc(sizeof(LeafNode));
	node->Leaf->frequency = -1;													//Frequency is set to -1 since it will increase by AddToTrie func
	return node;																//at least once
}
;

TriePtr TrieSearch(TriePtr root, const char *key)
{
	TriePtr level = root;
	TriePtr pPtr = NULL;

	while (1)
	{
		TriePtr found = NULL;
		TriePtr curr;
		while (level->Child != NULL)
		{
			if (*level->Child->key == *key)										//If letter is found in a child
			{
				level = level->Child;											//Advance deeper inside the trie
				if (strlen(key) == 1)											//If all letters are found return node of last letter
				{
					return level;
				}
				key++;
			}
			else
				break;

		}
		if (level->Child != NULL && *key != '\0')								//If postfix given is not found completelly
		{
			level = level->Child;
			if (level->Next == NULL) return NULL;								//If no next nodes exist all hope is lost there is no such postfix
			for (curr = level->Next; curr != NULL; curr = curr->Next)			//Else Search next nodes till letter is found
			{
				if (*curr->key == *key)
				{
					found = curr;
					break;
				}
			}
		}
		if (found == NULL) return NULL;											//If I didn't find it in Next nodes all hope is lost NULL
		level = found;															//Else keep searching from the node with key
		key++;																	//same as letter of postfix.
		if (*key == '\0') return level;											//full postfix found return pointer to last letter of Postfix!!
	}
}

void FixFrequency(LeafPtr leaf)
{
	int i = 0;
	TriePtr Father = leaf->Father;												//Last letter word with this leaf
	for (Father; *Father->key != '$'; Father = Father->Father)					//Fix all frequencies till root
	{
		if (Father->Frequent[i] == leaf) continue;								//If leaf is already in the list of most frequent leaves nothing to do here
		for (i = 0; i < N; i++)													//Else start checking the pointers to the most frequent leaves
		{
			if (Father->Frequent[i] == NULL)									//If the i pointer is NULL
			{																	//Now it points to the leaf
				Father->Frequent[i] = leaf;
				break;
			}
			else if (Father->Frequent[i]->frequency < leaf->frequency)			//If pointer is not NULL check if the frequency
			{																	//of the leaf is larger than the ones it pointed to
				Father->Frequent[i] = leaf;										//In case it is insert it in the place of the less
				break;															//frequent ones
			}
		}
	}

}

void PrintFrequent(TriePtr EndNode, int ActiveLeaves, char *result)
{
	LeafPtr leaf;
	char Words[ActiveLeaves][MAXLEN];											//Local Matrix to store most frequent words in order to
	fprintf(stderr, "\nPlease choose a word pressing numbers 1-%d\n",ActiveLeaves);//present them in alphabetical order
	for (int j = 0; j < ActiveLeaves; j++)
	{
		leaf = EndNode->Frequent[j];											//Goto leaf node Frequent[i]
		TriePtr Father;
		char word[MAXLEN];
		int i = 0;
		for (Father = leaf->Father; *Father->key != '$'; Father =Father->Father)//Follow path from leaf to root to get full word
		{
			word[i] = *Father->key;												//Store it in word[]
			i++;
		}
		word[i] = '\0';
		reverse(word);
		strcpy(Words[j], word);													//Copy frequent word to matrix Words
		Words[j][i] = '\0';														//Make sure it ended correctly
	}
	sort(Words,ActiveLeaves);													//Now that all words are available sort them
	for (int i = 0; i < ActiveLeaves; i++)										//Print sorted options
		fprintf(stderr,"%s    ",Words[i]);
	fprintf(stderr,"\n");
	int x = getchar_silent() - 48;												//Get next char from keyboard subtract ASCI code for 1
	if(ActiveLeaves>9)															//If N>9 it is assumed that N will always be smaller
		x=10*x+(getchar_silent()-48);											//than 100 cause of the impracticality of it
	if (x > 0 && x <= ActiveLeaves)												//If it is between the choices given
	{
		strcpy(result,Words[x-1]);												//Return value selected
		return;
	}
	result[0] = '\0';															//Else return empty string
	return;
}
void sort(char words[N][MAXLEN], int ActiveLeafs)
{
	char temp[MAXLEN];
	for (int c = 0; c < ActiveLeafs - 1; c++)
	{
		for (int d = 0; d < ActiveLeafs - c - 1; d++)
		{
			if (0<(strcmp(words[d], words[d + 1]))) 							//For alphabetical order
			{
				strcpy(temp, words[d]);
				strcpy(words[d],words[d + 1]);
				strcpy(words[d + 1],temp);
			}
		}
	}
}

void reverse(char *string)
{
	int length, c;
	char *begin, *end, temp;

	length = strlen(string);

	begin = string;
	end = string;

	for (c = 0; c < (length - 1); c++)
		end++;

	for (c = 0; c < length / 2; c++)
	{
		temp = *end;
		*end = *begin;
		*begin = temp;

		begin++;
		end--;
	}
}

