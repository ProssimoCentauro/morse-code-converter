#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <readline/readline.h>

// Reset
#define RESET    "\033[0m"

// Bold colors
#define BBLACK   "\033[1;30m"
#define BRED     "\033[0;31m"
#define BGREEN   "\033[1;32m"
#define BYELLOW  "\033[0;33m"
#define BBLUE    "\033[1;34m"
#define BMAGENTA "\033[1;35m"
#define BCYAN    "\033[1;36m"
#define BWHITE   "\033[1;37m"

typedef struct {
    const char *morse;
    char letter;
} MorseEntry;

char *char_append(char *str, char c) {
    size_t len = str ? strlen(str) : 0;

    char *new_str = realloc(str, len + 2);
    if (!new_str) {
        free(str);
        return (NULL);
    }
    new_str[len] = c;
    new_str[len + 1] = '\0';
    return (new_str);
}

const MorseEntry morse_table[] = {
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'},
    {".", 'E'}, {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'},
    {"..", 'I'}, {".---", 'J'}, {"-.-", 'K'}, {".-..", 'L'},
    {"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'},
    {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
    {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'},
    {"-.--", 'Y'}, {"--..", 'Z'},

    {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'},
    {"....-", '4'}, {".....", '5'}, {"-....", '6'}, {"--...", '7'},
    {"---..", '8'}, {"----.", '9'}, {NULL, 0}
};

char	search_in_table(char *code)
{
	size_t	i = 0;
	while (code && morse_table[i].morse)
	{
		if (!strcmp(code, morse_table[i].morse))
			return (morse_table[i].letter);
		i++;
	}
	printf(BRED " SYNTAX ERROR: " RESET "{%s}" BYELLOW " invalid symbol! " RESET, code);
	return (0);
}

void	morse_to_alpha(char *line)
{
	char	*code = NULL;
	char	letter;
	int		space_counter = 0;

	while(*line)
	{
		while (*line && *line != ' ')
		{
			code = char_append(code, *line);
			line++;
		}
		while (*line == ' ' )
		{
			space_counter++;
			line++;
		}
		if (*(line - 1) == ' ' || !*line)
		{
			letter = search_in_table(code);
			if (letter != 0)
				printf("%c", letter);
			free(code);
			code = NULL;
		}
		if (space_counter == 3)
			printf(" ");
		else if (space_counter > 1)
			printf(BRED" SYNTAX ERROR: " BYELLOW "Invalid spaces number! " RESET);
		space_counter = 0;
	}
	printf("\n");
}

void	alpha_to_morse(char *line)
{
	while (*line)
	{
		*line = toupper(*line);
		if (isalpha(*line))
		{
			printf("%s%s", morse_table[*line - 'A'].morse, *(line + 1) ? " " : "");
		}
		else if (isdigit(*line))
		{
			printf("%s%s", morse_table[*line - '0' + 26].morse, *(line + 1) ? " " : "");
		}
		else if (*line == ' ')
			printf("  ");
		line++;
	}
	//write(1, "\n", 1);
	printf("\n");
}

int	get_mode()
{
	char	*line;
	int		mode;
	while (42)
	{
		printf(BYELLOW "1) " RESET "Alpha to morse\n");
		printf(BYELLOW "2) " RESET "Morse to alpha\n");
		line = readline(BMAGENTA "_-_-> " RESET);
		if (!line || (strcmp(line, "1") && strcmp(line, "2")))
		{
			printf(BRED "INVALID INPUT!\n" RESET);
			free(line);
			continue;
		}
		mode = atoi(line);
		free(line);
		break;
	}
	return (mode);
}

char	*get_input()
{
	char	*line;

//	printf("WARNING: Select only character from a to z or numbers!\n");
	line = readline(BMAGENTA "_-_-> " RESET);
	if (!line)
		exit(EXIT_FAILURE);
	return (line);
}

void	select_mode(int mode, char *line)
{
	if (mode == 1)
		alpha_to_morse(line);
	else
		morse_to_alpha(line);
}

int	main(int ac, char **av)
{
	char *line = NULL;
	int	mode;

	mode = get_mode();
	line = get_input();
	select_mode(mode, line);
	return (0);
}

