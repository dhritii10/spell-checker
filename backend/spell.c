#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 10000
#define LEN 100

char dict[MAX][LEN];
int dict_size = 0;

void load_dictionary() {
    FILE *fp = fopen("../backend/dictionary.txt", "r");
    if (!fp) {
        printf("Dictionary not found\n");
        exit(1);
    }
    while (fscanf(fp, "%s", dict[dict_size]) != EOF) {
        dict_size++;
    }
    fclose(fp);
}

int check_word(char *word) {
    for (int i = 0; i < dict_size; i++) {
        if (strcmp(word, dict[i]) == 0)
            return 1;
    }
    return 0;
}

int min(int a, int b, int c) {
    if (a < b && a < c) return a;
    if (b < c) return b;
    return c;
}

int edit_distance(char *a, char *b) {
    int dp[LEN][LEN];
    int n = strlen(a), m = strlen(b);

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0) dp[i][j] = j;
            else if (j == 0) dp[i][j] = i;
            else if (a[i-1] == b[j-1])
                dp[i][j] = dp[i-1][j-1];
            else
                dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]);
        }
    }
    return dp[n][m];
}

int is_vowel(char c) {
    c = tolower(c);
    return (c=='a'||c=='e'||c=='i'||c=='o'||c=='u');
}

int vowel_insertion_match(char *a, char *b) {
    int i = 0, j = 0;
    while (a[i] && b[j]) {
        if (a[i] == b[j]) {
            i++; j++;
        } else if (is_vowel(b[j])) {
            j++;
        } else {
            return 0;
        }
    }
    return (a[i] == '\0');
}

void find_best_suggestion(char *word, char *best) {

    int best_score = 999;
    int len = strlen(word);

    for (int i = 0; i < dict_size; i++) {

        int dlen = strlen(dict[i]);

        if (abs(len - dlen) > 3) continue;

        if (vowel_insertion_match(word, dict[i])) {
            strcpy(best, dict[i]);
            return;
        }

        int d = edit_distance(word, dict[i]);
        if (d > 3) continue;

        int i1 = 0, j1 = 0, match = 0;

        while (word[i1] && dict[i][j1]) {
            if (word[i1] == dict[i][j1]) {
                match++;
                i1++; j1++;
            } else {
                j1++;
            }
        }

        if (match < len / 2) continue;

        int score = d + abs(len - dlen) - match;

        if (score < best_score) {
            best_score = score;
            strcpy(best, dict[i]);
        }
    }
}

void process_text(FILE *fp) {

    char raw[LEN];
    int start_sentence = 1;

    char final_sentence[10000] = "";
    int first_word = 1;

    while (fscanf(fp, "%s", raw) != EOF) {

        char word[LEN] = "";
        char punct[LEN] = "";

        int wi = 0, pi = 0;

        for (int i = 0; raw[i]; i++) {
            if (ispunct(raw[i]))
                punct[pi++] = raw[i];
            else
                word[wi++] = raw[i];
        }
        word[wi] = '\0';
        punct[pi] = '\0';

        if (strlen(word) == 0) continue;

        int was_cap = isupper(word[0]);

        char lower[LEN];
        strcpy(lower, word);
        for (int i = 0; lower[i]; i++)
            lower[i] = tolower(lower[i]);

        char corrected[LEN];
        strcpy(corrected, word);

        if (!check_word(lower)) {

            char suggestion[LEN] = "";
            find_best_suggestion(lower, suggestion);

            if (strlen(suggestion) > 0) {
                strcpy(corrected, suggestion);

                if (was_cap)
                    corrected[0] = toupper(corrected[0]);

                printf("%s|Spelling Error|%s\n", word, corrected);
            }
        }

        if (start_sentence && !isupper(corrected[0])) {
            corrected[0] = toupper(corrected[0]);
            printf("%s|Capitalization|%s\n", word, corrected);
        }

        if (strcmp(word, "i") == 0) {
            strcpy(corrected, "I");
            printf("%s|Capitalization|I\n", word);
        }

        if (strlen(punct) > 1) {
            printf("%s|Punctuation Error|%c\n", raw, punct[0]);
        }

        if (!first_word)
            strcat(final_sentence, " ");
        strcat(final_sentence, corrected);

        if (strlen(punct) > 0)
            strcat(final_sentence, punct);

        first_word = 0;

        int lenp = strlen(punct);
        if (lenp > 0) {
            char last = punct[lenp-1];
            if (last == '.' || last == '!' || last == '?')
                start_sentence = 1;
            else
                start_sentence = 0;
        } else {
            start_sentence = 0;
        }
    }

    if (!start_sentence) {
        printf("sentence_end|Punctuation Error|.\n");
        strcat(final_sentence, ".");
    }

    printf("FINAL|Sentence|%s\n", final_sentence);
}