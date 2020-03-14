#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMERROR 12

struct node {
	char *key;
	char *val;
	struct node *next;
};
struct table {
	int size;
	struct node **list;
};
struct table *create_table(int size)
{
	int i;
	struct table *t = (struct table *)malloc(sizeof(struct table));

	if (t == NULL)
		exit(MEMERROR);
	t->size = size;
	t->list = (struct node **)malloc(sizeof(struct node *)*size);

	if (t->list == NULL)
		exit(MEMERROR);

	for (i = 0; i < size; i++)
		t->list[i] = NULL;
	return t;
}
int hash_code(struct table *t, char *key)
{
	int char_sum = 0;
	int i;

	for (i = 0; i < strlen(key); i++)
		char_sum += key[i];
	return char_sum % t->size;
}
void insert(struct table *t, char *key, char *val)
{
	int pos = hash_code(t, key);
	struct node *list = t->list[pos];
	struct node *temp = list;
	struct node *newNode;

	while (temp) {
		if (temp->key == key) {
			temp->val = val;
			return;
		}
		temp = temp->next;
	}
	newNode = (struct node *)malloc(sizeof(struct node));

	if (newNode == NULL)
		exit(MEMERROR);
	newNode->key = malloc(sizeof(char) * strlen(key) + 1);

	if (newNode->key == NULL)
		exit(MEMERROR);
	strcpy(newNode->key, key);
	newNode->val = malloc(sizeof(char) * strlen(val) + 1);

	if (newNode->val == NULL)
		exit(MEMERROR);
	strcpy(newNode->val, val);
	newNode->next = list;
	t->list[pos] = newNode;
}
char *lookup(struct table *t, char *key)
{
	int pos = hash_code(t, key);
	struct node *list = t->list[pos];
	struct node *temp = list;

	while (temp) {
		if (strcmp(temp->key, key) == 0)
			return temp->val;
		temp = temp->next;
	}
	return NULL;
}
void erase(struct table *t, char *key)
{
	int pos = hash_code(t, key);
	struct node *list = t->list[pos];
	struct node *curr = list;
	struct node *prev = NULL;

	while (curr) {
		if (strcmp(curr->key, key) == 0) {
			if (prev)
				prev->next = curr->next;
			else
				t->list[pos] = curr->next;
			free(curr->key);
			free(curr->val);
			free(curr);
			break;
		}
		prev = curr;
		curr = curr->next;
	}
}

void free_table(struct table *t)
{
	int i;

	for (i = 0; i < t->size; i++) {
		struct node *list = t->list[i];
		struct node *curr = list;

		while (curr) {
			struct node *prev = curr;

			free(curr->key);
			free(curr->val);
			curr = curr->next;
			free(prev);
		}
	}
	free(t->list);
	free(t);
}
