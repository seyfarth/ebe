#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct tree_t *tree;

typedef struct node_t node;

struct node_t {
    node *left;
    node *right;
    int  key;
};

struct tree_t {
    node *root;
    int  count;
};

tree create_tree ( void );
void print_tree ( tree t );
void rec_print_tree ( node *root );
void add_key ( tree t, int n );

int main ( int argc, char **argv )
{
    tree t, s;
    int n;

    printf("sizeof = %ld\n", sizeof(*t));

    t = create_tree();
    printf("t is %p\n", t );
    s = create_tree();
    printf("s is %p\n", s );

    while ( scanf("%d",&n) == 1 ) {
        add_key ( t, n );
        print_tree ( t );
    }
    return 0;
}
tree create_tree ( void )
{
    tree t;

    t = calloc ( 1, sizeof(*t) );
    if ( t == NULL ) {
        fprintf(stderr,"Could not create a tree\n");
        exit(1);
    }
    return t;
}

void print_tree ( tree t )
{
    assert ( t );

    rec_print_tree ( t->root );
}

void rec_print_tree ( node *root )
{
    static int level = 0;
    int i;

    level++;
    if ( root ) {
        rec_print_tree ( root->left );
        for ( i = 0; i < level; i++ ) printf("...");
        printf ( " %d\n", root->key );
        rec_print_tree ( root->right );
    }
    level--;
}

void add_key ( tree t, int n )
{
    node *p, *new;

    new = calloc ( 1, sizeof(node) );
    assert ( new );
    new->key = n;

    t->count++;

    if ( t->root == NULL ) {
        t->root = new;
    } else {
        p = t->root;
        while ( p ) {
            if ( n < p->key ) {
                if ( p->left ) {
                    p = p->left;
                } else {
                    p->left = new;
                    break;
                }
            } else {
                if ( p->right ) {
                    p = p->right;
                } else {
                    p->right = new;
                    break;
                }
            }
        }
    }
}

