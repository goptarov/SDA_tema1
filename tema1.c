#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
	struct page* data;
	struct node* next;
}node;

typedef struct dnode {
	struct tab* data;
	struct dnode* next;
	struct dnode* prev;
}dnode;

typedef struct stack {
	node* top;
}stack;

typedef struct tabsList {
	dnode* head;
	int totLen;
}tabsList;

typedef struct page {
	int id;
	char url[50];
	char *description;
}page;

typedef struct tab {
	int id;
	page *currentPage;
	stack *backwardStack;
	stack *forwardStack;
}tab;

typedef struct browser {
	tab* current;
	tabsList list;
}browser;


void initStack(stack* s) {
	s->top = NULL;
}

void initList(tabsList* list) {
	page *implicit = malloc(sizeof(page));
	implicit->id = 0;
	strcpy(implicit->url, "https://acs.pub.ro/");
	implicit->description = malloc(sizeof(char) * 50);
	strcpy(implicit->description, "Computer Science");

	tab* tab0 = malloc(sizeof(tab));
	memset(tab0, 0, sizeof(tab));
	tab0->id = 0;
	tab0->currentPage = implicit;
	tab0->backwardStack = malloc(sizeof(stack));
	tab0->forwardStack = malloc(sizeof(stack));
	initStack(tab0->backwardStack);
	initStack(tab0->forwardStack);

	dnode* dnode0 = malloc(sizeof(dnode));
	dnode0->data = tab0;

	list->head = malloc(sizeof(dnode));
	list->head->next = dnode0;
	list->head->prev = dnode0;
	list->head->data = NULL;
	list->totLen = 1;
	dnode0->next = list->head;
	dnode0->prev = list->head;
}

void initBrowser(browser** b, tabsList* list) {
	*b = malloc(sizeof(browser));
	initList(list);
	(*b)->list = *list;
	(*b)->current = list->head->next->data;
}

void push(stack* s, page* p) {
	node* newNode = malloc(sizeof(node));
	newNode->data = p;
	newNode->next = s->top;
	s->top = newNode;
}

page* pop(stack* s) {
	if (s->top == NULL) return NULL;
	node* temp = s->top;
	page* data = temp->data;
	s->top = s->top->next;
	free(temp);
	return data;
}

void new_tab(tabsList* list, browser* b) {
	page *implicit = malloc(sizeof(page));
	implicit->id = 0;
	strcpy(implicit->url, "https://acs.pub.ro/");
	implicit->description = malloc(sizeof(char) * 50);
	strcpy(implicit->description, "Computer Science");

	tab* newTab = malloc(sizeof(tab));
	memset(newTab, 0, sizeof(tab));
	newTab->id = list->totLen;
	newTab->currentPage = implicit;
	newTab->backwardStack = malloc(sizeof(stack));
	newTab->forwardStack = malloc(sizeof(stack));
	initStack(newTab->forwardStack);
	initStack(newTab->backwardStack);

	dnode* newNode = malloc(sizeof(dnode));
	newNode->data = newTab;

	dnode* old = list->head->prev;
	old->next = newNode;

	newNode->prev = old;
	newNode->next = list->head;
	list->head->prev = newNode;
	list->totLen++;

	b->current = newTab;
}

void close(tabsList* list, browser* b, FILE* out) {
	if (b->current->id == 0) {
		fprintf(out, "403 Forbidden\n");
		return;
	}

	dnode* p = list->head->next;
	while (p->data != b->current) {
		p = p->next;
	}
	p->prev->next = p->next;
	p->next->prev = p->prev;
	b->current = p->prev->data;

}

void openTab(int tabid, browser* b, FILE* out) {
	dnode* p = b->list.head->next;
	while(p->data->id != tabid) {
		if(p->data->id > tabid || p->next == b->list.head) {
			fprintf(out, "403 Forbidden\n");
			return;
		}
		p = p->next;
	}
	b->current = p->data;
}

void next(browser* b, tabsList list) {
	dnode* p = list.head->next;
	while (p->data != b->current) {
		p = p->next;
	}
	if (p->next->data == list.head->data)
		b->current = p->next->next->data;
	else b->current = p->next->data;
}

void prev(browser* b, tabsList list) {
	dnode* p = list.head->prev;
	while (p->data != b->current) {
		p = p->prev;
	}
	if (p->prev->data == list.head->data)
		b->current = p->prev->prev->data;
	else b->current = p->prev->data;
}

void openPage(int pageid, browser* b, page pageList[50], FILE* out) {
	int i = 0;

	for (i = 0; i < 50; i++) {
		if (pageList[i].id == pageid) {
			break;
		}
	}
	if (i == 50) {
		fprintf(out, "403 Forbidden\n");
	}

	push(b->current->backwardStack, b->current->currentPage);

	b->current->currentPage = &pageList[i];
}

void backward(browser* b, FILE* out) {
	if (b->current->backwardStack->top == NULL) {
		fprintf(out, "403 Forbidden\n");
		return;
	}
	push(b->current->forwardStack, b->current->currentPage);

	b->current->currentPage = pop(b->current->backwardStack);
}

