
#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct Element {
	char *name;
	char *city;
	int vic, los, draw, score;
	struct Element *next;
}Element;

Element* CreateElement()
{
	Element *E = (Element*)malloc(sizeof(Element));
	return E;
}

void FreeElement(Element *E)
{
	free(E->name);
	free(E->city);
	free(E);
}

void fill(char *str)
{
	char c;
	int b = 1, i = 0, len = 100;
	while (b)
	{
		c = getchar();
		if (c != '\n' && i < len)
			str[i++] = c;
		else
			str[i] = b = 0;
	}
}


void init_elem(Element *E, const char *nam,const char *town,
	int v, int l, int d, int s)
{
	int n = 100;
	E->name = (char*)malloc(n * sizeof(char));
	E->city = (char*)malloc(n * sizeof(char));
	strcpy_s(E->name, 100, nam);
	strcpy_s(E->city, 100, town);
	E->vic = v;
	E->los = l;
	E->draw = d;
	E->score = s;

	E->next = NULL;
}

void print_elem(Element *E)
{
	if (E == NULL) {
		printf("NULL");
		return;
	}
	printf("The Team '%s' from '%s'\n", E->name, E->city);
	printf("Victories: %d\nLosses: %d\nDraws: %d\nScore in total: %d", E->vic, E->los, E->draw, E->score);
	printf("\n");
}

typedef struct List {
	Element *First;
}List;

List* CreateList()
{
	List* L = (List*)malloc(sizeof(List));
	L->First = NULL;
	return L;
}

void FreeList(List *L)
{
	Element *E = L->First;
	while (E) {
		FreeElement(E);
		E = E->next;
	}
	free(L);
}

void print_list(List *L)
{
	Element *E = L->First;
	printf("List of Teams:\n\n");
	while (E)
	{
		print_elem(E);
		printf("\n");
		E = E->next;
	}
}

Element* findN(List *L, int n)//���������� N-��� ��������
{
	Element *E = L->First;
	int counter = 0;
	while (counter < n && L->First) {
		E = E->next;
		counter++;
	}
	return E;
}

Element* getLast(List *L)//���������� ���������� ��������
{
	Element *E = L->First;
	if (E == NULL)
		return NULL;
	while (E->next) {	
		E = E -> next;
	}
	return E;
}

void add(List *L, Element *E)//���������� �������� � ����� ������
{
	E->next = NULL;
	Element *last = getLast(L);
	if (last == NULL) {
		L->First = E;
		return;
	}
	last->next = E;
}

void pushBack(List *L, Element *E)//���������� � ������ ������
{
	E->next = L->First;
	L->First = E;
}

void insert(List *L, Element *E, int n) 
{
	Element *tmp = findN(L, n);
	if (tmp == NULL) return;
	E->next = tmp->next;
	tmp->next = E;
}

void remove(List *L, int n) 
{
	if (n == 0) {
		if (L->First == NULL) return;
		else {
			Element *E = L->First;
			L->First = L->First->next;
			free(E);
			return;
		}
	}
	Element *prev = findN(L, n - 1);
	if (prev == NULL) return;
	Element *E = prev->next;
	if (E == NULL) return;
	prev->next = E->next;
	free(E);
}

List* load(const char *filename)
{
	FILE* f;
	fopen_s(&f, filename, "r");
	if (f == 0) return 0;

	List *L = CreateList();
	int nTeams;
	fscanf_s(f, "%d", &nTeams);
	for (int i = 0; i < nTeams; i++)
	{
		Element *E = CreateElement();

		int n = 100;
		E->name = (char*)malloc(n * sizeof(char));
		E->city = (char*)malloc(n * sizeof(char));
		char str[100] = {};
		fscanf_s(f, "%s", str, 100);
		memcpy(E->name, str, strlen(str) + 1);
		char str1[100] = {};
		fscanf_s(f, "%s", str1, 100);
		memcpy(E->city, str1, strlen(str1) + 1);
		strcpy_s(E->name, sizeof(str), str);
		fscanf_s(f, "%d", &(E->vic));
		fscanf_s(f, "%d", &(E->los));
		fscanf_s(f, "%d", &(E->draw));
		fscanf_s(f, "%d", &(E->score));

		pushBack(L, E);
	}

	fclose(f);
	return L;
}

int save(const char *filename, List *L)
{
	FILE* f;
	fopen_s(&f, filename, "w");

	if (f == 0) return 0;

	Element *E = L->First;
	while (E)
	{
		fprintf_s(f, "The Team '%s' from '%s'\n", E->name, E->city);
		fprintf_s(f, "Victories: %d\nLosses: %d\nDraws: %d\nScore in total: %d\n",
			E->vic, E->los, E->draw, E->score);
		E = E->next;
	}

	fclose(f);
	return 1;
}

