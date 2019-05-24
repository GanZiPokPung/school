#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_BUFFER 2048
#define WHITESPACES " \n\r\t"
#define VECTOR_INIT_SLOT_SIZE 8


typedef struct _Pitcher {
    char name[32];
    char team[4];
    int win, lose;
    float era;
    int games, games_started;
    int saves, svo;
    int innings, outs;
    int hits, runs, earned_runs;
    int homeruns, baseonballs;
    int strikeouts;
    float hits_per_atbats, whip;
} Pitcher;


typedef Pitcher *VElement;

typedef struct _Vector {
    VElement *elements;
    int size;
    int maxSize;
} Vector;


void Pitcher_print(const Pitcher *p)
{
    printf("[%-3s]%s W=%d L=%d ERA=%.2f G=%d GS=%d SV=%d SVO=%d IP=%d.%d H=%d R=%d ER=%d HR=%d BB=%d SO=%d AVG=%.2f WHIP=%.2f\n",
        p->team, p->name, p->win, p->lose, p->era,
        p->games, p->games_started,
        p->saves, p->svo,
        p->innings, p->outs,
        p->hits, p->runs, p->earned_runs,
        p->homeruns, p->baseonballs,
        p->strikeouts, p->hits_per_atbats, 
        p->whip);
}

Pitcher *Pitcher_alloc(const char *line)
{
    Pitcher p;
    int rank;
    char lname[20], fname[4];
    int n = sscanf(line, "%d%s%s%s%d%d%f%d%d%d%d%d.%d%d%d%d%d%d%d%f%f",
        &rank, lname, fname, p.team, &p.win, &p.lose, &p.era,
        &p.games, &p.games_started,
        &p.saves, &p.svo,
        &p.innings, &p.outs,
        &p.hits, &p.runs, &p.earned_runs,
        &p.homeruns, &p.baseonballs,
        &p.strikeouts, &p.hits_per_atbats, 
        &p.whip);
    // printf("read: %d rank=%d ln=%s fn=%s\n", n, rank, lname, fname);
    // printf(" hr=%d bb=%d so=%d avg=%f whip=%f\n", 
    //     p.homeruns, p.baseonballs, p.strikeouts, p.hits_per_atbats, p.whip);

    if (n != 21) return NULL;
    Pitcher *pitcher = (Pitcher *)malloc(sizeof (Pitcher));
    *pitcher = p;
    sprintf(pitcher->name, "%s %s", lname, fname);
    return pitcher;
}

void Pitcher_destroy(Pitcher *p)
{
    free(p);
}

void Vector_init(Vector *v)
{
    v->elements = NULL;
    v->size = 0;
    v->maxSize = 0;
}

int Vector_add(Vector *v, VElement e)
{
    if (v->size == v->maxSize) {
        if (v->maxSize == 0) {
            v->maxSize = VECTOR_INIT_SLOT_SIZE;
            v->elements = (VElement *)malloc(
                v->maxSize * sizeof (VElement));
        } else {
            v->maxSize = 2 * v->size;
            v->elements = (VElement *)realloc(
                v->elements, v->maxSize * sizeof (VElement));
        }
        printf("New vector capacity = %d\n", v->maxSize);
    }
    v->elements[v->size] = e;
    return v->size++;
}

void Vector_destroy(Vector *v)
{
    printf("Destroying Pitcher Vector of size %d\n", v->size);
    if (v->elements) {
        for (int i = 0; i < v->size; i++) {
            Pitcher_destroy(v->elements[i]);
        }
        free(v->elements);
    }
    Vector_init(v);
}

void Vector_print(Vector *v)
{
	for (int i = 0; i < v->size; ++i) {
		Pitcher_print(v->elements[i]);
	}
}

Vector pitchers;

void loadDataFromFile(const char *file)
{
    FILE *fp = fopen(file, "r");
    if (!fp) {
        printf("File not open: %s\n", file);
        return;
    }
    char buffer[MAX_LINE_BUFFER];
    while (fgets(buffer, MAX_LINE_BUFFER, fp)) {
        Pitcher *p = Pitcher_alloc(buffer);
        if (p) {
            Pitcher_print(p);
            Vector_add(&pitchers, p);
        }
    }
    fclose(fp);

    printf("Now number of pitchers = %d\n", pitchers.size);
}

