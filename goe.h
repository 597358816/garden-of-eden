#pragma once
#include<stdio.h>
#include<stdlib.h> 
#define LENGTH 16
struct GOETreeBinary {
	int configuration[LENGTH];
	GOETreeBinary* son[2];
	GOETreeBinary* father;
};

struct GOEQueueNode {
	GOETreeBinary* element;
	GOEQueueNode* next;
};

struct GOEQueue {
	int size;
	GOEQueueNode* head;
	GOEQueueNode* tail;
	GOEQueueNode* first;
};
int powerOfTwo(int num);
int* getRules(int *rules,int num);

GOETreeBinary* initGOENode();
int recordConfiguration(int* configuration, GOEQueue* queue);
GOETreeBinary* createGOERoot(int* rules, FILE* file);
GOETreeBinary* createGOETree(int* rules, GOETreeBinary* node, GOEQueue* queue, FILE* file);

GOEQueue* initGOEQueue();
void pushGOEQueue(GOEQueue* queue, GOETreeBinary* elem);
GOETreeBinary* popGOEQueue(GOEQueue* queue);
void destroyGOEQueue(GOEQueue* queue);
void destroyGOETree(GOETreeBinary* node);

int findLoopConfiguration(GOETreeBinary* node, int input, int origin);
