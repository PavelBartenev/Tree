#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int type;

struct NODE
{
	type data;
	NODE* left;
	NODE* right;
	int call;
};

NODE* tree_constructor();

NODE* push_left(NODE*, type);

NODE* push_right(NODE*, type);

int file_reader(FILE*, NODE*);

int main()
{
	FILE* input = fopen("input.txt", "r");
	NODE* head = tree_constructor();
	file_reader(input, head);

	printf("%d", head->right->left->right->data);

	return 0;
}

NODE* tree_constructor()
{
	NODE* head = (NODE*)calloc(1, sizeof(NODE));
	
	if (!head)
		exit(-1);
		
	head->left = nullptr;
	head->right = nullptr;
	head->data = -666;
	head->call = 0;

	return head;
}

NODE* push_left(NODE* node, type data)
{
	node->left = (NODE*)calloc(1, sizeof(NODE));

	if (!node->left)
		exit(-2);

	node->left->data = data;
	node->left->left = nullptr;
	node->left->right = nullptr;
	node->left->call = 0;

	return (node->left);
}

NODE* push_right(NODE* node, type data)
{
	node->right = (NODE*)calloc(1, sizeof(NODE));

	if (!node->right)
		exit(-3);

	node->right->data = data;
	node->right->left = nullptr;
	node->right->right = nullptr;
	node->right->call = 0;

	return (node->right);
}

int file_reader(FILE* input, NODE* node)
{
	if (node->data == -666)
	{
		type data = 0;
		fscanf(input, "%*c %d", &data);
		node->data = data;
	}

	node->call++;

	if (node->call == 1)
	{
		//int data = 0;
		//fscanf(input, "%d", &data);
		//node->data = data;

		char command[10] = {};

		fscanf(input, "%s", command);

		if (!strcmp("nil", command))
			node->call++;

		if (!strcmp("{", command))
		{
			int left_data = 0;

			fscanf(input, "%d", &left_data);

			NODE* left_node = push_left(node, left_data);

			file_reader(input, left_node);

			node->call++;
		}
	}


	if (node->call == 2)
	{
		char command[10] = {};

		fscanf(input, "%s", command);

		if (!strcmp("nil", command))
			node->call++;

		if (!strcmp("{", command))
		{
			int right_data = 0;

			fscanf(input, "%d", &right_data);

			NODE* right_node = push_right(node, right_data);

			file_reader(input, right_node);

			node->call++;
		}
	}

	if (node->call == 3)
	{
		char pass[10] = {};
		fscanf(input, "%s", pass);
	}

	return 0;
}



		

