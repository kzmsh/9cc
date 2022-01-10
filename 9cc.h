#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_RESERVED, // 記号
    TK_IDENT,    // 識別子
    TK_NUM,      // 整数
    TK_EOF       // 終端記号
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next; // 次の入力トークン
    int val;     // TK_NUM の数値
    char *str;   // トークン文字列
    int len;     // トークン長
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

bool consume(char *op);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();

Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

extern Token *token; // 解析済みのトークン列

typedef enum {
    ND_ADD,    // +
    ND_SUB,    // -
    ND_MUL,    // *
    ND_DIV,    // /
    ND_EQ,     // ==
    ND_NE,     // !=
    ND_LT,     // <
    ND_LE,     // <=
    ND_ASSIGN, // =
    ND_LVAR,   // Local variable
    ND_NUM,    // Integer
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind; // ノードの種類
    Node *next;    // 次のノード
    Node *lhs;     // 左辺 left-hand side
    Node *rhs;     // 右辺 right-hand side
    char name;     // kindがND_IDENTの場合の変数名
    int val;       // kindがND_NUMの場合の値
};

Node *program();

void codegen(Node *node);
