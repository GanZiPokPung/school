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
		cout << "   C++�� ������ AVLƮ��" << endl;
		cout << "-----------------------------" << endl;
		cout << "1. ���� ����" << endl;
		cout << "2. AVL Ʈ�� ���" << endl;
		cout << "3. ���� ��ȸ" << endl;
		cout << "4. ���� ��ȸ" << endl;
		cout << "5. ���� ��ȸ" << endl;
		cout << "6. ���α׷� ����" << endl;
		cout << "����? ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			cout << "������ ������ �Է��ϼ��� : ";
			cin >> item;
			AVL.root = AVL.insert(AVL.root, item);
	
		case 2:
			if (nullptr == AVL.root)
			{
				cout << "���� Ʈ���� ������ϴ�." << endl;
				continue;
			}
			cout << "\n[AVL Ʈ�� ���]" << endl;
			AVL.display(AVL.root, 1);
			break;

		case 3:
			cout << "\n[���� ��ȸ]" << endl;
			AVL.inorder(AVL.root);
			cout << endl;
			break;

		case 4:
			cout << "\n[���� ��ȸ]" << endl;
			AVL.preorder(AVL.root);
			cout << endl;
			break;

		case 5:
			cout << "\n[���� ��ȸ]" << endl;
			AVL.postorder(AVL.root);
			cout << endl;
			break;

		case 6:
			exit(1);
			break;

		default:
			cout << "�߸��� �Է��Դϴ�." << endl;
		}	
	}*/

#pragma endregion

#pragma region HeapTree

#pragma endregion

	return 0;
}