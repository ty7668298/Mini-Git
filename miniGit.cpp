#include<iostream>
#include<filesystem>
#include<fstream>
#include<sstream>
#include"miniGit.h"
using namespace std;
namespace fs = filesystem;

miniGit::miniGit() {
	head = new doublyNode;
	head->commitNumber = 0;
	head->head = nullptr;
	head->next = nullptr;
	head->previous = nullptr;
	currentNode = head;
	cout << "Initialize an empty repository in the current directory ? (Y/N): ";
	char tmp; // choice
	while (true) {
		cin >> tmp;
		if (tmp == 'Y') {
			fs::remove_all(".minigit"); // removes a directory and its contents
			fs::create_directory(".minigit"); // create a new directory
			cout << "Done!" << endl;
			break;
		}
		else if (tmp == 'N') {
			break;
		}
		else {
			cout << "Your input is incorrect. Please re-enter: ";
		}
	}
}

miniGit::~miniGit() {
	// destructor function
	while (head) {
		singlyNode* p = head->head;
		while (p) {
			singlyNode* tmp_s = p;
			p = p->next;
			delete tmp_s;
		}
		doublyNode* tmp_d = head;
		head = head->next;
		delete tmp_d;
	}
}

int miniGit::Menu() {
	cout << "----------------------" << endl;
	cout << "1. Adding files." << endl;
	cout << "2. Removing files." << endl;
	cout << "3. Committing changes." << endl;
	cout << "4. Check out." << endl;
	cout << "5. Quit." << endl;
	cout << "----------------------" << endl;
	string tmp; // choice
	cout << "Your choice (Press 1-5): ";
	while (true) {
		cin >> tmp;
		if (tmp.length() != 1 || tmp[0] < '1' || tmp[0] > '5') {
			cout << "Your input is incorrect. Please re-enter: ";
		}
		else {
			break;
		}
	}
	switch (tmp[0] - '0')
	{
	case 1:
		Adding_files();
		break;
	case 2:
		Removing_files();
		break;
	case 3:
		Committing_changes();
		break;
	case 4:
		Check_out();
		break;
	case 5:
		return 0;
	default:
		break;
	}
	return 1;
}

void miniGit::Adding_files() {
	//1. Prompt user to enter a file name.
	string file_name;
	cout << "Please enter a file name: ";
	fstream file;
	while (true) {
		cin >> file_name;
		file.open(file_name.c_str());
		//2. Check whether the file with the given name exists in the current directory.
		if (!file) {
			cout << "Please enter a valid file name: ";
		}
		else {
			file.close();
			break;
		}
	}
	//3. The SLL is checked to see whether the file has already been added.
	singlyNode* p = currentNode->head;
	int flag = 0; // Flag whether the existing file was found
	while (p != nullptr) {
		if (file_name == p->fileName) {
			flag = 1;
			break;
		}
		p = p->next;
	}
	if (flag) {
		cout << "A file by the same name cannot be added twice." << endl;
		return;
	}

	//4. A new SLL node gets added containing the name of the input file, name of the repository file, as well as a pointer to the next node.
	singlyNode* newNode = new singlyNode;
	newNode->fileName = file_name;
	newNode->fileVersion = get_fileVersion(file_name, 0);
	newNode->next = nullptr;
	if (currentNode->head == nullptr) {
		currentNode->head = newNode;
	}
	else {
		p = currentNode->head;
		while (p->next) {
			p = p->next;
		}
		p->next = newNode;
	}
	cout << "Done!" << endl;
}

void miniGit::Removing_files() {
	//1. Prompt user to enter a file name.
	string file_name;
	cout << "Please enter a file name: ";
	cin >> file_name;
	singlyNode* p = currentNode->head;

	//2. Check the SLL for whether the file exists in the current version of the repository.
	int flag = 0; // Flag whether the existing file was found
	while (p != nullptr) {
		if (file_name == p->fileName) {
			flag = 1;
			break;
		}
		p = p->next;
	}
	if (!flag) {
		// There is no such file
		cout << "The file does not exist in the current version of the repository." << endl;
		return;
	}

	//3. If found, delete the SLL node.
	if (p == currentNode->head) {
		currentNode->head = p->next;
		delete p;
	}
	else {
		// Find the previous node of p
		singlyNode* tmp = currentNode->head;
		while (tmp->next != p) {
			tmp = tmp->next;
		}
		tmp->next = p->next;
		delete p;
	}
	cout << "Done!" << endl;
}

