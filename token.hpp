#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

typedef struct {
  int addr;
} Token;

void token_new(Token *token, int addr);
bool token_is_set(Token *token);
bool token_is_clear(Token *token);
void token_set(Token *token);
void token_clear(Token *token);

#endif
