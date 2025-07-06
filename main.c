/* MARGARIT ANDREI - 315CD */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct page {
    int id;
    char url[50];
    char *description;
} page;

// STIVA

typedef struct stacknode {
    struct page elem;
    struct stacknode *next;
} stacknode;

typedef struct stack {
    stacknode* head;
    int size;
} stack;

stack* createStack(){
	stack *st;
	st = (stack*) malloc(sizeof(stack));
	st->head = NULL;
	st->size = 0;
	return st;
}

int isStackEmpty(stack *st) {
    if (st == NULL || st->head == NULL) {
        return 1;
    }
    return 0;
}

void push(stack *st, struct page elem) {
    stacknode *newNode = (stacknode*) malloc (sizeof(stacknode));
    newNode->elem.description = malloc((strlen(elem.description) + 1) * sizeof(char));
    strcpy(newNode->elem.description, elem.description);
    newNode->elem.id = elem.id;
    strcpy(newNode->elem.url, elem.url);
    newNode->next = st->head;
	st->head = newNode;
	st->size++;
}

void pop(stack *st) {
    if (st == NULL || st->head == NULL) {
        return;
    }
    stacknode *it = st->head;
    st->head = st->head->next;
    free(it->elem.description);
    free(it);
    st->size--;
}

page top(stack *st) {
    return st->head->elem;
}

void deleteContent(stack *st) {
    if (st == NULL || st->head == NULL) {
        return;
    }
    stacknode *it = st->head, *temp;
    while(it != NULL) {
        temp = it;
        it = it->next;
        free(temp->elem.description);
        free(temp);
    }
    st->head = NULL;
    st->size = 0;
}

// GATA CU STIVA

struct tab {
    int id;
    struct page *currentPage;
    struct stack *backwardStack;
    struct stack *forwardStack;
};

// LISTA DUBLU INLANTUITA CIRCULARA CU SANTINELA

typedef struct listNode{
	struct tab elem; 
	struct listNode *next; 
	struct listNode *prev; 
} listNode;

typedef struct tabsList {
    listNode *sentinel;
} tabsList;

typedef struct browser {
    struct tab *current;
    struct tabsList *list;
    int maxtabid;
} browser;

listNode* createNode() { 
    listNode *node = malloc(sizeof(listNode));
    node->elem.currentPage = malloc(sizeof(page));
    node->elem.id = -1;
    node->elem.currentPage->id = 0;
    node->elem.currentPage->description = malloc((strlen("Computer Science") + 1) * sizeof(char));
    strcpy(node->elem.currentPage->description, "Computer Science");
    strcpy(node->elem.currentPage->url, "https://acs.pub.ro/");
    node->elem.backwardStack = createStack();
    node->elem.forwardStack = createStack();
    node->prev = NULL;
    node->next = NULL;
    return node;
}

tabsList* createList() {
    tabsList* list = (tabsList*) malloc(sizeof(tabsList));
    list->sentinel = (listNode*) calloc(1, sizeof(listNode));
    list->sentinel->prev = list->sentinel;
    list->sentinel->next = list->sentinel;
    return list;
}

void create_newtab(browser *Browser) {
    listNode *node = createNode();
    node->prev = Browser->list->sentinel->prev;
    node->next = Browser->list->sentinel;
    Browser->list->sentinel->prev->next = node;
    Browser->list->sentinel->prev = node;
    Browser->maxtabid++;
    node->elem.id = Browser->maxtabid;
    Browser->current = &(Browser->list->sentinel->prev->elem);
}

void close_tab(browser *Browser, FILE *g) {
    listNode *temp = Browser->list->sentinel->next;
    if (Browser->current->id == 0) {
        fprintf(g,"403 Forbidden\n");
        return;
    }
    while (temp->elem.id != Browser->current->id) {
        temp = temp->next;
    }
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    Browser->current = &(temp->prev->elem); 
    free(temp->elem.currentPage->description);
    free(temp->elem.currentPage);
    deleteContent(temp->elem.backwardStack);
    free(temp->elem.backwardStack);
    deleteContent(temp->elem.forwardStack);
    free(temp->elem.forwardStack);
    free(temp);
}

