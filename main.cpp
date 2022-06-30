#include<stdio.h>
#include"goe.h"
int main() {
	int i;
	FILE* file;
	fopen_s(&file, "answer1.txt", "w+");
	int *rules = (int*)malloc(LENGTH * sizeof(int));

	for (i = 0; i < powerOfTwo(LENGTH); i++) {
		rules = getRules(rules, i);
		GOETreeBinary* root = createGOERoot(rules, file);
		destroyGOETree(root);
	}

	fclose(file);
	printf("结束\n");
	return 0;
}