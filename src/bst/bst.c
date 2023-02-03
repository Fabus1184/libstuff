#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "header-only/memory.h"

/**
 * Binary search tree.
 * @param value The value stored in the node.
 * @param left The left child.
 * @param right The right child.
 */
struct bst {
    const void *value;
    struct bst *left;
    struct bst *right;
};

/**
 * Insert a value into a binary search tree using a comparator function.
 * @param value The value to insert.
 * @param bst The binary search tree.
 * @param size The size of the value.
 * @param cmp The comparator function.
 */
bool bst_insert_cmp(const void *value, struct bst **bst, size_t size, comparator cmp) {
    if (*bst == NULL) {
        *bst = malloc(sizeof(struct bst));
        assert(*bst != NULL && "Could not allocate memory for new node");
        **bst = (struct bst){value, NULL, NULL};
        return true;
    }

    if (cmp(value, (*bst)->value, size) == 0) {
        return false;
    }

    if (cmp(value, (*bst)->value, size) < 0) {
        return bst_insert_cmp(value, &(*bst)->left, size, cmp);
    } else {
        return bst_insert_cmp(value, &(*bst)->right, size, cmp);
    }
}

/**
 * Insert a value into a binary search tree.
 * @param value The value to insert.
 * @param bst The binary search tree.
 * @param size The size of the value.
 */
bool bst_insert(const void *value, struct bst **bst, size_t size) {
    return bst_insert_cmp(value, bst, size, memcomparator);
}

/**
 * Delete a value from a binary search tree using a comparator function.
 * @param value The value to delete.
 * @param bst The binary search tree.
 * @param size The size of the value.
 * @param cmp The comparator function.
 */
bool bst_delete_cmp(const void *value, struct bst **bst, size_t size, comparator cmp) {
    if (*bst == NULL) {
        return false;
    }

    if (cmp(value, (*bst)->value, size) == 0) {
        if ((*bst)->left == NULL && (*bst)->right == NULL) {
            free(*bst);
            *bst = NULL;
        } else if ((*bst)->left == NULL) {
            struct bst *temp = *bst;
            *bst = (*bst)->right;
            free(temp);
        } else if ((*bst)->right == NULL) {
            struct bst *temp = *bst;
            *bst = (*bst)->left;
            free(temp);
        } else {
            struct bst *temp = (*bst)->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            (*bst)->value = temp->value;
            bst_delete_cmp(temp->value, &(*bst)->right, size, cmp);
        }
        return true;
    }

    if (cmp(value, (*bst)->value, size) < 0) {
        return bst_delete_cmp(value, &(*bst)->left, size, cmp);
    } else {
        return bst_delete_cmp(value, &(*bst)->right, size, cmp);
    }
}

/**
 * Delete a value from a binary search tree.
 * @param value The value to delete.
 * @param bst The binary search tree.
 * @param size The size of the value.
 */
bool bst_delete(const void *value, struct bst **bst, size_t size) {
    return bst_delete_cmp(value, bst, size, memcomparator);
}

/**
 * Traverse a binary search tree in order (left, root, right).
 * @param bst The binary search tree.
 * @param f The function to apply to each value.
 */
size_t bst_inorder(const struct bst *bst, void (*f)(const void *, const void *), void *args) {
    if (bst == NULL) {
        return 0;
    }

    size_t n = 0;
    n += bst_inorder(bst->left, f, args);
    f(bst->value, args);
    n += bst_inorder(bst->right, f, args);
    return n + 1;
}

/**
 * Search for a value in a binary search tree using a comparator function.
 * @param value The value to search for.
 * @param bst The binary search tree.
 * @param size The size of the value.
 * @param cmp The comparator function.
 */
bool bst_search_cmp(const void *value, const struct bst *bst, size_t size, comparator cmp) {
    if (bst == NULL) {
        return false;
    }

    if (cmp(value, bst->value, size) == 0) {
        return true;
    }

    if (cmp(value, bst->value, size) < 0) {
        return bst_search_cmp(value, bst->left, size, cmp);
    } else {
        return bst_search_cmp(value, bst->right, size, cmp);
    }
}

/**
 * Search for a value in a binary search tree.
 * @param value The value to search for.
 * @param bst The binary search tree.
 * @param size The size of the value.
 */
bool bst_search(const void *value, const struct bst *bst, size_t size) {
    return bst_search_cmp(value, bst, size, memcomparator);
}

/**
 * Free a binary search tree.
 * @param bst The binary search tree (may be NULL).
 */
void bst_free(struct bst *bst) {
    if (bst == NULL) {
        return;
    }
    if (bst->left != NULL) {
        bst_free(bst->left);
    }
    if (bst->right != NULL) {
        bst_free(bst->right);
    }
    free(bst);
}

/**
 * Get the size of a binary search tree.
 * @param bst The binary search tree.
 */
size_t bst_size(const struct bst *bst) {
    if (bst == NULL) {
        return 0;
    } else {
        return 1 + bst_size(bst->left) + bst_size(bst->right);
    }
}

/**
 * Get the height of a binary search tree.
 * @param bst The binary search tree.
 */
size_t bst_height(const struct bst *bst) {
    if (bst == NULL) {
        return 0;
    } else {
        size_t left = bst_height(bst->left);
        size_t right = bst_height(bst->right);
        return 1 + (left > right ? left : right);
    }
}
