#ifndef BST_H
#define BST_H

#include <stddef.h>
#include <stdint.h>

struct bst {
    const void *value;
    struct bst *left;
    struct bst *right;
};

typedef int32_t (*comparator)(const void *, const void *, size_t);

bool bst_insert_cmp(const void *value, struct bst **bst, size_t size, comparator cmp);

bool bst_insert(const void *value, struct bst **bst, size_t size);

bool bst_search_cmp(const void *value, const struct bst *bst, size_t size, comparator cmp);

bool bst_search(const void *value, const struct bst *bst, size_t size);

size_t bst_inorder(const struct bst *bst, void (*f)(const void *, void *), void *args);

void bst_free(struct bst *bst);

size_t bst_size(const struct bst *bst);

size_t bst_height(const struct bst *bst);

#endif  // BST_H
