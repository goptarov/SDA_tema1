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

void initList(tabsList* list) { //should all of this really be in the list init?
	page *implicit = malloc(sizeof(page));
	implicit->id = 0;
	strcpy(implicit->url, "https://acs.pub.ro");
	implicit->description = malloc(sizeof(char) * 50);
	strcpy(implicit->description, "Computer Science");

	tab* tab0 = malloc(sizeof(tab));
	dnode* dnode0 = malloc(sizeof(dnode));
	tab0->id = 0;

	tab0->currentPage = implicit;

	tab0->backwardStack = malloc(sizeof(stack));
	tab0->forwardStack = malloc(sizeof(stack));
	initStack(tab0->backwardStack);
	initStack(tab0->forwardStack);

	dnode0->data = tab0;

	list->head = malloc(sizeof(dnode));
	list->head->next = dnode0;
	list->head->prev = dnode0;
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

void new_tab(tabsList* list, browser* b, int closedTabs) {
	page *implicit = malloc(sizeof(page));
	implicit->id = 0;
	strcpy(implicit->url, "https://acs.pub.ro");
	implicit->description = malloc(sizeof(char) * 50);
	strcpy(implicit->description, "Computer Science");

	tab* newTab = malloc(sizeof(tab));
	newTab->id = list->head->prev->data->id + 1 + closedTabs;
	newTab->currentPage = implicit;
	newTab->backwardStack = malloc(sizeof(stack));
	newTab->forwardStack = malloc(sizeof(stack));

	b->current = newTab;

	dnode* old = list->head->prev;
	dnode* newNode = malloc(sizeof(dnode));
	newNode->data = newTab;
	old->next = newNode;
	list->head->prev = newNode;
	list->head->prev->next = list->head;
	list->head->prev->prev = old;

	initStack(b->current->forwardStack);
	initStack(b->current->backwardStack);
}

void close(tabsList* list, browser* b, FILE* out) { //does the next tab recieve this one's id and so on? hope not
	if (b->current->id == 0) {
		fprintf(out, "403 Forbidden\n");
		return;
	}

	list->head->prev = list->head->prev->prev;
	list->head->prev->next = list->head;

	b->current = list->head->prev->data;
}

void openTab(int tabid, browser* b, FILE* out) {
	printf("helo");
	dnode* p = b->list.head->next;
	while(p->data->id != tabid) {
		p = p->next;
		if(p->data->id > tabid) {
			printf("helo");
			fprintf(out, "403 Forbidden\n");
			return;
		}
	}
	b->current = p->data;
}

void next(browser* b, tabsList list) {
	dnode* p = list.head->next;
	while (p->data != b->current) {
		p = p->next;
	}
	b->current = p->data;
}

void prev(browser* b, tabsList list) {
	dnode* p = list.head->prev;
	while (p->data != b->current) {
		p = p->prev;
	}
	b->current = p->data;
}

void openPage(int pageid, browser* b, page pageList[10], FILE* out) {
	int i;
	for (i = 0; i < 10; i++) {
		if (pageid == pageList[i].id)
			break;
	}
	if (pageid != pageList[i].id) {
		fprintf(out, "403 Forbidden\n");
	}
	push(b->current->backwardStack, b->current->currentPage);
	//b->current->currentPage = pop(b->current->forwardStack);
	b->current->currentPage = &pageList[i];
}

void backward(browser* b, FILE* out) {
	if (b->current->backwardStack->top->next == NULL) {
		fprintf(out, "403 Forbidden\n");
		return;
	}
	//b->current->forwardStack->top->data = b->current->currentPage;
	//b->current->forwardStack->top->next = b->current->forwardStack->top;
	push(b->current->forwardStack, b->current->currentPage);

	b->current->currentPage = b->current->backwardStack->top->data;

	//b->current->backwardStack->top = b->current->backwardStack->top->next;
	pop(b->current->backwardStack);
}

void forward(browser* b, FILE* out) {
	if (b->current->forwardStack->top->next == NULL) {
		fprintf(out, "403 Forbidden\n");
		return;
	}
	//b->current->backwardStack->top->data = b->current->currentPage;
	//b->current->backwardStack->top->next = b->current->backwardStack->top;
	push(b->current->backwardStack, b->current->currentPage);

	b->current->currentPage = b->current->forwardStack->top->data;

	//b->current->forwardStack->top = b->current->forwardStack->top->next;
	pop(b->current->forwardStack);
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
	fprintf(out, "\n");
}

void recursivePrint(node* n, FILE* out) {
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
	while (m->next != NULL) {
		fprintf(out, "%s\n", m->data->url);
		m = m->next;
	}
	fprintf(out, "%s\n", m->next->data->url);

}

int main() {

	FILE* in = fopen("tema1.in", "r");
	if (!in) {
		perror("Failed to open input file");
		return 1;
	}
	FILE* out = fopen("tema1.out","w");
	tabsList* list = malloc(sizeof(tabsList));
	browser* b;
	int tabNr = 0, pageNr, operationNr;
	page pageList[10];

	initBrowser(&b, list);

	fscanf(in, "%d", &pageNr);
	for (int i = 0; i < pageNr; i++) {

		fscanf(in, "%d", &pageList[i].id);
		fscanf(in, "%s", pageList[i].url);
		fgetc(in);
		pageList[i].description = malloc(sizeof(char) * 51);
		fgets(pageList[i].description, 50, in);
	}

	fscanf(in, "%d", &operationNr);
	char operation[20];
	int closedTabs = 0;

	for (int i = 0; i < operationNr; i++) {
		int j;
		fscanf(in, "%s %d", operation, &j);
		if (strcmp(operation, "NEW_TAB") == 0) {
			new_tab(list, b, closedTabs);
		}
		if (strcmp(operation, "CLOSE") == 0) {
			close(list, b, out);
			closedTabs++;
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

	free(list);
	free(b);

}
