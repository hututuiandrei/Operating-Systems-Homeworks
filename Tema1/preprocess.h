#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_NAME_FILE 50
#define MAX_LINE_SIZE 256
#define MAX_NUM_PATHS 50
#define HASHMAP_SIZE 100
#define FLAG_OFFSET 2
#define BAD_ARGS -1
#define ELSE_SIZE 5
#define ENDIF_SIZE 6
#define MEMERROR 12

int preprocess(FILE *in_fp, FILE *out_fp, struct table *t, char **args_paths)
{
	int ret_code = 0;
	char *buffer;
	char *buf_copy;
	char *ret;
	int print_next = TRUE;

	buffer = malloc(sizeof(char) * MAX_LINE_SIZE);

	if (buffer == NULL)
		exit(MEMERROR);
	buf_copy = malloc(sizeof(char) * MAX_LINE_SIZE);

	if (buf_copy == NULL)
		exit(MEMERROR);
	ret = malloc(sizeof(char) * MAX_LINE_SIZE);

	if (ret == NULL)
		exit(MEMERROR);
	/*read line by line from input*/
	while (fgets(buffer, MAX_LINE_SIZE, in_fp)) {

		char *token;
		/*skip newlines*/
		if (strcmp(buffer, "\n") == 0)
			continue;
		/*line ending in backslash character must be concatenated with*/
		/*the following one*/
		while (buffer[strlen(buffer) - 2] == '\\') {

			remove_chars(buffer, "\n\\");
			fgets(buf_copy, MAX_LINE_SIZE, in_fp);
			strcat(buffer, buf_copy);
		}

		strcpy(buf_copy, buffer);
		token = strtok(buf_copy, "\t []{}<>=+-*/%!&|^.,:;()");
		/*token is the first word in a line*/
		if (strcmp(token, "#include") == 0) {
			/*name of the file we are looking to include*/
			char *file_name = strtok(NULL, "\"");
			FILE *header_fp;
			/*this will become the absolute path to file*/
			char *abs_file_name = malloc(sizeof(char) *
				MAX_NAME_FILE);
			int i;
			int found = FALSE;
			/*look for file in current director*/
			header_fp = fopen(file_name, "r");

			if (abs_file_name == NULL)
				exit(MEMERROR);
			/*if file is in current director preprocess the file*/
			if (header_fp) {

				found = TRUE;
				preprocess(header_fp, out_fp, t, args_paths);
				fclose(header_fp);
				free(abs_file_name);
				continue;
			}
			/*args_paths[0] contains the path to the original .c*/
			/*file which needs to be preprocessed, the rest of*/
			/*them are directories given as arguments with -I*/
			/*flag; We are also looking for the file here*/
			for (i = 0; strcmp(args_paths[i], ""); i++) {

				strcpy(abs_file_name, args_paths[i]);
				strcat(abs_file_name, "/");
				strcat(abs_file_name, file_name);
				header_fp = fopen(abs_file_name, "r");

				if (header_fp) {

					found = TRUE;
					preprocess(header_fp, out_fp, t,
						args_paths);
					fclose(header_fp);
					break;
				}
			}
			/*if we still can't find we continue the process but*/
			/*the program will exit with an error code*/
			if (!found)
				ret_code = BAD_ARGS;
			free(abs_file_name);
			continue;
		}
		/*print_next determines whether the next line will be*/
		/*skiped or not*/
		if (strncmp(token, "#else\n", ELSE_SIZE) == 0) {

			print_next = !print_next;
			continue;
		}

		if (strncmp(token, "#endif", ENDIF_SIZE) == 0) {

			print_next = TRUE;
			continue;
		}

		if (strcmp(token, "#elif") == 0) {

			char *cond = strtok(NULL, "\n ");
			/*search for condition in the table*/
			if (lookup(t, cond)) {

				if (atoi(lookup(t, cond)) > 0)
					print_next = TRUE;
				else
					print_next = FALSE;
			} else {

				if (atoi(cond))
					print_next = TRUE;
				else
					print_next = FALSE;
			}
			continue;
		}
		/*following directives should not even be*/
		/*acknowledged if we are skiping the next line.*/
		/*Just the previous ones can change the truth value*/
		/*of print_next from FALSE to TRUE*/
		if (!print_next)
			continue;
		/*defines are inserted in the table*/
		if (strcmp(token, "#define") == 0) {

			char *key = strtok(NULL, " ");
			char *value = strtok(NULL, "\n ");

			strcpy(ret, "");
			/*the value can contain multiple tokens*/
			/*so we need to concatenate them and lookup*/
			/*for each of them in the table in case we*/
			/*already know its value*/
			while (value) {

				if (strcmp(ret, "") != 0)
					strcat(ret, " ");

				if (lookup(t, value))
					strcat(ret, lookup(t, value));
				else
					strcat(ret, value);
				value = strtok(NULL, "\n ");
			}

			insert(t, key, ret);
			continue;
		}
		/*delete key value from table*/
		if (strcmp(token, "#undef") == 0) {

			char *key = strtok(NULL, "\n ");

			erase(t, key);
			continue;
		}
		if (strcmp(token, "#if") == 0) {

			char *cond = strtok(NULL, "\n ");

			if (lookup(t, cond)) {
				if (atoi(lookup(t, cond)) > 0)
					print_next = TRUE;
				else
					print_next = FALSE;
			} else {
				if (atoi(cond))
					print_next = TRUE;
				else
					print_next = FALSE;
			}
			continue;
		}
		if (strcmp(token, "#ifdef") == 0) {

			char *var = strtok(NULL, "\n ");

			if (lookup(t, var))
				print_next = TRUE;
			else
				print_next = FALSE;
			continue;
		}
		if (strcmp(token, "#ifndef\0") == 0) {

			char *var = strtok(NULL, "\n ");

			if (lookup(t, var))
				print_next = FALSE;
			else
				print_next = TRUE;
			continue;
		}
		/*replace each occurrence of a token in this line (buffer)*/
		/*if there is a definition of it in the table and the token*/
		/*is not in quotes (replace_word's responsability). Then it*/
		/*puts the modified line in the output fp*/
		while (token) {

			if (lookup(t, token)) {

				char *result = replace_word(buffer, token,
					lookup(t, token));
				strcpy(buffer, result);
				free(result);
			}

			token = strtok(NULL, "\t []{}<>=+-*/%!&|^.,:;()\n");
		}
		fputs(buffer, out_fp);
	}

	free(ret);
	free(buffer);
	free(buf_copy);
	return ret_code;
}
