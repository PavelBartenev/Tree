#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* type;

int const MAXDATA = 100;

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

int akinator(NODE*, NODE*, FILE*);

int print_tree(NODE*, FILE*, int);

int found(NODE*, char*, int*, int);

int compare_items(NODE* node);

int compare_ways(NODE*, int*, int*, char*, char*);

int definition(NODE*);

int Play();

int Draw(NODE*);

int Draw_param(NODE*, FILE*);

int Draw_tree(NODE*, FILE*, int);

int main()
{
	//FILE* input = fopen("input2.txt", "r+");

	//NODE* head = tree_constructor();

	//file_reader(input, head);

	Play();

	return 0;
}

int Play()
{
	FILE* input = fopen("input2.txt", "r+");

	NODE* head = tree_constructor();

	file_reader(input, head);

	int game_mode = 0;

	printf("If you want to play with Akinator press 1\nIf you want to ask the item's definition press 2\nIf you want to compare the items press 3\nUse _ instead of spaces\n\n");

	scanf("%d", &game_mode);

	switch (game_mode) {
	case 1:
		printf("I'll try to guess your word. Just answer the questions with 'yes' or 'no'.\n\n");
		akinator(head, head, input);
		break;
	case 2:
		printf("Write your word. Maybe I can give a definition to it.\n\n");
		definition(head);
		break;
	case 3:
		printf("Write two words in two lines. I'll try to compare them\n\n");
		compare_items(head);
		break;
	}

	Draw(head);

	return 0;
}

NODE* tree_constructor()
{
	NODE* head = (NODE*)calloc(1, sizeof(NODE));

	if (!head)
		exit(-1);

	head->left = nullptr;
	head->right = nullptr;
	head->data = nullptr;
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
	if (node->data == nullptr)
	{
		type data = (type)calloc(MAXDATA, sizeof(char));

		fscanf(input, " { %s", data);

		node->data = data;
	}

	node->call++;

	if (node->call == 1)
	{
		char command[10] = {};

		fscanf(input, " %s", command);

		if (!strcmp("nil", command))
			node->call++;

		if (!strcmp("{", command))
		{
			type left_data = (type)calloc(MAXDATA, sizeof(char));

			fscanf(input, " %s", left_data);

			NODE* left_node = push_left(node, left_data);

			file_reader(input, left_node);

			node->call++;
		}
	}


	if (node->call == 2)
	{
		char command[10] = {};

		fscanf(input, " %s", command);

		if (!strcmp("nil", command))
			node->call++;

		if (!strcmp("{", command))
		{
			type right_data = (type)calloc(MAXDATA, sizeof(char));

			fscanf(input, " %s", right_data);

			NODE* right_node = push_right(node, right_data);

			file_reader(input, right_node);

			node->call++;
		}
	}

	if (node->call == 3)
	{
		char pass[10] = {};
		fscanf(input, " %s", pass);
	}

	return 0;
}

int akinator(NODE* head, NODE* node, FILE* input)
{
	if ((node->left != nullptr) || (node->right != nullptr))
	{
		printf("%s\n", node->data);

		char answer[10] = {};

		scanf("%s", answer);

		if (!strcmp(answer, "yes"))
		{
			akinator(head, node->left, input);
			return 0;
		}

		else if (!strcmp(answer, "no"))
		{
			akinator(head, node->right, input);
			return 0;
		}
	}

	printf("It is %s\nRight?\n", node->data);

	char check[10] = {};

	scanf("%s", check);

	if (strcmp(check, "yes") == 0)
	{
		printf("Thanks for playing\n");

		return 0;
	}

	printf("Who it really was?\n");

	type new_character = (type)calloc(MAXDATA, sizeof(char));

	scanf("%s", new_character);

	printf("Please, ask a question with answer 'yes' in case of %s and 'no' in case of %s\n", new_character, node->data);

	type difference = (type)calloc(MAXDATA, sizeof(char));

	scanf("%s", difference);

	NODE* left = push_left(node, new_character);

	NODE* right = push_right(node, node->data);

	node->data = difference;

	fseek(input, 0, SEEK_SET);
	print_tree(head, input, 0);

	printf("Thanks for playing\n");

	return 0;
}

int print_tree(NODE* node, FILE* output, int num_of_spaces)
{
	if (!node)
		return 0;

	fprintf(output, "\n");

	for (int i = 0; i < num_of_spaces; i++)
		fprintf(output, " ");

	fprintf(output, "{ %s ", node->data);

	num_of_spaces += 5;

	if (node->left || node->right)
		fprintf(output, "\n");

	if (node->left)
	{
		print_tree(node->left, output, num_of_spaces);
	}
	else
	{
		fprintf(output, "nil ");
	}

	if (node->right)
	{
		print_tree(node->right, output, num_of_spaces);
	}
	else
	{
		fprintf(output, "nil ");
	}

	fprintf(output, "} ");

	num_of_spaces -= 5;

	return 0;

}

