#include "BinarySearchTree.h"
#include "Tree_AVL.h"

#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

int main()
{
#pragma region BinarySearchTree
	/*BinarySearchTree* mainTree = new BinarySearchTree();

	mainTree->insert(60);

	mainTree->insert(41);
	mainTree->insert(74);

	mainTree->insert(16);
	mainTree->insert(53);
	mainTree->insert(65);

	mainTree->insert(25);
	mainTree->insert(46);
	mainTree->insert(55);
	mainTree->insert(63);
	mainTree->insert(70);

	mainTree->insert(42);
	mainTree->insert(62);
	mainTree->insert(64);

	mainTree->remove(60);

	mainTree->preorderPrint();
	cout << endl << endl;

	mainTree->inorderPrint();
	cout << endl << endl;

	mainTree->postorderPrint();
	cout << endl << endl;*/
#pragma endregion

#pragma region AVLTree
	/*int choice, item;

	CTreeAVL AVL;

	while (1)
	{
		cout << "\n----------------------------" << endl;
		cout << "   C++로 구현한 AVL트리" << endl;
		cout << "-----------------------------" << endl;
		cout << "1. 원소 삽입" << endl;
		cout << "2. AVL 트리 출력" << endl;
		cout << "3. 중위 순회" << endl;
		cout << "4. 전위 순회" << endl;
		cout << "5. 후위 순회" << endl;
		cout << "6. 프로그램 종료" << endl;
		cout << "선택? ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			cout << "삽입할 정수를 입력하세요 : ";
			cin >> item;
			AVL.root = AVL.insert(AVL.root, item);
	
		case 2:
			if (nullptr == AVL.root)
			{
				cout << "현재 트리가 비었습니다." << endl;
				continue;
			}
			cout << "\n[AVL 트리 출력]" << endl;
			AVL.display(AVL.root, 1);
			break;

		case 3:
			cout << "\n[중위 순회]" << endl;
			AVL.inorder(AVL.root);
			cout << endl;
			break;

		case 4:
			cout << "\n[전위 순회]" << endl;
			AVL.preorder(AVL.root);
			cout << endl;
			break;

		case 5:
			cout << "\n[후위 순회]" << endl;
			AVL.postorder(AVL.root);
			cout << endl;
			break;

		case 6:
			exit(1);
			break;

		default:
			cout << "잘못된 입력입니다." << endl;
		}	
	}*/

#pragma endregion

#pragma region HeapTree

#pragma endregion

	return 0;
}