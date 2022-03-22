#pragma once
#include<string>
using namespace std;
class miniGit {
public:
	miniGit();
	~miniGit();
	int Menu();
private:
	string get_fileVersion(string filename, int version);
	int getVersion(string file_name);
	void Adding_files();
	void Removing_files();
	void Committing_changes();
	void Check_out();
	struct singlyNode {
		string fileName; // Name of local file
		string fileVersion; // Name of file in .minigit folder
		singlyNode* next;
	};
	struct doublyNode {
		int commitNumber;
		singlyNode* head;
		doublyNode* previous;
		doublyNode* next;
	};
	doublyNode* head;
	doublyNode* currentNode;
};