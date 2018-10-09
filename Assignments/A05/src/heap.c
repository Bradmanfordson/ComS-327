#include "heap.h"
#include "macros.h"

#undef min

// General structure of the heap: The heap is made of heap_nodes(the struct below). Each 
// node has 4 pointers to other nodes, this is how everything is connected. These 4 pointers
// create two seperate lists. The "next" list and the "child" list. The "child" list is simple
// and just a list going from the beginning(min) to the last child. This list is traversed
// with [*parent] and [*child]. The "next" list is a little more complicated and contains 
// all of the values that have the same parent. One key feature of this list is that it is
// a loop aning that if you iterate with [*next] you will reach the current node again. This
// list can be iterated with [*next] and [*prev].
//
// Diagram of heap:
// Note: The items in brackets are nodes that the current node has pointers to.
//
//  [*parent]
//    |
//  [*prev] <- {curr_node} -> [*next]
//      |           |             |
//      |           |           next.child -> next.child.next
//      |           |
//      |         [*child] -> child.next -> child.next.next
//      |             |                               |
//      |             |                     child.next.next.child
//      |             |
//      |          child.child
//      |
//     prev.child -> prev.child-next
//
// Diagram of a "child" list:
//
//      parent.parent
//            |
//        [*parent]
//            |
//       {curr_node}
//            |
//        [*child]
//            |
//       child.child
//
// Diagram of a "next" list:
// Note: since the "next" list is a loop. next->next is connected to prev->prev
//
// prev.prev <- [*prev] <- {curr_node} -> [*next] -> next.next
//     ^                                                   |
//     |                                                   |
//     ----------------------<next>-------------------------

// Important struct only known to the heap functions.
struct heap_node {

  // The next node with the same parent.
  heap_node_t *next;

  // The previous node with the same parent.
  heap_node_t *prev;

  // The parent to this node. Min shouldn't have a child.
  heap_node_t *parent;

  // Child to the node. This only stores one of the children. To get to the other you
  // must iterate through *next.
  heap_node_t *child;

  // The main data stored in this node.
  void *datum;

  // The amount of children the node has.
  uint32_t degree;

  // Used with heap decrease key.
  uint32_t mark;
};

// Macro(like a function): splice_heap_node_lists
// If both n1 and n2 exist, connect n1 and n2 as [next] and [prev]. Then connect n1->next
// to n2->prev.
// from:
// [n1] -- n1->next
// n2->prev -- [n2]
// to:
// [n1] -- [n2]
// n2->prev -- n1->next
#define splice_heap_node_lists(n1, n2) ({ \
  if ((n1) && (n2)) {                     \
    (n1)->next->prev = (n2)->prev;        \
    (n2)->prev->next = (n1)->next;        \
    (n1)->next = (n2);                    \
    (n2)->prev = (n1);                    \
  }                                       \
})

// Macro: insert_heap_node_in_list
// Inserts node n into the list l is in. It is inserted before node l.
#define insert_heap_node_in_list(n, l) ({ \
  (n)->next = (l);                        \
  (n)->prev = (l)->prev;                  \
  (n)->prev->next = (n);                  \
  (l)->prev = (n);                        \
})

// Macro: remove_heap_node_from_list
// Removes the given node from the heap list. It does this by connecting the [next] and [prev]
// nodes together.
#define remove_heap_node_from_list(n) ({ \
  (n)->next->prev = (n)->prev;           \
  (n)->prev->next = (n)->next;           \
})

void print_heap_node(heap_node_t *n, unsigned indent,
                     char *(*print)(const void *v))
{
  heap_node_t *nc;

  printf("%*s%s\n", indent, "", print(n->datum));
  if (!(nc = n->child)) {
    return;
  }

  do {
    print_heap_node(nc, indent + 2, print);
    nc = nc->next;
  } while (nc != n->child);
}

void print_heap(heap_t *h, char *(*print)(const void *v))
{
  heap_node_t *n;

  if (h->min) {
    printf("size = %u\n", h->size);
    printf("min = ");
    n = h->min;
    do {
      print_heap_node(n, 0, print);
      n = n->next;
    } while (n != h->min);
  } else {
    printf("(null)\n");
  }
}

void print_heap_node_list(heap_node_t *n)
{
  heap_node_t *hn;

  if (!n) {
    return;
  }

  hn = n;
  do {
    printf("%p ", hn->datum);
    hn = hn->next;
  } while (hn != n);
  printf("\n");
}

// Inits heap
void heap_init(heap_t *h,
               int32_t (*compare)(const void *key, const void *with),
               void (*datum_delete)(void *))
{
  // Sets compare function and deatum_delete function to the given values.
  h->min = NULL;
  h->size = 0;
  h->compare = compare;
  h->datum_delete = datum_delete;
}

// Goes through heap and deletes hn from the heap.
void heap_node_delete(heap_t *h, heap_node_t *hn)
{
  heap_node_t *next;

  hn->prev->next = NULL;
  while (hn) {
    if (hn->child) {
      heap_node_delete(h, hn->child);
    }
    next = hn->next;
    // If a datum_delete was given, use it.
    if (h->datum_delete) {
      h->datum_delete(hn->datum);
    }
    free(hn);
    hn = next;
  }
}

