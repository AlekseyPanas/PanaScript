/*
Print decimal ascii of each character in the string
*/
void print_bin(char *str);

/*
Given a string, mutate it by replacing trailing spaces with null pointers and returning
a new pointer to the first nonspace character to remove leading spaces
*/
char *strip(char *s);

/*Return pointer to first occurrence of c in s, or NULL otherwise*/
char *char_in(char c, char *s);
