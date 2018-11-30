#include <stdio.h>
#include <stdlib.h>

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
	struct Node* parent;
	struct Node* down;
	struct Node* right;
	int used;	
	int width;
	int height;
	int x;
	int y;

} Node;

Node *findNode(Node *node, int w, int h);
Node *splitNode(Node **node, int w, int h);
int MAX(int a, int b);
int MIN(int a, int b);


int main()
{
	FILE *file;
	char line[80];
	Node *root;
	Block **blocks;
	Block **blocksRemaining;
	int totalBoards, boardWidth, boardHeight, totalBlocks, totalBlocksRemaining = 0;
	int i, j, k;

	root = malloc(sizeof(Node));
	root->x = 0;
	root->y = 0;
	root->down = NULL;
	root->right = NULL;
	

	file = fopen("blocks.txt", "r");

	// First Line
	// Amount of Boards you can cut
	fgets(line, 80, file);
	sscanf(line, "%d\n", &totalBoards);

	// Second Line
	// Boards width
	fgets(line, 80, file);
	sscanf(line, "%d\n", &boardWidth);
	root->width = boardWidth;

	// Third Line
	// Boards height
	fgets(line, 80, file);
	sscanf(line, "%d\n", &boardHeight);
	root->height = boardHeight;

	// Fourth Line
	// Amount of blocks to cut
	fgets(line, 80, file);
	sscanf(line, "%d\n", &totalBlocks);
	blocks = malloc(totalBlocks * sizeof(Block *));
	blocksRemaining = malloc(totalBlocks * sizeof(Block *));

	// Read Blocks
	// Blocks width and height
	for (i = 0; i < totalBlocks; ++i)
	{	
		if(fgets(line, 80, file)){
			int w, h;
			blocks[i] = malloc(sizeof(Block));
			sscanf(line, "%d %d", &w, &h);
			blocks[i]->width = w;
			blocks[i]->height = h;
			blocks[i]->id = i;
			blocks[i]->fit = NULL;
		}
	}

	fclose(file);	

	file = fopen("cuts.txt", "w+");
	
	// Loop through boards
	for (k = 1; k <= totalBoards; ++k){

		fprintf(file, "board %d\n", k);

		root->used = 0;
		totalBlocksRemaining = 0;

		// Sort Blocks for best performance
		// Bubble sort by Max Side
		for (i = 0; i < totalBlocks; i++) {			
			for (j = 0; j < totalBlocks - i - 1; j++) {
				int maxJ = MAX(blocks[j]->height, blocks[j]->width);
				int minJ = MIN(blocks[j]->height, blocks[j]->width);

				int maxJ1 = MAX(blocks[j + 1]->height, blocks[j + 1]->width);
				int minJ1 = MIN(blocks[j + 1]->height, blocks[j + 1]->width);

				if (maxJ < maxJ1) {
					Block *b = blocks[j];
					blocks[j] = blocks[j + 1];
					blocks[j + 1] = b;
				}
				else if (maxJ == maxJ1 && minJ < minJ1) {
					Block *b = blocks[j];
					blocks[j] = blocks[j + 1];
					blocks[j + 1] = b;
				}
			}
		}

		//Start Algorithm
		for (i = 0; i < totalBlocks; i++) {
			Block *block = blocks[i];
			Node *node;
			if (node = findNode(root, block->width, block->height)) {	
				block->fit = splitNode(&node, block->width, block->height);
			}
		}

		for (i = 0; i < totalBlocks; i++) {
			Block *block = blocks[i];
			if (block->fit) {
				fprintf(file, "X%d Y%d W%d H%d\n", block->fit->x, block->fit->y, block->width, block->height);
				printf("BLOCK %d fit in BOARD %d\n", block->id, k);
				printf("x %d y %d\n", block->fit->x, block->fit->y);
				printf("w %d h %d\n", block->width, block->height);
				printf("\n");
			} else {				
				printf("BLOCK %d dont fit in BOARD %d\n\n", block->id, k);
				blocksRemaining[totalBlocksRemaining] = malloc(sizeof(Block));
				blocksRemaining[totalBlocksRemaining] = block;
				totalBlocksRemaining++;
			}
		}

		free(blocks);

		blocks = malloc(totalBlocksRemaining * sizeof(Block *));
		totalBlocks = totalBlocksRemaining;
		for (i = 0; i < totalBlocksRemaining; i++) {
			blocks[i] = blocksRemaining[i];
		}

	}		

    return 0;
}

Node *findNode(Node *node, int w, int h) {	
	if (node->used == 1) {
		Node * rightNode = findNode(node->right, w, h);
		Node * downNode = findNode(node->down, w, h);
		if (rightNode)
			return rightNode;
		return downNode;
	}
	else if ((w <= node->width) && (h <= node->height)) {
		return node;
	}
	else {
		return NULL;
	}
}

Node *splitNode(Node **node, int w, int h) {

	(*node)->used = 1;

	(*node)->down = malloc(sizeof(Node));
	(*node)->down->x = (*node)->x;
	(*node)->down->y = (*node)->y + h;
	(*node)->down->width = (*node)->width;
	(*node)->down->height = (*node)->height - h;
	(*node)->down->used = 0;

	(*node)->right = malloc(sizeof(Node));
	(*node)->right->x = (*node)->x + w;
	(*node)->right->y = (*node)->y;
	(*node)->right->width = (*node)->width - w;
	(*node)->right->height = h;
	(*node)->right->used = 0;

	return *node;
}

int MAX(int a, int b) {
	return (a > b) ? a : b;
}
int MIN(int a, int b) {
	return (a < b) ? a : b;
}