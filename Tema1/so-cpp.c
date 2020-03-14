#include <stdio.h>
#include <string.h>
#include "hash-map.h"
#include "string_utils.h"
#include "preprocess.h"

#define TRUE 1
#define FALSE 0
#define MAX_NAME_FILE 50
#define MAX_LINE_SIZE 256
#define MAX_NUM_PATHS 50
#define HASHMAP_SIZE 100
#define FLAG_OFFSET 2
#define BAD_ARGS -1
#define MEMERROR 12

void free_memory(FILE *in_fp, FILE *out_fp, struct table *t)
{
	if (in_fp != NULL)
		fclose(in_fp);
	if (out_fp != NULL)
		fclose(out_fp);
	free_table(t);
}

int main(int argc, char **argv)
{
	int ret_code = 0;
	struct table *t = create_table(HASHMAP_SIZE);

	FILE *in_fp = NULL;
	FILE *out_fp = NULL;

	int num_args_files = 0;
	int i;
	int add_to_hash = FALSE;
	int add_to_path = FALSE;
	int path_index = 1;

	char *in_file_name;
	char **args_paths;

	in_file_name = malloc(sizeof(char) * MAX_NAME_FILE);

	if (in_file_name == NULL)
		exit(MEMERROR);

	args_paths = malloc(sizeof(char *) * MAX_NUM_PATHS);

	if (args_paths == NULL)
		exit(MEMERROR);

	for (i = 0; i < MAX_NUM_PATHS; i++) {

		args_paths[i] = malloc(sizeof(char) * MAX_NAME_FILE);
		if (args_paths[i] == NULL)
			exit(MEMERROR);
	}

	for (i = 1; i < argc; i++) {
		/*this argument follows -D flag*/
		if (add_to_hash) {

			char *key = strtok(argv[i], "=");
			char *value = strtok(NULL, "=");

			if (value == NULL)
				insert(t, key, "\"\"");
			else
				insert(t, key, value);
			add_to_hash = FALSE;
			continue;
		}
		/*this argument follows -I flag*/
		if (add_to_path) {

			strcpy(args_paths[path_index], argv[i]);
			path_index++;
			add_to_path = FALSE;
			continue;
		}
		/*Check for bad flags (eg. -X)*/
		if (argv[i][0] == '-' && argv[i][1] != 'D'
				&& argv[i][1] != 'I' && argv[i][1] != 'o') {

			free_memory(in_fp, out_fp, t);
			return BAD_ARGS;
		}
		/*-D flag*/
		if (argv[i][0] == '-' && argv[i][1] == 'D') {
			/*-D var1=var2*/
			if (argv[i][2] == '\0') {
				add_to_hash = TRUE;
			/*-Dvar1=var2*/
			} else {
				char *key = strtok(argv[i] + FLAG_OFFSET, "=");
				char *value = strtok(NULL, "=");

				if (value == NULL)
					insert(t, key, "");
				else
					insert(t, key, value);
			}
		/*-I flag*/
		} else if (argv[i][0] == '-' && argv[i][1] == 'I') {
			/*-I dir*/
			if (argv[i][2] == '\0') {
				add_to_path = TRUE;
			/*-Idir*/
			} else {

				strcpy(args_paths[path_index], argv[i]
					+ FLAG_OFFSET);
				path_index++;
			}
		/*-o flag*/
		} else if (argv[i][0] == '-' && argv[i][1] == 'o') {

		} else {
			/*first file encountered is the input file*/
			if (num_args_files == 0) {

				in_fp = fopen(argv[i], "r");
				/*check if file exists*/
				if (in_fp == NULL) {

					free_memory(in_fp, out_fp, t);
					return BAD_ARGS;
				}
				strcpy(args_paths[0], get_path(argv[i]));
			/*second file encountered is the output file*/
			} else if (num_args_files == 1) {

				out_fp = fopen(argv[i], "w");
			/*3 or more files means wrong arguments*/
			} else {

				free_memory(in_fp, out_fp, t);
				return BAD_ARGS;
			}
			num_args_files++;
		}
	}

	if (in_fp == NULL)
		in_fp = stdin;
	if (out_fp == NULL)
		out_fp = stdout;

	ret_code = preprocess(in_fp, out_fp, t, args_paths);

	for (i = 0; i < MAX_NUM_PATHS; i++)
		free(args_paths[i]);
	free(args_paths);
	free(in_file_name);
	free_memory(in_fp, out_fp, t);

	return ret_code;
}