void open_tab(browser *Browser, int id, FILE *g) {
    listNode *it = Browser->list->sentinel->next;
    while (it->elem.id != id && it != Browser->list->sentinel) {
        it = it->next;
    }
    if (it != Browser->list->sentinel && it->elem.id == id) {
        Browser->current = &(it->elem);
    } else {
        fprintf(g,"403 Forbidden\n");
        return;
    }
}

void next_tab(browser *Browser) {
    listNode *it = Browser->list->sentinel->next;
    while(it->elem.id != Browser->current->id) {
        it = it->next;
    }
    if (it->next == Browser->list->sentinel) {
        it = it->next;
    } 
    Browser->current = &(it->next->elem);
}

void prev_tab(browser *Browser) {
    listNode *it = Browser->list->sentinel->next;
    while(it->elem.id != Browser->current->id) {
        it = it->next;
    }
    if (it->prev == Browser->list->sentinel) {
        it = it->prev;
    } 
    Browser->current = &(it->prev->elem);
}

void open_page(browser *Browser, page *pages, int n, int id, FILE *g) {
    int i, pos = -1;
    for (i = 0; i < n; i++) {
        if (pages[i].id == id) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        fprintf(g, "403 Forbidden\n");
        return;
    }
    deleteContent(Browser->current->forwardStack);
    page oldPage;

    oldPage.id = Browser->current->currentPage->id;
    strcpy(oldPage.url, Browser->current->currentPage->url);
    oldPage.description = malloc((strlen(Browser->current->currentPage->description) + 1) * sizeof(char));
    strcpy(oldPage.description, Browser->current->currentPage->description);
    push(Browser->current->backwardStack, oldPage);
    free(oldPage.description);

    free(Browser->current->currentPage->description);
    Browser->current->currentPage->description = malloc((strlen(pages[pos].description) + 1)* sizeof(char));
    strcpy(Browser->current->currentPage->description, pages[pos].description);
    strcpy(Browser->current->currentPage->url, pages[pos].url);
    Browser->current->currentPage->id = pages[pos].id;
}

void afisare(browser *Browser, FILE *g) {
    listNode *it = Browser->list->sentinel->next;
    while (it->elem.id != Browser->current->id) {
        it = it->next;
    }
    listNode *start = it;
    do {
        fprintf(g,"%d ", it->elem.id);
        it = it->next;
        if (it == Browser->list->sentinel) {
            it = it->next;
        }
    } while (it != start);
    fprintf(g, "\n");
    fprintf(g, "%s\n", Browser->current->currentPage->description);
}

void afisare_pagini(browser *Browser, int id, FILE *g) {
    // caut tab-ul respectiv
    int n = 0, i, found = 0;
    listNode *it2 = Browser->list->sentinel->next;
    while (it2 != Browser->list->sentinel) {
        if (it2->elem.id == id) {
            found = 1;
            break;
        }
        it2 = it2->next;
    }
    if (!found) {
        fprintf(g, "403 Forbidden\n");
        return;
    }
    // incep cu stiva Forward in ordine de la ultimul element
    stacknode *it = it2->elem.forwardStack->head;
    while (it != NULL) {
        n++;
        it = it->next;
    }
    char urls[n][50];
    n = 0;
    it = it2->elem.forwardStack->head;
    while (it != NULL) {
        strcpy(urls[n], it->elem.url);
        n++;
        it = it->next;
    }
    for (i = n - 1; i >= 0; i--) {
        fprintf(g, "%s\n", urls[i]);
    }
    fprintf(g,"%s\n", it2->elem.currentPage->url);
    it = it2->elem.backwardStack->head;
    while (it != NULL) {
        fprintf(g,"%s\n", it->elem.url);
        it = it->next;
    }
    // continui cu elementul curent
    //termin cu stiva Backward in ordine de la primul element
}

void backward(browser *Browser, FILE *g) {
    if (isStackEmpty(Browser->current->backwardStack)) {
        fprintf(g, "403 Forbidden\n");
        return;
    }
    page current;
    current.id = Browser->current->currentPage->id;
    strcpy(current.url, Browser->current->currentPage->url);
    current.description = malloc((strlen(Browser->current->currentPage->description) + 1) * sizeof(char));
    strcpy(current.description, Browser->current->currentPage->description);
    push(Browser->current->forwardStack, current);
    free(current.description);

    page new = top(Browser->current->backwardStack);
    Browser->current->currentPage->id = new.id;
    free(Browser->current->currentPage->description);
    Browser->current->currentPage->description = malloc((strlen(new.description) + 1) * sizeof(char));
    strcpy(Browser->current->currentPage->description, new.description);
    strcpy(Browser->current->currentPage->url, new.url);
    pop(Browser->current->backwardStack);
}