// deletes entire heap. By setting everthing to null.
void heap_delete(heap_t *h)
{
  if (h->min) {
    heap_node_delete(h, h->min);
  }
  h->min = NULL;
  h->size = 0;
  h->compare = NULL;
  h->datum_delete = NULL;
}

// Inserts value into the heap. It does this by adding v into the the upper level list.
heap_node_t *heap_insert(heap_t *h, void *v)
{
  // Init new heap node with given values
  heap_node_t *n;

  n = calloc(1, sizeof (*n));
  n->datum = v;

  // If min exists add it to the top level list.
  if (h->min) {
    insert_heap_node_in_list(n, h->min);
  } else {
    // If min doesn't exists that means this is the first value so set it up like one.
    n->next = n->prev = n;
  }

  // Set n to min if it is the smallest value.
  if (!h->min || (h->compare(v, h->min->datum) < 0)) {
    h->min = n;
  }
  h->size++;

  return n;
}

void *heap_peek_min(heap_t *h)
{
  return h->min ? h->min->datum : NULL;
}

// Adds {node} as a child of {root} 
static void heap_link(heap_t *h, heap_node_t *node, heap_node_t *root)
{
  /*  remove_heap_node_from_list(node);*/

  // If the {root} node has a [child], insert {node} into the end of the [child] list.
  if (root->child) {
    insert_heap_node_in_list(node, root->child);
  } else {
    // If root has no [child] add {node} as the child of root.
    root->child = node;
    node->next = node->prev = node;
  }
  node->parent = root;
  root->degree++;
  node->mark = 0;
}

// Consolidates the heap.
static void heap_consolidate(heap_t *h)
{
  uint32_t i;
  heap_node_t *x, *y, *n;
  heap_node_t *a[64]; /* Need ceil(lg(h->size)), so this is good  *
                       * to the limit of a 64-bit address space,  *
                       * and much faster than any lg calculation. */

  // Inits array a with all 0s
  memset(a, 0, sizeof (a));

  // Changes min's [prev] node to point to nothing. This essentially cuts the "next" list
  // so that it doesn't loop
  h->min->prev->next = NULL;

  // This for loop essintaily makes the heap into a more tree like structure. It gives
  // the heap more depth by iterating though the top list and fusing together nodes with
  // the same amount of children.
  for (x = n = h->min; n; x = n) {

    // set n to the next value in the list, this is just to store it for the next iteration.
    n = n->next;

    // If a previous node has the same [degree] as the current node, make the greater one a 
    // child of the small node. Then check if this new node has the same [degree] of a previous 
    // node, repeat. Reminder, the entire structure is a heap and the [degree] is the amount of
    // children the node has.
    while (a[x->degree]) {
      y = a[x->degree];

      // If {x}'s value is greater, swap the values in {x} and {y}. This means that {x} contains 
      // the node with the smallest value.
      if (h->compare(x->datum, y->datum) > 0) {
        swap(x, y);
      }

      // Set the value at the current list to null
      a[x->degree] = NULL;

      // Make {y} a child of {x}.
      heap_link(h, y, x);
    }

    // Put current node into the list at the location of the amount of children it has.
    a[x->degree] = x;
  }

  // After the for loop above, you will be left with a few roots in the "a" list. One of
  // these is the min node. Go through the nodes and figure out which one is the root.
  // Add the others to the min list.
  for (h->min = NULL, i = 0; i < 64; i++) {
    if (a[i]) {
      if (h->min) {
        // If the min node exists, check if the current node is smaller. If it is swap them.
        insert_heap_node_in_list(a[i], h->min);
        if (h->compare(a[i]->datum, h->min->datum) < 0) {
          h->min = a[i];
        }
      } else {
        // If no min node is found yet, use this as the min node.
        h->min = a[i];
        a[i]->next = a[i]->prev = a[i];
      }
    }
  }
}

// removes the minimum node from the heap. A lot of other fancy stuff happens here as well.
void *heap_remove_min(heap_t *h)
{
  void *v;
  heap_node_t *n;

  v = NULL;

  // Something to note. When the heap is first created

  // if min exits
  if (h->min) {
    // set v to min's value
    v = h->min->datum;

    // if size of heap is zero that means we have no more heap, so free min and set min to null
    if (h->size == 1) {
      free(h->min);
      h->min = NULL;
    } else {

      // This basiclly just goes through min's children and sets their parent pointer to
      // null.
      if ((n = h->min->child)) {
        for (; n->parent; n = n->next) {
          n->parent = NULL;
        }
      }

      // Sets the [child] of min's [next]. This brings the rest min's child nodes
      // up a level to the top level and appends the min's original [next] list to the end.
      //
      // Example:
      // NOTE: [prev_1] is connected to [next_1] and [prev_2] is connected to [prev_2]. This is
      // because the [next] lists are a loop.
      // Before:
      // [prev_1] <- {min} -> [next_1]
      //               |
      // [prev_2] <- [child] -> [next_2]
      //
      // After:
      // [prev_1] <- {min} -> [child] -> [next_2] -> [prev_2] -> [next_1]
      //
      // TLDR: prepends the [child] list to the [next] list.
      splice_heap_node_lists(h->min, h->min->child);

      // Set n from the heap, make the child the new min.
      n = h->min;
      remove_heap_node_from_list(n);
      h->min = n->next;
      free(n);

      // TLDR: Makes the heap more heap like.
      heap_consolidate(h);
    }

    h->size--;
  }

  return v;
}