List* filterlessWins(List *L, int wins)
{
	List *L1 = CreateList();
	Element *E = L->First;
	while (E) 
	{
		if (E->vic < wins)
		{
			Element *tmp = CreateElement();
			tmp->vic = E->vic;
			tmp->los = E->los;
			tmp->draw = E->draw;
			tmp->score = E->score;
			tmp->name = (char*)malloc((strlen(E->name)+1) * sizeof(char));
			memcpy(tmp->name, E->name, (strlen(E->name)+1));
			tmp->city = (char*)malloc((strlen(E->city) + 1) * sizeof(char));
			memcpy(tmp->city, E->city, (strlen(E->city) + 1));
			pushBack(L1, tmp);

		}
		E = E->next;
	}
	return L1;
} 

List* filterequalWins(List *L, int wins)
{
	List *L1 = CreateList();
	Element *E = L->First;
	while (E)
	{
		if (E->vic == wins)
		{
			Element *tmp = CreateElement();
			tmp->vic = E->vic;
			tmp->los = E->los;
			tmp->draw = E->draw;
			tmp->score = E->score;
			tmp->name = (char*)malloc((strlen(E->name) + 1) * sizeof(char));
			memcpy(tmp->name, E->name, (strlen(E->name) + 1));
			tmp->city = (char*)malloc((strlen(E->city) + 1) * sizeof(char));
			memcpy(tmp->city, E->city, (strlen(E->city) + 1));
			pushBack(L1, tmp);

		}
		E = E->next;
	}
	return L1;
}

List* filtergreaterWins(List *L, int wins)
{
	List *L1 = CreateList();
	Element *E = L->First;
	while (E)
	{
		if (E->vic > wins)
		{
			Element *tmp = CreateElement();
			tmp->vic = E->vic;
			tmp->los = E->los;
			tmp->draw = E->draw;
			tmp->score = E->score;
			tmp->name = (char*)malloc((strlen(E->name) + 1) * sizeof(char));
			memcpy(tmp->name, E->name, (strlen(E->name) + 1));
			tmp->city = (char*)malloc((strlen(E->city) + 1) * sizeof(char));
			memcpy(tmp->city, E->city, (strlen(E->city) + 1));
			pushBack(L1, tmp);

		}
		E = E->next;
	}
	return L1;
}

Element* searchName(List *L, const char *nam)
{
	Element *E = L->First;
	int b = 1;

	while(E && b)
	{
		char str1[100] = {};
		strcpy_s(str1, sizeof(str1), E->name);
		b = strcmp(str1, nam);
		if (b == 0)
			return E;
		E = E->next;
	}
	return E;
}

void SetupElement(Element *E)
{
	printf("Enter the name of the team\n");
	E->name = (char*)malloc(100 * sizeof(char));
	fill(E->name);
	printf("Enter the city name\n");
	E->city = (char*)malloc(100 * sizeof(char));
	fill(E->city);
	printf("Enter the number of wins, losses, draws and score\n");
	scanf_s("%d %d %d %d", &(E->vic), &(E->los), &(E->draw), &(E->score));
}

int main()
{
	List *L = load("xex.txt");


	Element *E1 = CreateElement();
	SetupElement(E1);
	pushBack(L, E1);

	print_list(L);

	Element *En = searchName(L, "Gork");
	print_elem(En);

	int b = save("out.txt", L);
	printf("\n%d\n", b);
	/*
	Element *E1 = CreateElement();
	Element *E2 = CreateElement();
	Element *E4 = CreateElement();
	Element *E5 = CreateElement();
	Element *E6 = CreateElement();

	init_elem(E1, "Gork", "BigTown", 1, 0, 1, 110);
	init_elem(E2, "Mork", "SmallCity", 2, 1, 2, 35);
	init_elem(E4, "Akappa", "Brezent", 6, 3, 0, 20);
	init_elem(E5, "Brodite", "Kholmgard", 5, 5, 27, 110);
	init_elem(E6, "PolarBears", "Nelson", 9, 2, 2, 10);

	pushBack(L, E1);
	pushBack(L, E2);
	pushBack(L, E4);
	pushBack(L, E5);
	pushBack(L, E6);

	Element *E3 = CreateElement();
	init_elem(E3, "VentoVeloce", "CasaInMontagna", 10, 0, 0, 1);

	insert(L, E3, 1);

	print_list(L);

	List *L1 = filterlessWins(L, 5);
	print_list(L1);
	List *L2 = filtergreaterWins(L, 5);
	print_list(L2);

	Element *En = searchName(L, "PolarBears");
	print_elem(En);*/
}
