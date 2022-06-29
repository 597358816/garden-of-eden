#include<stdio.h>
#include"goe.h"
int main() {
	int i, j, k;
	FILE* file;
	fopen_s(&file, "answer1.txt", "w+");
	int *rules = (int*)malloc(LENGTH * sizeof(int));
	for (i = 0; i < LENGTH; i++) {
		rules[i] = 0;
	}

	for (i = 0; i < 256; i++) {
		rules = getRules(rules, i);
		GOETreeBinary* root = createGOERoot(rules, file);
		destroyGOETree(root);
	}
	fclose(file);
	printf("结束\n");
	return 0;
}