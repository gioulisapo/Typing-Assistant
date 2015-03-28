/*
 * TriesStruct.h
 *
 *  Created on: Apr 30, 2014
 *      Author: gioulisapo
 */
#include <stdio.h>
#ifndef TRIESTRUCT_H_
#define TRIESTRUCT_H_


#define N 4
#define MAXLEN 40
typedef struct lnode *LeafPtr;
typedef struct tnode *TriePtr;
typedef struct lnode
{
	int frequency;
	TriePtr Father;
}LeafNode;
typedef struct tnode
{
	char * key;
	LeafPtr Frequent[N];
	TriePtr Father;
	TriePtr Child;
	TriePtr Next;
	LeafPtr Leaf;
}TrieNode;

void AddToTrie(TriePtr *root, char *w);								//Adds a word to Trie. If word already exists just inc frequency
TriePtr InitialiseTrie(FILE *f);									//Initializes tree with words read from file f
TriePtr TrieCreateNode(char* key);									//Allocates a node with key = w and returns a pointer to it
TriePtr TrieSearch(TriePtr root, const char *key);					//Returns a pointer to last letter (node with key equal to
																	//the last letter) of the word or prefix
void FixFrequency(LeafPtr leaf);									//Runs through the parents of a leaf and makes sure tha the
																	//frequency tables are correct
void PrintFrequent(TriePtr EndNode,int ActiveLeaves,char *result); //result gets the value of the word chosen by the user
																	//the possible choices are taken from the Endnode->Frequent[ActiveLeaves]
void reverse(char *string);											//string is reversed e.g sinnet->tennis
void sort(char words[N][MAXLEN], int ActiveLeafs);					//sorts N arrays of size MAXLEN alphabetically


#endif /* TRIESTRUCT_H_ */