void miniGit::Committing_changes() {
	singlyNode* p = currentNode->head;
	while (p) {
		//1. Check whether the corresponding fileVersion ﬁle exists in .minigit directory.
		ifstream f1("./.minigit/" + p->fileVersion);
		if (!f1) {
			// The file is not in the .minigit folder
			ofstream f2("./.minigit/" + p->fileVersion);
			ifstream f3(p->fileName);
			ostringstream sin;
			//Input the characters in the file stream into the string stream
			sin << f3.rdbuf();
			f2 << sin.str();
			f2.close();
			f3.close();
		}
		else {
			ifstream f2(p->fileName);
			string tmp_str1 = "", tmp_str2 = "";
			ostringstream sin1, sin2;
			//Input the characters in the file stream into the string stream
			sin1 << f1.rdbuf();
			tmp_str1 = sin1.str();
			sin2 << f2.rdbuf();
			tmp_str2 = sin2.str();
			if (tmp_str1 != tmp_str2) {
				//2. copy the ﬁle from the current directory to the .minigit directory, 
				//and give it a name with the incremented version number.
				string file_name = get_fileVersion(p->fileName, getVersion(p->fileVersion) + 1);
				ofstream f3("./.minigit/" + file_name);
				f3 << tmp_str2;
				f3.close();

				//3. Also, update the SLL node member fileVersion to the incremented name.
				p->fileVersion = file_name;
			}
			f2.close();
		}
		f1.close();
		p = p->next;
	}
	
	//4. An exact (deep) copy of the SLL from the previous node shall be copied into the new DLL node.
	doublyNode* newDnode = new doublyNode;
	newDnode->commitNumber = currentNode->commitNumber + 1;
	newDnode->previous = currentNode;
	newDnode->next = nullptr;
	p = currentNode->head;
	singlyNode* pre_node = nullptr;
	if (!p) {
		newDnode->head = nullptr;
	}
	while (p) {
		singlyNode* newSnode = new singlyNode;
		newSnode->fileName = p->fileName;
		newSnode->fileVersion = p->fileVersion;
		newSnode->next = nullptr;
		if (p == currentNode->head) {
			newDnode->head = newSnode;
		}
		else {
			pre_node->next = newSnode;
		}
		pre_node = newSnode;
		p = p->next;
	}
	currentNode->next = newDnode;
	currentNode = newDnode;
	cout << "Done!" << endl;
}

void miniGit::Check_out() {
	//1. If the user chooses to checkout a version, they should be prompted to enter a commit number.
	cout << "Please enter a commit number: ";
	int Commit_number;
	cin >> Commit_number;
	doublyNode* p = head;
	int flag = 0; // Flag whether the commit was found
	while (p) {
		if (p->commitNumber == Commit_number) {
			flag = 1;
			break;
		}
		p = p->next;
		
	}
	if (!flag) {
		cout << "Not a valid commit number." << endl;
		return;
	}

	//3. It is a good idea to issue a warning to the user that they will loose their local changes.
	cout << "Warning: You may loose your local changes." << endl;
	cout << "Do you want to commit now ? (Y/N): ";
	char user_choice;
	while (true) {
		cin >> user_choice;
		if (user_choice == 'Y') {
			Committing_changes();
			break;
		}
		else if (user_choice == 'N') {
			break;
		}
		else {
			cout << "Your input is incorrect. Please re-enter: ";
		}
	}

	//2. the ﬁles in the current directory should be overwritten by the the corresponding ﬁles from the.minigit directory. 
	singlyNode* tmp = p->head;
	while (tmp) {
		ifstream f1("./.minigit/" + tmp->fileVersion);
		ofstream f2(tmp->fileName);
		if (f1 && f2) {
			ostringstream sin;
			//Input the characters in the file stream into the string stream
			sin << f1.rdbuf();
			f2 << sin.str();
		}
		f1.close();
		f2.close();
		tmp = tmp->next;
	}

	cout << "Done!" << endl;
}

string miniGit::get_fileVersion(string filename, int version) {
	int i = 0;
	for (i = 0; filename[i] != '.'; i++) {}
	string version_tmp = "00";
	version_tmp[0] += version / 10;
	version_tmp[1] += version % 10;
	string fileVersion = filename.substr(0, i) + "_" + version_tmp + filename.substr(i);
	return fileVersion;
}

int miniGit::getVersion(string file_name) {
	int i;
	for (i = 0; file_name[i] != '_'; i++) {}
	string version_str = file_name.substr(i + 1, 2);
	int version = 10 * (version_str[0] - '0') + version_str[1] - '0';
	return version;
}
