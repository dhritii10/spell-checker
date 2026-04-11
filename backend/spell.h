#ifndef SPELL_H
#define SPELL_H

void load_dictionary();
int check_word(char *word);
void suggest_word(char *word);
void process_word(char *word);

#endif