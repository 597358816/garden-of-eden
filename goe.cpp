#include"goe.h"

int powerOfTwo(int num) {
	int ans = 1;
	int i;
	for (i = 0; i < num; i++) {
		ans *= 2;
	}
	return ans;
}

int* getRules(int* rules, int num) {
	int i;
	for (i = LENGTH - 1; i >= 0; i--) {
		rules[i] = num % 2;
		num /= 2;
	}
	return rules;
}

GOETreeBinary* initGOENode() {
	GOETreeBinary* node = (GOETreeBinary*)malloc(sizeof(GOETreeBinary));
	int i;
	for (i = 0; i < LENGTH; i++) {
		node->configuration[i] = 0;
	}
	node->father = NULL;
	node->son[0] = NULL;
	node->son[1] = NULL;
	return node;
}

int recordConfiguration(int* configuration, GOEQueue* queue) {
	GOEQueueNode* p = queue->first->next;
	int i;
	while (p != queue->head) {
		for (i = 0; i < LENGTH; i++) {
			if (configuration[i] != p->element->configuration[i]) {
				break;
			}
		}
		if (i == LENGTH) {
			//0指该状态已经存在
			return 0;
		}
		else {
			p = p->next;
		}
	}
	return 1;
}

GOETreeBinary* createGOERoot(int* rules, FILE* file) {
	GOETreeBinary* root = initGOENode();
	int i;
	for (i = 0; i < LENGTH; i++) {
		//rules[5] = 1指 101 映射到 1
		//if (rules[i] == 1) {
			//例：configuration[7] = 1 指该节点中有111这一状态
			//例：configuration[6] = 0 指该节点中无110这一状态
		//	root->configuration[i] = 1;
		//}
		//else if (rules[i] == 0) {
			root->configuration[i] = 1;
		//}
	}
	//root->configuration[0] = 1;
	//root->configuration[1] = 1;
	//使用队列实现层序遍历
	GOEQueue* queue = initGOEQueue();
	pushGOEQueue(queue, root);
	//此时队列中有root
	while (queue->head != queue->tail) {
		GOETreeBinary* temp = popGOEQueue(queue);
		//如果途中出现GOE或者其他不符合的情况，直接返回
		if (!createGOETree(rules, temp, queue, file)) {
			destroyGOEQueue(queue);
			return root;
		}
	}
	//构造树完毕
	destroyGOEQueue(queue);
	for (i = 0; i < LENGTH; i++) {
		fprintf(file, "%d", rules[i]);
		printf("%d", rules[i]);
	}
	printf("\n");
	fprintf(file, "\n");
	return root;
}

GOETreeBinary* createGOETree(int* rules, GOETreeBinary* node, GOEQueue* queue, FILE* file) {
	int i;
	//如果该块为空，则存在GOE
	int judge = 0;
	for (i = 0; i < LENGTH; i++) {
		if (node->configuration[i] == 1) {
			judge = 1;
		}
	}
	if (judge == 0) {
		//空块返回
		//输出路径
		/*
		GOETreeBinary* p = node;
		while (p->father) {
			if (p == p->father->son[0]) {
				fprintf(file, "0");
			}
			else if (p == p->father->son[1]) {
				fprintf(file, "1");
			}
			p = p->father;
		}
		fprintf(file, "#");
		*/

		return nullptr;
	}
	//*
	if (node->father && node->father->father) {
		for (i = 0; i < LENGTH; i++) {
			if (node->configuration[i] == 1 && findLoopConfiguration(node->father, i, i)) {
				break;
			}
		}
		if (i == LENGTH) {
			return nullptr;
		}
	}
	//*/



	//如果与已知状态相同，则不创建树，什么也不做
	//如果该状态不存在，则将其子节点加入链表并且创建树
	if (recordConfiguration(node->configuration, queue) == 1) {
		GOETreeBinary* zero = initGOENode();
		GOETreeBinary* one = initGOENode();
		zero->father = node;
		one->father = node;
		for (i = 0; i < LENGTH; i++) {
			if (node->configuration[i] == 1) {
				int index = (i % (LENGTH / 2)) * 2;
				if (rules[index] == 0) {
					zero->configuration[index] = 1;
				}
				else if (rules[index] == 1) {
					one->configuration[index] = 1;
				}
				index++;
				if (rules[index] == 0) {
					zero->configuration[index] = 1;
				}
				else if (rules[index] == 1) {
					one->configuration[index] = 1;
				}
			}
		}
		node->son[0] = zero;
		node->son[1] = one;
		pushGOEQueue(queue, zero);
		pushGOEQueue(queue, one);
	}
	return node;
}

GOEQueue* initGOEQueue()
{
	GOEQueue* queue = (GOEQueue*)malloc(sizeof(GOEQueue));
	GOEQueueNode* node = (GOEQueueNode*)malloc(sizeof(GOEQueueNode));
	node->next = NULL;
	//head和tail实现了队列的操作。
	//first记录了队列的第一个元素（被删除），这样即便该元素从队列中移除也可以通过first找到
	queue->first = node;
	queue->head = node;
	queue->tail = node;
	queue->size = 0;
	return queue;
}


void pushGOEQueue(GOEQueue* queue, GOETreeBinary* elem)
{
	GOEQueueNode* node = (GOEQueueNode*)malloc(sizeof(GOEQueueNode));
	node->element = elem;
	queue->tail->next = node;
	queue->tail = node;
}

GOETreeBinary* popGOEQueue(GOEQueue* queue)
{
	if (queue->head->next)
	{
		GOEQueueNode* temp = queue->head->next;
		queue->head = queue->head->next;
		return temp->element;
	}
	else
	{
		return nullptr;
	}

}
void destroyGOEQueue(GOEQueue* queue) {
	GOEQueueNode* p = queue->first;
	if (p->next) {
		GOEQueueNode* tmp = p;
		p = p->next;
		free(tmp);
	}
	free(queue);
}
void destroyGOETree(GOETreeBinary* node) {
	//递归销毁树
	if (node->son[0])
		destroyGOETree(node->son[0]);
	if (node->son[1])
		destroyGOETree(node->son[1]);
	free(node);
}

int findLoopConfiguration(GOETreeBinary* node, int input, int origin) {
	int i;
	if (!node->father) {
		if (origin % (LENGTH / 2) == input / 2) {
			//找到了
			return 1;
		}
		else {
			return 0;
		}
	}

	for (i = 0; i < LENGTH; i++) {
		if (node->configuration[i] == 1 && i % (LENGTH / 2) == input / 2) {
			//存在原像 往前找
			//前面找到了，这个返回1
			if (findLoopConfiguration(node->father, i, origin)) {
				return 1;
			}
		}
	}
	return 0;
}