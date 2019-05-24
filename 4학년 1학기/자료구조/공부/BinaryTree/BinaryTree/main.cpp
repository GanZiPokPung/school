#include "pch.h"

#include "BinarySearchTree.h"
#include "Tree_AVL.h"

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
	int choice, item;

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
			root = AVL.insert(root, item);
		case 2:
			if (nullptr == root)
			{
				cout << "���� Ʈ���� ������ϴ�." << endl;
				continue;
			}

			cout << "[AVL Ʈ�l ���]" << endl;
			AVL.display(root, 1);
			break;
		case 3:
			cout << "[���� ��ȸ]" << endl;
			AVL.inorder(root);
			cout << endl;
			break;
		case 4:
			cout << "[���� ��ȸ]" << endl;
			AVL.inorder(root);
			cout << endl;
			break;
		case 5:
			cout << "[���� ��ȸ]" << endl;
			AVL.inorder(root);
			cout << endl;
			break;
		case 6:
			exit(1);
			break;
		default:
			cout << "�߸��� �Է��Դϴ�." << endl;
		}	
	}

#pragma endregion

	return 0;
}