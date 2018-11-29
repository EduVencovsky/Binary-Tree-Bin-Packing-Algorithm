
#include "stdio.h"
#include "stdlib.h"


typedef struct Block
{
	struct Node* fit;
	int width;
	int height;
	int x;
	int y;
	int id;

} Block;

typedef struct Node 
{
	struct Node* down;
	struct Node* right;
	int used;	
	int width;
	int height;
	int x;
	int y;

} Node;

Node *findNode(Node *root, int w, int h);
Node *splitNode(Node **root, int w, int h);

int main()
{
	FILE *file;
	char line[80];
	Block **blocks;	
	int totalBoards, boardWidth, boardHeight, totalBlocks;
	int i = 1, j;
	
	Node *root = malloc(sizeof(Node));
	root->x = 0;
	root->y = 0;
	root->used = 0;

	fopen_s(&file, "blocks.txt", "r");


	while (fgets(line, 80, file)) {
		if (i == 1) {
			sscanf_s(line, "%d\n", &totalBoards);
		} else if (i == 2) {
			sscanf_s(line, "%d\n", &boardWidth);
			root->width = boardWidth;
		} else if (i == 3) {
			sscanf_s(line, "%d\n", &boardHeight);
			root->height = boardHeight;
		} else if (i == 4) {
			sscanf_s(line, "%d\n", &totalBlocks);
			blocks = malloc(totalBlocks * sizeof(Block *));
			
		} else {			
			int w, h;
			blocks[i - 5] = malloc(sizeof(Block));
			sscanf_s(line, "%d %d", &w, &h);
			blocks[i - 5]->width = w;
			blocks[i - 5]->height = h;
			blocks[i - 5]->id = i - 5;
		}

		i++;
	}

	for (i = 0; i < totalBlocks; i++) {
		blocks[i]->id = i;
		for (j = 0; j < totalBlocks - i - 1; j++) {
			if (blocks[j]->width < blocks[j + 1]->width) {
				Block *b = blocks[j];
				blocks[j] = blocks[j + 1];
				blocks[j + 1] = b;
			}
		}
	}

	for (i = 0; i < totalBlocks; i++) {
		Block *block = blocks[i];	
		Node *node;
		if (node = findNode(root, block->width, block->height)) {
			block->fit = splitNode(&node, block->width, block->height);
		}
		printf("next block\n\n");
	}

	for (i = 0; i < totalBlocks; i++) {
		Block *block = blocks[i];
		if (block->fit) {			
			printf("id %d \nx %d y %d\nw %d h %d\n\n", block->id, block->fit->x, block->fit->y, block->width, block->height);
		}
	}

    return 0;
}

Node *findNode(Node *root, int w, int h) {
	if (root->used == 1) {

		printf("root is used \n");
		
		//return findNode(root->right, w, h) || findNode(root->down, w, h);
		Node * rightNode;
		Node * downNode;
		if (rightNode = findNode(root->right, w, h)) {
			printf("root->right \n");
			return rightNode;
		}
		else if (downNode = findNode(root->down, w, h)) {
			printf("root->down \n");
			return downNode;
		}

	}
	else if ((w <= root->width) && (h <= root->height)) {
		printf("return root\n");
		return root;
	}
	else {
		printf("null\n");
		return NULL;
	}
}

Node *splitNode(Node **root, int w, int h) {
	printf("split node\n");

	(*root)->used = 1;
	(*root)->down = malloc(sizeof(Node));
	(*root)->down->right = malloc(sizeof(Node));
	(*root)->down->down = malloc(sizeof(Node));
	(*root)->down->x = (*root)->x;
	(*root)->down->y = (*root)->y + h;
	(*root)->down->width = (*root)->width;
	(*root)->down->height = (*root)->height - h;
	(*root)->down->used = 0;


	(*root)->right = malloc(sizeof(Node));
	(*root)->right->right = malloc(sizeof(Node));
	(*root)->right->down = malloc(sizeof(Node));
	(*root)->right->x = (*root)->x + w;
	(*root)->right->y = (*root)->y;
	(*root)->right->width = (*root)->width - w;
	(*root)->right->height = (*root)->height;
	(*root)->right->used = 0;

	return *root;
}