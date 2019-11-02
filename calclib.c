#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

int vals[32];
int vals_len = 0;

char exps[32];
int exps_len = 0;

static void push_value(int exp);
static void pop_value(int *exp);
static void push_exp(char exp);
static void pop_exp(char *exp);
static int exec_calc(char exp, int val1, int val2);
static int val_strlen(char *str);

int calc(char *str) {
    int result;
    char *pos;
    int ret;
    int len;
    int tmp, val1,val2;
    char exp;

#if DEBUG
    fprintf(stdout, "str:%s\n", str);
#endif

    pos = str;
    while(*pos) {
        switch (*pos) {
        case '(':
            push_exp(*pos);
            pos++;
            break;
        case ')':
            while(0 < exps_len) {
                pop_exp(&exp);
                if (exp == '(') {
                    // TODO �������̑Ή��`�F�b�N�͂��Ă��Ȃ�
                    break;
                }
                pop_value(&val2);
                pop_value(&val1);
                ret = exec_calc(exp, val1, val2);
                push_value(ret);
            }
            pos++;
            break;
        case '*':
        case '/':
        case '+':
            push_exp(*pos);
            pos++;
            break;
        case '-':
        	// �}�C�i�X�l�Ή� �� 0 - 1 ���v�Z���邱�Ƃ� -1 �����
        	if (vals_len < 1) {
        		// -1 + 2�̂悤�ȏꍇ
        		push_value(0);
        	}

        	if ((0 < exps_len) && (exps[exps_len -1] == '(')) {
        		// (-1 + 2)�̂悤�ȏꍇ
        		push_value(0);
        	}
            push_exp(*pos);
            pos++;
            break;
        case ' ':
            // �󔒂̓X�L�b�v
            pos++;
            break;
        default:
            if (isdigit((int)*pos)) {
                // ���������̕����񒷂����߂�
                len = val_strlen(pos);
                tmp = atoi(pos);
#if DEBUG
                fprintf(stdout, "tmp:[%d]\n", tmp);
#endif
                push_value(tmp);
                if (exps_len) {
                    // ���Z�q�̗D��x���`�F�b�N
                    if ((exps[exps_len -1] == '*') ||
                        (exps[exps_len -1] == '/') ||
                        (exps[exps_len -1] == '-')) {
                        pop_value(&val2);
                        pop_value(&val1);
                        pop_exp(&exp);
                        ret = exec_calc(exp, val1, val2);
                        push_value(ret);
                    }
                }
                pos += len;
                continue;
            } else {
                fprintf(stderr, "syntax error [%c]\n", *pos);
                return -1;
            }
        }
    }

    while(0 < exps_len) {
        pop_exp(&exp);
        if (exp == '(') {
            // TODO �������̑Ή��`�F�b�N�͂��Ă��Ȃ�
            continue;
        }

        pop_value(&val2);
        pop_value(&val1);
        ret = exec_calc(exp, val1, val2);
        push_value(ret);
    }

    pop_value(&result);
    return result;
}

int exec_calc(char exp, int val1, int val2) {
    int result;

#if DEBUG
    fprintf(stdout, "calc: %d %c %d \n", val1, exp, val2);
#endif

    switch (exp) {
    case '+':
        result = val1 + val2;
        break;
    case '-':
        result = val1 - val2;
        break;
    case '*':
        result = val1 * val2;
        break;
    case '/':
        result = val1 / val2;
        break;
    default:
        fprintf(stderr, "Unkown exp [%c]\n", exp);
        break;
    }

#if DEBUG
    fprintf(stdout, "calc result:[%d]\n", result);
#endif
    return result;
}

int val_strlen(char *str) {
    int len = 0;
    while(isdigit((int)*str)) {
        len++;
        str++;
    }
    return len;
}

void push_value(int val) {
    vals[vals_len] = val;
    vals_len++;
    return;
}

void pop_value(int *val) {
    *val = vals[vals_len - 1];
    vals_len--;
    return;
}


void push_exp(char exp) {
    exps[exps_len] = exp;
    exps_len++;
    return;
}

void pop_exp(char *exp) {
    *exp = exps[exps_len - 1];
    exps_len--;
    return;
}