int found(NODE* node, char* item, int* way, int pos)
{
	if (!node)
		return 0;

	if (!strcmp(item, node->data))
		return 1;

	if (node->left)
	{
		way[pos] = 1;
		pos++;
		int stop = found(node->left, item, way, pos);
		pos--;

		if (stop) return 1;
	}

	if (node->right)
	{
		way[pos] = 2;
		pos++;
		int stop = found(node->right, item, way, pos);
		pos--;

		if (stop) return 1;
	}

	way[pos] = 0;

	return 0;
}

int compare_ways(NODE* node, int* way1, int* way2, char* item1, char* item2)
{
	int i = 0;

	printf("The questions with the similar answers:\n");

	while ((way1[i] == way2[i]) && (way1[i+1]))
	{
		printf("%s   ", node->data);

		if (way1[i] == 1)
		{
			printf("Yes\n");
			node = node->left;
		}
		else
		{
			printf("No\n");
			node = node->right;
		}

		i++;
	}

	printf("\nThe question with the different answers:\n");

	printf("%s   \n\n", node->data);

	if (way1[i] == 1)
		printf("Answer Yes in case of %s\nAnswer No in case of %s", item1, item2);
	else
		printf("Answer Yes in case of %s\nAnswer No in case of %s\n", item2, item1);

	return 0;
}

int definition(NODE* node)
{
	char* item = (char*)calloc(10, sizeof(char));

	scanf("%s", item);

	int* way = (int*)calloc(100, sizeof(int));

	int item_exist = found(node, item, way, 0);
	
	if (!item_exist)
	{
		printf("There is no %s in the variants", item);
		return 0;
	}

	int i = 0;

	while (way[i] != 0)
	{
		printf("%s     ", node->data);

		if (way[i] == 1)
		{
			printf("Yes\n");
			node = node->left;
		}

		if (way[i] == 2)
		{
			printf("No\n");
			node = node->right;
		}
		
		i++;
	}

	return 0;
}

int compare_items(NODE* head)
{
	char* item1 = (char*)calloc(10, sizeof(char));
	char* item2 = (char*)calloc(10, sizeof(char));

	scanf("%s", item1);
	scanf("%s", item2);

	int* way1 = (int*)calloc(100, sizeof(int));
	int* way2 = (int*)calloc(100, sizeof(int));

	int item_exist1 = found(head, item1, way1, 0);
	int item_exist2 = found(head, item2, way2, 0);

	if (!item_exist1)
	{
		printf("There is no %s in the variants", item1);
		return 0;
	}

	if (!item_exist2)
	{
		printf("There is no %s in the variants", item2);
		return 0;
	}

	compare_ways(head, way1, way2, item1, item2);

	return 0;
}

int Draw(NODE* node)
{
	FILE* output = fopen("graph_code.txt", "w");

	Draw_param(node, output);
	Draw_tree(node, output, 1);
	fprintf(output, "}\n}");

	return 0;
}

int Draw_param(NODE* node, FILE* output)
{
	fprintf(output, "digraph MyPlan {\n");
	fprintf(output, "  node[shape = \"circle\", style = \"filled\", fillcolor = \"white\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n");
	fprintf(output, "  edge [style=\"dashed\"];\n");

	fprintf(output, "{\n node[shape = \"plaintext\",style = \"invisible\"];\n edge[color = \"white\"];\n 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10;\n}\n");
	
	fprintf(output, "subgraph tree {\n   node [shape=\"ellipse\", style=\"filled\", fillcolor=\"white\", fontcolor=\"black\", fontsize=\"15\"];\n");


	return 0;
}

int Draw_tree(NODE* node, FILE* output, int level)
{
	if (node->left)
	{
		fprintf(output, "   \"%s\"->\"%s\" [ label = \"Yes\" ];\n", node->data, node->left->data);
		fprintf(output, "   { rank = \"same\"; \"%d\"; \"%s\"; }\n", level, node->data);
		Draw_tree(node->left, output, (level+1));
	}

	if (node->right)
	{
		fprintf(output, "   \"%s\"->\"%s\" [ label = \"No\" ];\n", node->data, node->right->data);
		fprintf(output, "   { rank = \"same\"; \"%d\"; \"%s\"; }\n", level, node->data);
		Draw_tree(node->right, output, (level+1));
	}

	return 0;
}
