#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MEMERROR 12

char *replace_word(const char *s, const char *oldW, const char *newW)
{
	char *result;
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);
	int in_quote = FALSE;

	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] == '\"')
			in_quote = !in_quote;
		if (in_quote)
			continue;
		if (strstr(&s[i], oldW) == &s[i]) {
			cnt++;
			i += oldWlen - 1;
		}
	}
	result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);
	if (result == NULL)
		exit(MEMERROR);
	i = 0;
	while (*s) {
		if (s[0] == '\"')
			in_quote = !in_quote;
		if (!in_quote && strstr(s, oldW) == s) {
			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
		} else
			result[i++] = *s++;
	}
	result[i] = '\0';
	return result;
}
void remove_char(char *s, int c)
{
	int i, j, n = strlen(s);

	for (i = j = 0; i < n; i++)
		if (s[i] != c)
			s[j++] = s[i];
	s[j] = '\0';
}
void remove_chars(char *s, char *cs)
{
	int i, n = strlen(cs);

	for (i = 0; i < n; i++)
		remove_char(s, cs[i]);
}
char *get_path(char *file_name)
{
	int i, n = strlen(file_name);
	int last_slash_pos = 0;

	for (i = 0; i < n; i++) {

		if (file_name[i] == '/' || file_name[i] == '\\')
			last_slash_pos = i;
	}
	file_name[last_slash_pos] = '\0';
	return file_name;
}
