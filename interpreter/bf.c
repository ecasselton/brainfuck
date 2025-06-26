#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t u8;

void interpret(char *tokens, u8 *cells, int *info) {
    int token = 0;
    int ptr = 0;
    while (token < strlen(tokens)) {
		// printf("%d ", ptr);
        switch (tokens[token]) {
            case '=':
                cells[ptr] += info[token];
                break;
            case '^':
                ptr += info[token];
                break;
            case '.':
                putchar(cells[ptr]);
                fflush(stdout);
                break;
            case ',':;
                int c = getchar();
                if (c != EOF) {
                    cells[ptr] = c;
                }
                break;
            case '[':
                if (cells[ptr] == 0) {
                    token += info[token];
                }
                break;
            case ']':
                if (cells[ptr] != 0) {
                    token -= info[token];
                }
                break;
        }
        token++;
        // printf("%d ", token);
    }
}

int pairBrackets(char *tokens, int *jumps, int depth) {
    int token = 0;
    while (token < strlen(tokens)) {
        if (tokens[token] == '[') {
            jumps[token] = pairBrackets(tokens + token + 1, jumps + token + 1, depth + 1);
            token += jumps[token];
        } else if (tokens[token] == ']') {
            jumps[token] = token + 1;
            return token + 1;
        }
        token++;
    }
    if (depth == 0) {
        return 0;
    } else {
        return -1;
    }
}

int isKeyword(char c) {
    return c == '-' || c == '+' || c == '<' || c == '>' || c == '.' || c == ',' || c == '[' || c == ']';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No input file\n");
        return 1;
    }

    FILE *fptr;

    fptr = fopen(argv[1], "r");
    u8 cells[300000] = {0};
    char tokens[100000] = {0};
    int info[100000] = {0};
    int length = 0;
    char c;
    u8 num = 0;


    while (!feof(fptr)) {
        c = fgetc(fptr);
        if (isKeyword(c)) {
            switch (c) {
                case '+':
					if (tokens[length] == '^')
						length++;
                    info[length]++;
                    tokens[length] = '=';
                    break;
                case '-':
					if (tokens[length] == '^')
						length++;
                    info[length]--;
                    tokens[length] = '=';
                    break;
                case '>':
					if (tokens[length] == '=')
						length++;
                    info[length]++;
                    tokens[length] = '^';
                    break;
                case '<':
					if (tokens[length] == '=')
						length++;
                    info[length]--;
                    tokens[length] = '^';
                    break;
                default:
					if (tokens[length] == '=' || tokens[length] == '^')
						length++;
                    tokens[length++] = c;
                    break;
            }
        }
    }

    fclose(fptr);

    int err = pairBrackets(tokens, info, 0);
    if (err == -1) {
        printf("Unmatched '['");
    } else if (err > 0) {
        printf("Unmatched ']'");
    }

	//    for (int i = 0; i < length; i++) {
	//        printf("%d ", info[i]);
	//    }
	// printf("\n");

    interpret(tokens, cells, info);

    return 0;
}