#define DEFINE_Pitcher_sort_by(member, sign)\
int Pitcher_sort_by_##member(const void *a, const void *b)\
{\
	Pitcher *p1 = *(Pitcher**)a;\
	Pitcher *p2 = *(Pitcher**)b;\
	if(p1->member == p2->member) return 0;\
	return (p1->member > p2->member) ? (sign) : -(sign);\
}

#define DEFINE_Pitcher_sort_by_cmp(member, sign)\
int Pitcher_sort_by_##member(const void *a, const void *b)\
{\
	Pitcher *p1 = *(Pitcher**)a;\
	Pitcher *p2 = *(Pitcher**)b;\
	return (sign) * strcmp(p1->member, p2->member);\
}

// era를 만들고 오름차순으로
DEFINE_Pitcher_sort_by(whip, 1)
DEFINE_Pitcher_sort_by(era, 1)
DEFINE_Pitcher_sort_by(win, -1)
DEFINE_Pitcher_sort_by(lose, 1)
DEFINE_Pitcher_sort_by(strikeouts, -1)

DEFINE_Pitcher_sort_by(outs, 1)

DEFINE_Pitcher_sort_by_cmp(name, 1)
DEFINE_Pitcher_sort_by_cmp(team, 1)

#define DEFINE_sort_function(member) \
void sortBy_##member () \
{ \
	qsort(pitchers.elements, pitchers.size, \
	sizeof(VElement), Pitcher_sort_by_##member); \
	Vector_print(&pitchers); \
}

DEFINE_sort_function(era)
DEFINE_sort_function(whip)
DEFINE_sort_function(name)
DEFINE_sort_function(team)

#define DEFINE_case(member)\
if(strcmp(cmd, #member) == 0) \
{\
sortBy_##member(); \
continue; \
}

//int Pitcher_sort_by_whip(const void *a, const void *b)
//{
//	Pitcher *p1 = *(Pitcher **)a;
//	Pitcher *p2 = *(Pitcher **)b;
//	if (p1->whip == p2->whip) return 0;
//	return (p1->whip > p2->whip) ? 1 : -1;
//}

int Pitcher_sort_by_whip_desc(const void *a, const void *b)
{
	return -Pitcher_sort_by_whip(a, b);
}

//void sortByWhip(void)
//{
//	qsort(pitchers.elements, pitchers.size,
//		sizeof(VElement), Pitcher_sort_by_whip);
//
//	Vector_print(&pitchers);
//}

void sortByWhipDesc(void)
{
	qsort(pitchers.elements, pitchers.size,
		sizeof(VElement), Pitcher_sort_by_whip_desc);

	Vector_print(&pitchers);
}

//int Pitcher_sort_by_era(const void *a, const void *b)
//{
//	Pitcher *p1 = *(Pitcher **)a;
//	Pitcher *p2 = *(Pitcher **)b;
//	if (p1->era == p2->era) return 0;
//	return (p1->era > p2->era) ? 1 : -1;
//}
//
//int Pitcher_sort_by_name(const void *a, const void *b)
//{
//	Pitcher *p1 = *(Pitcher **)a;
//	Pitcher *p2 = *(Pitcher **)b;
//
//	int result = strcmp(p1->name, p2->name);
//
//	if (0 == result) return 0;
//
//	return (0 < result) ? 1 : -1;
//}
//
//int Pitcher_sort_by_team(const void *a, const void *b)
//{
//	Pitcher *p1 = *(Pitcher **)a;
//	Pitcher *p2 = *(Pitcher **)b;
//
//	int result = strcmp(p1->team, p2->team);
//
//	if (0 == result) return 0;
//
//	return (0 < result) ? 1 : -1;
//}
//
//int Pitcher_sort_by_outs(const void *a, const void *b)
//{
//	Pitcher *p1 = *(Pitcher **)a;
//	Pitcher *p2 = *(Pitcher **)b;
//	if (p1->outs == p2->outs) return 0;
//	return (p1->outs > p2->outs) ? 1 : -1;
//}

int Pitcher_sort_by_innings(const void *a, const void *b)
{
	Pitcher *p1 = *(Pitcher **)a;
	Pitcher *p2 = *(Pitcher **)b;
	// 이닝수가 같으면 아웃으로 검사
	if (p1->innings == p2->innings) return Pitcher_sort_by_outs(a, b);
	return (p1->innings > p2->innings) ? 1 : -1;
}

//void sortByEra(void)
//{
//	// 평균 자책점
//	qsort(pitchers.elements, pitchers.size,
//		sizeof(VElement), Pitcher_sort_by_era);
//
//	Vector_print(&pitchers);
//}
//
//void sortByName(void)
//{
//	// 이름 순서
//	// strcmp
//	qsort(pitchers.elements, pitchers.size,
//		sizeof(VElement), Pitcher_sort_by_name);
//
//	Vector_print(&pitchers);
//}
//
//void sortByTeam(void)
//{
//	// 팀 순서
//	qsort(pitchers.elements, pitchers.size,
//		sizeof(VElement), Pitcher_sort_by_team);
//
//	Vector_print(&pitchers);
//}

void sortByInnings(void)
{
	// 이닝 수가 많은 사람 순서대로
	//  innings, outs

	// innings수가 같으면 그때 outs를 검사
	qsort(pitchers.elements, pitchers.size,
		sizeof(VElement), Pitcher_sort_by_innings);

	Vector_print(&pitchers);
}

#define COUNT 50

char *ptr_names[COUNT];
char arr_names[COUNT][32];

void loadNames()
{
	FILE *fp = fopen("mlb_pitchers.txt", "r");
	if (!fp) {
		printf("File not open: mlb_pitchers.txt");
		return;
	}
	int index = 0;
	char buffer[MAX_LINE_BUFFER];
	while (fgets(buffer, MAX_LINE_BUFFER, fp)) {
		int rank;
		char name[32];
		int n = sscanf(buffer, "%d%s", &rank, name);
		if (n < 2) {

			continue;
		}
		strcpy(arr_names[index], name);
		int len = strlen(name);
		ptr_names[index] = (char*)malloc(len + 1);
		strcpy(ptr_names[index], name);
		index++;
		if (index >= COUNT) {
			break;
		}
	}
	fclose(fp);
}

int compare_ptr_name(const void **a, const void **b)
{
	char* p1 = *(char**)a;
	char* p2 = *(char**)b;

	int result = strcmp(p1, p2);

	if (0 == result) return 0;

	return (0 < result) ? 1 : -1;
}

int compare_arr_name(const void *a, const void *b)
{
	char* p1 = (char*)a;
	char* p2 = (char*)b;

	int result = strcmp(p1, p2);

	if (0 == result) return 0;

	return (0 < result) ? 1 : -1;
}

void sortNames(void)
{
	// ptr 사용한 배열 sort
	qsort(ptr_names, COUNT, sizeof(char*), compare_ptr_name);
	for (int i = 0; i < 50; ++i) {
		printf("%s ", ptr_names[i]);
	}
	printf("\n\n");
	
	// arr 사용한 배열 sort
	qsort(arr_names, COUNT, sizeof(char) * 32, compare_arr_name);
	for (int i = 0; i < 50; ++i) {
		printf("%s ", arr_names[i]);
	}
	printf("\n");
}

int main(void)
{
    Vector_init(&pitchers);

    while (1) {
        printf(">>> ");
        char buffer[MAX_LINE_BUFFER];
        if (!fgets(buffer, MAX_LINE_BUFFER, stdin)) {
            break;
        }
        char *cmd = strtok(buffer, WHITESPACES);
        if (!cmd || !*cmd) {
            continue;
        }
		if (strcmp(cmd, "lm") == 0) {
			loadNames();
			continue;
		}
		if (strcmp(cmd, "sn") == 0) {
			sortNames();
			continue;
		}
        if (strcmp(cmd, "load") == 0) {
            char *file = strtok(NULL, WHITESPACES);
            printf("filename: %s\n", file);
            loadDataFromFile(file);
            continue;
        }

		DEFINE_case(era);
		DEFINE_case(whip);
		DEFINE_case(name);
		DEFINE_case(team);
	
		if (strcmp(cmd, "whipd") == 0) {
			sortByWhipDesc();
			continue;
		}
		if (strcmp(cmd, "innings") == 0) {
			sortByInnings();
			continue;
		}
        if (strcmp(cmd, "quit") == 0) {
            break;
        }
        printf("Unknown command: %s\n", cmd);
    }
    return 0;
}