#include "9cc.h"

// 与えられたノードがローカル変数である場合、変数のアドレスを計算し、スタックにプッシュする
// それ以外の場合、エラーを報告する
void gen_lval(Node *node) {
    if (node->kind != ND_LVAR) {
        error("左辺値ではありません");
    }

    int offset = (node->name - 'a' + 1) * 8;
    printf("  mov rax, rbp\n");        // ベースポインタのアドレス取得
    printf("  sub rax, %d\n", offset); // ベースポインタからのオフセットにより変数アドレスを取得
    printf("  push rax\n");            // 変数アドレスをスタックにプッシュ
}

void store() {
    printf("  pop rdi\n");        // スタックトップから右辺値を取得
    printf("  pop rax\n");        // スタックトップから変数アドレスを取得
    printf("  mov [rax], rdi\n"); // 右辺値を変数に格納
    printf("  push rdi\n");       // 右辺値をスタックにプッシュ
}

void load() {
    printf("  pop rax\n");        // スタックトップから変数アドレスを取得
    printf("  mov rax, [rax]\n"); // 変数アドレスから変数値を取得
    printf("  push rax\n");       // 変数値をスタックにプッシュ
}

void gen(Node *node) {
    switch (node->kind) {
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        load();
        return;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);
        store();
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_NE:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    }

    printf("  push rax\n");
}

void codegen(Node *node) {
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // プロローグ
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n"); // 変数a-z分の領域を確保する

    for (Node *n = node; n; n = n->next) {
        gen(n);
        printf("  pop rax\n"); // スタックオーバーフローを防ぐため、式の評価結果としてスタックに残っている値をポップする
    }

    // エピローグ
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
}
