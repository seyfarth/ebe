#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct treenode treenode;

struct treenode {
    treenode *left;
    int  key;
    treenode *right;
};

typedef struct junk junk;

struct junk {
    junk *left, *right;
    int  key;
};

typedef struct header header, *tree;

struct header {
    treenode *root;
    int count;
    int values[2][3];
};

int stuff=6;
//long junk=9;
double x=1.1;

tree create_tree ( void );
treenode *create_node ( int key );
void insert ( tree t, int key );
void print_tree ( tree t );
void print_nodes ( treenode *r, int level );

void insert ( tree t, int key )
{
    treenode *x, *p;

    if ( t->root == NULL ) {
        t->root = create_node ( key );
    } else {
        x = t->root;
        while ( x ) {
            p = x;
            if ( key < x->key ) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        if ( key < p->key ) {
            p->left = create_node ( key );
        } else {
            p->right = create_node ( key );
        }
    }
    t->count++;
}

tree create_tree ( void )
{
    tree t;

    t = calloc ( 1, sizeof(header) );
    assert ( t );
    return t;
}

treenode *create_node ( int key )
{
    treenode *x;

    x = calloc ( 1, sizeof(treenode) );
    assert ( x );
    x->key = key;
    return x;
}

void print_tree ( tree t )
{
    printf("Count: %d\n", t->count );
    print_nodes ( t->root, 0 );
}

void print_nodes ( treenode *r, int level )
{
    int i;

    if ( r ) {
        print_nodes ( r->left, level+1 );
        for ( i = 0; i < level; i++ ) printf("  ");
        printf("%d\n",r->key);
        print_nodes ( r->right, level+1 );
    }
}
  
int main ( int argc, char *argv[] )
{
    int key;
    tree t;
    junk j = {0,0,0};

    t = create_tree();
    while ( scanf("%d",&key) == 1 ) {
        insert ( t, key );
        print_tree ( t );
    }
    printf("j %d\n", j.key);
}
