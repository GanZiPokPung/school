#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _DBTNode
{
	//char *word;
	char *desc;
	struct _DBTNode *left;
	struct _DBTNode *right;
	char word[0];
}DBTNode;

DBTNode *DBTNode_allocate(const char *word, const char *desc)
{
	int wlen = strlen(word);
	int dlen = strlen(desc);
	int size = sizeof(DBTNode) + wlen + 1 + dlen + 1; // '\0'를 위해 1을 더해준다.
	DBTNode *node = (DBTNode *)malloc(size);
	node->left = NULL;
	node->right = NULL;
	node->desc = node->word + wlen + 1; // 설명이 시작되는 곳으로 시작위치를 설정한다.
	strcpy(node->word, word);
	strcpy(node->desc, desc);

	return node;

	 
}

void DBTNode_free(DBTNode *node)
{
	free(node);
}

int DBTNode_getHeight(const DBTNode *node)
{
	return 0;
}

int DBTNode_countNode(const DBTNode *node)
{
	return 0;
}

int DBTNode_getBalanceFactor(const DBTNode *node)
{
	// 왼쪽 자식의 heigt와 오른쪽 자식의 height를 뺀 값 (0에 가까울수록 밸런스)
	return 0;
}

DBTNode *DBTNode_insert(DBTNode *node, DBTNode *newNode)
{
	int cmp = strcmp(node->word, newNode->word);
	if (cmp == 0)
	{
		return NULL;
	}
	if (cmp > 0)
	{
		if(!DBTNode_insert(node->left, newNode))
			return NULL;
		else
			node->left = newNode;
	}
	else
	{
		if (!DBTNode_insert(node->right, newNode))
			return NULL;
		else
			node->right = newNode;
	}
	return node;
}

void DBTNode_print(const DBTNode *node, int level)
{
	if (!node) return;
	// pre-order
	/* %별c를 하면 첫번째 인자로 두번째를 문자로 */
	/* level만큼의 빈칸을 확보하고 print한다 */
	DBTNode_print(node->left, level + 1);
	printf("%*c%s\n", level, '#', node->word);
	DBTNode_print(node->right, level + 1);
}

DBTNode *DBTNode_find(DBTNode *node, const char *word)
{
	if (!node) return NULL;
	int cmp = strcmp(node->word, word);
	if (cmp == 0)
		return node;

	if (cmp < 0)
		return DBTnode_find(node->left, word);
	else
		return DBTNode_find(node->right, word);
}

typedef struct _DTree
{
	DBTNode *root;
}DTree;

// return 1: true 0: false
int DTree_add(DTree *tree, const char *word, const char *desc)
{
	DBTNode *node = DBTNode_allocate(word, desc);
	if (!tree->root)
	{
		tree->root = node;
		return 1; // true
	}

	if (!DBTNode_insert(tree->root, node))
	{
		DBTNode_free(node);
		return 0; //false
	}

	return 1; // true
}

void trimRight(char *str)
{
    char *p = str + strlen(str) - 1;
    while (str <= p && isspace(*p)) {
        *p-- = '\0';
    }
}

DTree g_tree;

void readFile(const char *filename)
{
	/* 파일 오픈 */
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }

    int lines = 0;
    char buf[2048];
    char word[256];
	/* 읽을 line이 없을때까지 루프 */
    while (fgets(buf, sizeof (buf), fp)) {
        trimRight(buf);
        char *p = buf;
		/* 이 버퍼에 빈칸이 있다면 다음으로 넘어간다. */
        while (*p && isspace(0xFF & *p)) {
            p++;
        }
		/* 단어 하나를 읽어서 word에 저장한다. */
        if (1 != sscanf(p, "%s", word)) {
            continue;
        }

        int wl = strlen(word);
		/* 단어 설명의 시작점으로 */
        char *desc = p + wl;
        while (*desc && isspace(0xFF  &*desc)) {
            desc++;
        }

        if (!*desc) {
            continue;
        }

        lines++;
        printf("Read #%d. %s: %s\n", lines, word, desc);
		DTree_add(&g_tree, word, desc);
    }
    printf("Read %d line(s).\n", lines);
    fclose(fp);
}

void findWord(const char *word)
{
	DBTNode *node = DBTNode_find(g_tree.root, word);
	if (!node)
	{
		//printf("word [%s] /")
	}
    printf("findWord(%s) Not Implemented\n", word);
}

int main(void)
{
    while (1) {
        char buf[2048];
        if (!fgets(buf, sizeof (buf), stdin)) {
            return 0;
        }
        trimRight(buf);
        switch (buf[0]) {
        case 'r':
            readFile(buf + 2);
            break;
        case 'w':
            findWord(buf + 2);
            break;
		case 't':
			DBTNode_print(g_tree.root, 1);
			break;
        case 'q':
            return 0;
        default:
            printf("Unknown command %c\n", buf[0]);
            break;
        }
    }
    /* code */
    return 0;
}