void forward(browser* b, FILE* out) {
	if (b->current->forwardStack->top == NULL) {
		fprintf(out, "403 Forbidden\n");
		return;
	}
	push(b->current->backwardStack, b->current->currentPage);

	b->current->currentPage = pop(b->current->forwardStack);
}

void print(browser b, FILE* out) {
	dnode* p = b.list.head->next;
	while (p != b.list.head) {
		if (p->data == b.current)
			break;
		p = p->next;
	}
	fprintf(out, "%d ", p->data->id);
	p = p->next;

	while (p->data != b.current) {
		if (p == b.list.head)
			p=p->next;
		else {
			if (p->next->data == b.current)
				fprintf(out, "%d", p->data->id);
			else
				fprintf(out, "%d ", p->data->id);
			p = p->next;
		}
	}

	fprintf(out, "\n");
	for (int i = 0; i < strlen(b.current->currentPage->description); i++) {
		fprintf(out, "%c", b.current->currentPage->description[i]);
	}
	if(strlen(b.current->currentPage->description) != 0) fprintf(out, "\n");
}

void recursivePrint(node* n, FILE* out) {
	if (n == NULL) return;
	if (n->next != NULL) {
		recursivePrint(n->next, out);
	}
	fprintf(out, "%s\n", n->data->url);
}

void printHistory(int tabid, browser b, FILE* out) {
	dnode* p = b.list.head->next;
	while (p->data->id != tabid) {
		p = p->next;
		if (p == b.list.head) {
			fprintf(out, "403 Forbidden\n");
			return;
		}
	}

	node* n = p->data->forwardStack->top;
	node* m = p->data->backwardStack->top;
	recursivePrint(n, out);
	fprintf(out, "%s\n", p->data->currentPage->url);
	if (m == NULL) return;
	else {
		while (m->next != NULL) {
			fprintf(out, "%s\n", m->data->url);
			m = m->next;
		}
		fprintf(out, "%s\n", m->data->url);
	}
}

//Free-ing functions for memory cleanup
void freeStack(stack* s) {
	node* p = s->top;
	while (p) {
		node* tmp = p;
		p = p->next;
		free(tmp);
	}
	free(s);
}

void freeTab(tab* t, page pageList[50]) {
	if (t->currentPage) {
		free(t->currentPage->description);
		free(t->currentPage);
	}
	freeStack(t->forwardStack);
	freeStack(t->backwardStack);
	free(t);
}

void freeList(tabsList* list, page pageList[50]) {
	dnode* p = list->head->next;
	while (p != list->head) {
		dnode* tmp = p;
		p = p->next;
		freeTab(tmp->data, pageList);
		free(tmp);
	}
	free(list->head);
}


int main() {

	FILE* in = fopen("tema1.in", "r");
	if (!in) {
		perror("Failed to open input file");
		return 1;
	}
	FILE* out = fopen("tema1.out","w");
	tabsList* list = malloc(sizeof(tabsList));
	browser* b = {0};
	int  pageNr=0, operationNr=0;
	page pageList[50] = {0};

	initBrowser(&b, list);

	fscanf(in, "%d", &pageNr);
	for (int i = 0; i < pageNr; i++) {
		fscanf(in, "%d", &pageList[i].id);
		fscanf(in, "%s", pageList[i].url);
		fgetc(in);
		pageList[i].description = malloc(sizeof(char) * 51);
		fgets(pageList[i].description, 50, in);
		pageList[i].description[strlen(pageList[i].description) - 1] = '\0';
	}

	fscanf(in, "%d", &operationNr);
	char operation[30];

	for (int i = 0; i < operationNr; i++) {
		int j;
		fscanf(in, "%s %d", operation, &j);
		if (strcmp(operation, "NEW_TAB") == 0) {
			new_tab(list, b);
		}
		if (strcmp(operation, "CLOSE") == 0) {
			close(list, b, out);
		}
		if (strcmp(operation, "OPEN") == 0) {
			openTab(j, b, out);
		}
		if (strcmp(operation, "NEXT") == 0) {
			next(b, *list);
		}
		if (strcmp(operation, "PREV") == 0) {
			prev(b, *list);
		}
		if (strcmp(operation, "PAGE") == 0) {
			openPage(j, b, pageList, out);
		}
		if (strcmp(operation, "FORWARD") == 0) {
			forward(b, out);
		}
		if (strcmp(operation, "BACKWARD") == 0) {
			backward(b, out);
		}
		if (strcmp(operation, "PRINT") == 0) {
			print(*b, out);
		}
		if (strcmp(operation, "PRINT_HISTORY") == 0) {
			printHistory(j, *b, out);
		}
	}

	fclose(in);
	fclose(out);

	for (int i = 0; i < pageNr; i++)
		free(pageList[i].description);

	freeList(list, pageList);
	free(b);
	free(list);
}