void forward(browser *Browser, FILE *g) {
    if (isStackEmpty(Browser->current->forwardStack)) {
        fprintf(g, "403 Forbidden\n");
        return;
    }
    page current;
    current.id = Browser->current->currentPage->id;
    strcpy(current.url, Browser->current->currentPage->url);
    current.description = malloc((strlen(Browser->current->currentPage->description) + 1) * sizeof(char));
    strcpy(current.description, Browser->current->currentPage->description);
    push(Browser->current->backwardStack, current);
    free(current.description);

    page new = top(Browser->current->forwardStack);
    Browser->current->currentPage->id = new.id;
    free(Browser->current->currentPage->description);
    Browser->current->currentPage->description = malloc((strlen(new.description) + 1) * sizeof(char));
    strcpy(Browser->current->currentPage->description, new.description);
    strcpy(Browser->current->currentPage->url, new.url);
    pop(Browser->current->forwardStack);
}

// GATA CU LISTA DUBLU INLANTUITA CIRCULARA CU SANTINELA

browser* init_browser() {
    browser *Browser = (browser*) malloc(sizeof(browser));
    Browser->maxtabid = -1;
    Browser->list = createList();
    create_newtab(Browser);
    Browser->current = &(Browser->list->sentinel->prev->elem);
    return Browser;
}

// Functii pentru eliberarea memoriei/stergerea continutului

void freeTab(listNode *node) {
    free(node->elem.currentPage->description);
    free(node->elem.currentPage);
    deleteContent(node->elem.backwardStack);
    free(node->elem.backwardStack);
    deleteContent(node->elem.forwardStack);
    free(node->elem.forwardStack);
    free(node);
}

void freeList(tabsList *list) {
    listNode *it = list->sentinel->next, *temp;
    while (it != list->sentinel) {
        temp = it;
        it = it->next;
        freeTab(temp);
    }
    free(list->sentinel);
    free(list);
}

void freeBrowser(browser *Browser) {
    freeList(Browser->list);
    free(Browser);
}

int main() {
    browser *Browser = init_browser();
    FILE *f = fopen("tema1.in", "r");
    FILE *g = fopen("tema1.out", "w");
    int n, i, id, t;
    char line[255], *p;
    fscanf(f, "%d\n", &n);
    page *pages = (page *) malloc(n * sizeof(page));
    for (i = 0; i < n; i++) {
        fscanf(f, "%d\n", &id);
        pages[i].id = id;

        fgets(line, 255, f);
        line[strlen(line) - 1] = '\0';
        strcpy(pages[i].url, line);

        fgets(line, 255, f);
        line[strlen(line) - 1] = '\0';
        pages[i].description = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(pages[i].description, line);
    }
    fscanf(f, "%d\n", &t);
    for (i = 0; i < t; i++) {
        fgets(line, 255, f);
        if (strstr(line, "NEW_TAB")) {
            create_newtab(Browser);
        } else if (strstr(line, "CLOSE")) {
            close_tab(Browser, g);
        } else if (strstr(line, "OPEN")) {
            p = strtok(line, " ");
            p = strtok(NULL, " ");
            id = atoi(p);
            open_tab(Browser, id, g);
        } else if (strstr(line, "NEXT")) {
            next_tab(Browser);
        } else if (strstr(line, "PREV")) {
            prev_tab(Browser);
        } else if (strstr(line, "PAGE")) {
            p = strtok(line, " ");
            p = strtok(NULL, " ");
            id = atoi(p);
            open_page(Browser, pages, n, id, g);
        } else if (strstr(line, "BACKWARD")) {
            backward(Browser, g);
        } else if (strstr(line, "FORWARD")) {
            forward(Browser, g);
        } else if (strstr(line, "PRINT_HISTORY")) {
            p = strtok(line, " ");
            p = strtok(NULL, " ");
            id = atoi(p);
            afisare_pagini(Browser, id, g);
        } else {
            afisare(Browser, g);
            // PRINT
        }
    }
    fclose(f);
    fclose(g);
    for (i = 0; i < n; i++) {
        free(pages[i].description);
    }
    free(pages);
    freeBrowser(Browser);
    return 0;
}