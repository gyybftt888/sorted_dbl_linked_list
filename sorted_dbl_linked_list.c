#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERR_BASE 0
#define ERR_DUPLICATE ERR_BASE - 1
#define ERR_NOT_FOUND ERR_BASE - 2
#define ERR_OUT_OF_MEM ERR_BASE - 3
#define ERR_OPEN_FAIL ERR_BASE - 4
#define ERR_LOAD_FAIL ERR_BASE - 5
#define ERR_SAVE_FAIL ERR_BASE - 6

#define TYPE_FROM_HEAD 1
#define TYPE_FROM_TAIL 2

typedef struct dlist
{
    int element;
    struct dlist *next, *prev;
} dlist;

int add(int);
int del(int);
int dump(int);
int save(char *);
int load(char *);
int clear();

dlist *g_head = NULL, *g_tail = NULL;

int main()
{
    int status = 0;
    add(3);
    add(5);
    add(1);
    add(4);
    add(2);
    add(6);
    del(5);
    del(1);
    del(3);
    del(1);
    // dump(TYPE_FROM_TAIL);
    save("test.txt");
    clear();
    add(7);
    load("test.txt");
    dump(TYPE_FROM_HEAD);
    return SUCCESS;
}

int add(int element)
{
    dlist *newdlist = (dlist *)malloc(sizeof(dlist));
    newdlist->element = element;
    newdlist->prev = NULL;
    newdlist->next = NULL;
    if (g_head == NULL && g_tail == NULL)
    {
        g_head = newdlist;
        g_tail = newdlist;
        return SUCCESS;
    }
    dlist *current = g_head;
    while (current != NULL)
    {
        // element existed
        if (current->element == element)
        {
            printf("error = %d, while add.\n", ERR_DUPLICATE);
            return 1;
        }
        // element is smallest
        else if (current->element > element)
        {
            newdlist->next = current;
            current->prev = newdlist;
            g_head = newdlist;
            return SUCCESS;
        }
        // element is medium
        else if (current->next != NULL && current->next->element > element)
        {
            newdlist->prev = current;
            newdlist->next = current->next;
            current->next->prev = newdlist;
            current->next = newdlist;
            return SUCCESS;
        }
        // element is largest
        else if (current->next == NULL)
        {
            newdlist->prev = current;
            current->next = newdlist;
            g_tail = newdlist;
            return SUCCESS;
        }
        current = current->next;
    }
}

int del(int element)
{
    if (g_head == NULL && g_tail == NULL)
    {
        printf("error = %d, while del.\n", ERR_NOT_FOUND);
        return 1;
    }
    dlist *current = g_head;
    while (current != NULL)
    {
        if (current->element == element)
        {
            if (current == g_head)
            {
                g_head = current->next;
                g_head->prev = NULL;
            }
            else if (current == g_tail)
            {
                g_tail = current->prev;
                g_tail->prev = NULL;
            }
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            free(current);
            return SUCCESS;
        }
        current = current->next;
    }
    printf("error = %d, while del.\n", ERR_NOT_FOUND);
    return 1;
}

int dump(int type)
{
    if (g_head == NULL & g_tail == NULL)
    {
        printf("error = %d, while dump.\n", ERR_NOT_FOUND);
        return 0;
    }
    else
    {
        if (type == TYPE_FROM_HEAD)
        {
            dlist *current = g_head;
            while (current != NULL)
            {
                if (current != g_tail)
                {
                    printf("%d,", current->element);
                }
                else
                {
                    printf("%d", current->element);
                }
                current = current->next;
            }
        }
        else if (type == TYPE_FROM_TAIL)
        {
            dlist *current = g_tail;
            while (current != NULL)
            {
                if (current != g_head)
                {
                    printf("%d,", current->element);
                }
                else
                {
                    printf("%d", current->element);
                }
                current = current->prev;
            }
        }
        return SUCCESS;
    }
}
int save(char *filename)
{
    FILE *fptr;
    fptr = fopen(filename, "w");
    dlist *current = g_head;
    while (current != NULL)
    {
        fprintf(fptr, "%d\n", current->element);
        current = current->next;
    }
    fclose(fptr);
    return SUCCESS;
}

int load(char *filename)
{
    clear();
    FILE *fptr;
    fptr = fopen(filename, "r");
    int element;
    while (fscanf(fptr, "%d", &element) != EOF)
    {
        add(element);
    }
    fclose(fptr);
    return SUCCESS;
}

int clear()
{
    dlist *current = g_head;
    while (current != NULL)
    {
        dlist *temp = current->next;
        free(current);
        current = temp;
    }
    g_head = NULL;
    g_tail = NULL;
    return SUCCESS;
}
