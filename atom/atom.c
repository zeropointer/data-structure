/**
 * @file atom.c
 * @author zeropointer <zeropointer@qq.com>
 * @date 2017-02-27
 * @brief atom类型实现
 */

#include "atom.h"

static atom_t* bucket[INIT_BUCKET_SIZE];

void atom_init (int hint) {
    assert (hint >= 0);

    if (hint >= INIT_BUCKET_SIZE * 32) {

    }
}

const char* atom_new (const char* str, int len) {
    unsigned long h;
    int i;
    atom_t* p;

    assert (len >= 0);
    assert (str);

    h = hash_func (str, len);

    //遍历哈希链表，查找看是否已有
    for (p = bucket[h]; p; p = p->link) {
        if (len == p->len) {
            for (i = 0; i < len && p->str[i] == str[i]; i++);
            if (i == len) {
                return p->str;
            }
        }
    }

    //分配新的原子节点，插入哈希表
    p = (atom_t*) malloc (sizeof (*p) + sizeof (char) * len + 1);
    p->len = len;
    strncpy (p->str, str, len);
    p->str[len] = '\0';
    p->link = bucket[h];
    bucket[h] = p;
}

const char* atom_string (const char* str) {
    assert (str);
    return atom_new (str, strlen (str));
}

const char* atom_int (long n) {
    char str[43];
    char *p = str + sizeof (str);
    unsigned long m;

    //最大正数与最小负数并不对称，不能直接取相反数
    if (n == LONG_MIN) {
        m = LONG_MAX + 1UL;
    } else if (n < 0) {
        m = -n;
    } else {
        m = n;
    }

    do {
        *--p = m % 10 + '0';
    } while ((m / 10) > 0);

    if (n < 0) {
        *--p = '-';
    }

    return atom_new (p, str + sizeof (str) - p);
}

/**
 * 释放一个指定的原子
 * @param[in] str 待释放的原子
 */
void atom_free (char* str) {
    unsigned long h;
    unsigned int len, i;
    atom_t *p *q;

    assert (str);

    len = strlen (str);
    h = hash_func (str, len);
    for (p = q = bucket[h]; p; p = p->link) {
        if (p->len == len) {
            for (i = 0; i < len && p->str[i] == str[i]; i++);
            if (i == len) {
                if (q == p) {
                    bucket[h] = p->link;
                } else {
                    q->link = p->link;
                }
                free (p);
                return ;
            }
        }
        q = p;
    }
}

/**
 * 释放所有原子
 */
void atom_reset (void) {
    int i;
    atom_t *p, *q;
    for (i = 0; i < INIT_BUCKET_SIZE; i++) {
        for (p = bucket[i]; p;) {
            q = p->link;
            free (p);
            p = q;
        }
    }
}

static long hash_func (const char* str, int len) {
    static unsigned long scatter[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1
    };

    long h;
    int i;
    for (h = 0, i = 0; i < len; i++) {
        h = (h << 1) + scatter[(unsigned char)str[i]];
    }

    return h % INIT_BUCKET_SIZE;
}