int heap_combine(heap_t *h, heap_t *h1, heap_t *h2)
{
  if (h1->compare != h2->compare ||
      h1->datum_delete != h2->datum_delete) {
    return 1;
  }

  h->compare = h1->compare;
  h->datum_delete = h1->datum_delete;

  if (!h1->min) {
    h->min = h2->min;
    h->size = h2->size;
  } else if (!h2->min) {
    h->min = h1->min;
    h->size = h1->size;
  } else {
    h->min = ((h->compare(h1->min->datum, h2->min->datum) < 0) ?
              h1->min                                          :
              h2->min);
    splice_heap_node_lists(h1->min, h2->min);
  }

  memset(h1, 0, sizeof (*h1));
  memset(h2, 0, sizeof (*h2));

  return 0;
}

// Removes n from p's child list and adds it to the upper level list(min's list).
static void heap_cut(heap_t *h, heap_node_t *n, heap_node_t *p)
{
  // If decrease the amount of children "p" has and if p has no more children, set child to null.
  if (!--p->degree) {
    p->child = NULL;
  }

  // If the direct child of "p" is "n" set p's child to the next child.
  if (p->child == n) {
    p->child = p->child->next;
  }

  // Removes "n" from p's child list.
  remove_heap_node_from_list(n);
  n->parent = NULL;
  n->mark = 0;

  // Inserts "n" into the upper level list.
  insert_heap_node_in_list(n, h->min);
}

// heap cuts node n up until it doesn't have any parents left.
static void heap_cascading_cut(heap_t *h, heap_node_t *n)
{
  heap_node_t *p;

  if ((p = n->parent)) {
    if (!n->mark) {
      n->mark = 1;
    } else {
      heap_cut(h, n, p);
      heap_cascading_cut(h, n);
    }
  }
}

int heap_decrease_key(heap_t *h, heap_node_t *n, void *v)
{
  if (h->compare(n->datum, v) <= 0) {
    return 1;
  }

  if (h->datum_delete) {
    h->datum_delete(n->datum);
  }
  n->datum = v;

  return heap_decrease_key_no_replace(h, n);
}

// Updates the heap after n's value was decreased. Use this after you decrease the
// value that is pointed to by n.
int heap_decrease_key_no_replace(heap_t *h, heap_node_t *n)
{
  /* No tests that the value hasn't actually increased.  Change *
   * occurs in place, so the check is not possible here.  The   *
   * user is completely responsible for ensuring that they      *
   * don't fubar the queue.                                     */

  heap_node_t *p;

  // Get the parent of the given node
  p = n->parent;

  // If the parent to n exists and is larger than n, ...
  if (p && (h->compare(n->datum, p->datum) < 0)) {

    // Moves the node n from p's child list and adds to the upper level list.
    heap_cut(h, n, p);
    // Pushes node n up until it doesn't have any more parents.
    heap_cascading_cut(h, p);
  }

  // If node n is smaller than the current min, make n min.
  if (h->compare(n->datum, h->min->datum) < 0) {
    h->min = n;
  }

  return 0;
}

#ifdef TESTING

int32_t compare(const void *key, const void *with)
{
  return *((int *) key) - *((int *) with);
}

char *print_int(const void *v)
{
  static char out[640];

  snprintf(out, 640, "%d", *((int *) v));

  return out;
}

int main(int argc, char *argv[])
{
  heap_t h;
  int **keys;
  heap_node_t **a;
  /*  int *p;*/
  int i, j;
  int n;

  if (argc == 2) {
    n = atoi(argv[1]);
  } else {
    n = 20;
  }

  keys = calloc(n, sizeof (*keys));
  a = calloc(n, sizeof (*a));

  heap_init(&h, compare, free);

  for (i = 0; i < n; i++) {
    keys[i] = malloc(sizeof (*keys[i]));
    *keys[i] = i;
    a[i] = heap_insert(&h, keys[i]);
  }

  print_heap(&h, print_int);
  printf("------------------------------------\n");

  heap_remove_min(&h);
  keys[0] = malloc(sizeof (*keys[0]));
  *keys[0] = 0;
  a[0] = heap_insert(&h, keys[0]);
  for (i = 0; i < 100 * n; i++) {
    j = rand() % n;
    /*    p = malloc (sizeof (*p));*/
    (*(int *) a[j]->datum)--;
    /*    (*p)--;*/
    heap_decrease_key_no_replace(&h, a[j]);
    print_heap(&h, print_int);
    printf("------------------------------------\n");
  }

  free(keys);

  return 0;
}

#endif
