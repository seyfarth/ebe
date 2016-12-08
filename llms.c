#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node node;

struct node {
    int key;
    node *next;
};
    
node *build_list(void);
void print_list ( node *list );
node *merge_sort ( node *list );
node *merge ( node *list, node *tail );
node *split ( node *list );

int main ( void )
{
    node *list;
    node nd;
    nd.key = 12;
    nd.next = 0;

    list = build_list();
    list = merge_sort ( list );
    print_list ( list );
    print_list ( &nd );
    return 0;
}

node *split ( node *list )
{
    node *t, *tail;
    int i, n;

    n = 0;
    t = list;
    while ( t ) {
        n++;
        t = t->next;
    }

    t = list;
    for ( i = 2; i < n; i += 2 ) {
        t = t->next;
    }

    tail = t->next;
    t->next = NULL;

    return tail;
}

node *merge ( node *list, node *tail )
{
    node *start, *end;

    if ( list == NULL ) return tail;
    if ( tail == NULL ) return list;

    if ( list->key < tail->key ) {
        start = end = list;
        list = list->next;
    } else {
        start = end = tail;
        tail = tail->next;
    }

    while ( list && tail ) {
        if ( list->key < tail->key ) {
            end->next = list;
            list = list->next;
        } else {
            end->next = tail;
            tail = tail->next;
        }
        end = end->next;
    }

    if ( list ) end->next = list;
    else end->next = tail;

    return start;
}

node *merge_sort ( node *list )
{
    node *tail;

    if ( list && list->next ) {
        tail = split(list);
        list = merge_sort(list);
        tail = merge_sort(tail);
        return merge ( list, tail );
    } else {
        return list;
    }
}

node *build_list(void)
{
    node *list, *n;
    int k;

    list = NULL;

    while ( scanf("%d", &k) == 1 ) {
        n = calloc ( 1, sizeof(node) );
        n->key = k;
        n->next = list;
        list = n;
    }
    return list;
}

void print_list ( node *list )
{
    while ( list ) {
        printf("%d\n",list->key);
        list = list->next;
    }
}

