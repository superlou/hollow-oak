#ifndef TOKEN_HPP_
#define TOKEN_HPP_

typedef struct {
  int addr;
  char name[32];
} Token;

void token_new(Token *token, int addr, char* name);
bool token_is_set(Token *token);
bool token_is_clear(Token *token);
void token_set(Token *token);
void token_clear(Token *token);

#endif
