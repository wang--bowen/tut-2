#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

enum CommandType { ADD_LINE = 0, DISPLAY_ALL = 1, DELETE_LINE = 2, CLEAR_ALL = 3, EXIT = 4, INVALID = 5, SORT = 6, SEARCH = 7};

const int ARGUMENT_NUMBER = 2;
const string MESSAGE_EMPTY = "%s is empty";
const string MESSAGE_ADDED = "added to %s: \"%s\"";
const string MESSAGE_DELETED = "deleted from %s: \"%s\"";
const string MESSAGE_CLEARED = "all info deleted from %s";
const string MESSAGE_INVALID_FORMAT = "invalid command format: %s";
const string MESSAGE_PROGRAM_TERMINATION = "Press any key to exit: ";
const string MESSAGE_WELCOME = "Welcome to TextBuddy. %s is ready for use";
const string ERROR_UNRECOGNISED_COMMAND_TYPE = "ERROR: Unrecognised command type.\n1. add <text>\n2. delete <line number>\n3. clear all\n4. display all\n";
const string MESSAGE_ERROR = "ERROR: textbuddy.exe <fileName>.txt";
const string MESSAGE_SORTED = " %s is sorted alphabetically";
const string MESSAGE_FOUND = " %s is found in %s";
char buffer[255];

void showErrorExit();
void getCommand(string fileName);
void showToUser(string text);
string executeCommand(string fileName, string userCommand);
string addLine(string fileName, string info);
string displayAll(string fileName);
string deleteLine(string fileName, string info);
string clearAll(string fileName);
CommandType determineCommandType(string userCommand);
string getFirstWord(string userCommand);
string removeFirstWord(string userCommand);
string sortLine(string fileName);
string searchWord(string fileName, string keyword);



int main(int argc, char* argv[]) {
	string fileName;

	if (argc != ARGUMENT_NUMBER) 
		showErrorExit();

	fileName = argv[1];

	sprintf_s(buffer, MESSAGE_WELCOME.c_str(), fileName.c_str());
	showToUser(buffer);
	getCommand(fileName);
	return 0;
}

void showErrorExit(){
	showToUser(MESSAGE_ERROR);
		showToUser(MESSAGE_PROGRAM_TERMINATION);

		getchar();
		exit(0);
}

//let user type in their input
void getCommand(string fileName) {                       

		while (true) {	
		string userCommand;
		cout << "command: ";
		getline(cin, userCommand);
		string feedback = executeCommand(fileName, userCommand);
		showToUser(feedback);
	}
}

//find out what command it is and execute correspondingly
string executeCommand(string fileName, string userCommand) { 
	CommandType command;
	string info;
	string keyword;

	command = determineCommandType(getFirstWord(userCommand));
	info = removeFirstWord(userCommand);
 

	switch (command) {
		case ADD_LINE:
			return addLine(fileName, info);

		case DISPLAY_ALL:
			return displayAll(fileName);

		case DELETE_LINE:
			return deleteLine(fileName, info);

		case CLEAR_ALL:
			return clearAll(fileName);

		case EXIT:
			exit(0);

		case INVALID:
			sprintf_s(buffer, MESSAGE_INVALID_FORMAT.c_str(), userCommand.c_str());
			return buffer;

		case SORT:
			return sortLine(fileName);

		case SEARCH:{
			keyword = removeFirstWord(userCommand);
			return searchWord(fileName,keyword);
		}
					
		default:
			showToUser(ERROR_UNRECOGNISED_COMMAND_TYPE);
			showToUser(MESSAGE_PROGRAM_TERMINATION);

			getchar();
			exit(EXIT_FAILURE);
	}

}

//add user input content into the file
string addLine(string fileName, string info) {             
	ofstream file;

	file.open(fileName, ios::app);
	file << info << endl;
	file.close();

	sprintf_s(buffer, MESSAGE_ADDED.c_str(), fileName.c_str(), info.c_str());

	return buffer;
}

//show all contents in the file
string displayAll(string fileName) {                    
	ifstream file;
	string line;
	int i;

	file.open(fileName);

	for (i = 1; getline(file, line); i++) {
		cout << i << ". " << line << endl;
	}

	if (i == 1) {
		sprintf_s(buffer, MESSAGE_EMPTY.c_str(), fileName.c_str());
		return buffer;
	}

	file.close();

	return "";
}

 //delete a specific line from the file
string deleteLine(string fileName, string info) {      
	ofstream newFile;
	ifstream oldFile;
	string tempLine;
	string deletedLine;

	oldFile.open(fileName);
	newFile.open("temp.txt");

	for (int i = 1; getline(oldFile, tempLine); i++) {
		if (i == stoi(info)) {
			deletedLine = tempLine;
			continue;
		}
		else {
			newFile << tempLine << endl;
		}
	}

	oldFile.close();
	newFile.close();

	remove(fileName.c_str());
	rename("temp.txt", fileName.c_str());

	sprintf_s(buffer, MESSAGE_DELETED.c_str(), fileName.c_str(), deletedLine.c_str());

	return buffer;
}

 //clear all contents in the file
string clearAll(string fileName) {                  
	ifstream file;

	file.open("temp.txt");
	file.close();
	remove(fileName.c_str());
	rename("temp.txt", fileName.c_str());

	sprintf_s(buffer, MESSAGE_CLEARED.c_str(), fileName.c_str());

	return buffer;
}

//determine what is the command that user typed in
CommandType determineCommandType(string command) {     
	transform(command.begin(), command.end(), command.begin(), ::tolower);

	if (command == "add"){
		return CommandType::ADD_LINE;
	}
	else if (command == "display") {
		return CommandType::DISPLAY_ALL;
	}
	else if (command == "delete") {
		return CommandType::DELETE_LINE;
	}
	else if (command == "clear") {
		return CommandType::CLEAR_ALL;
	}
	else if (command == "exit") {
		return CommandType::EXIT;
	}
	else if (command == "sort"){
		return CommandType::SORT;
	}
	else if (command == "search"){
		return CommandType::SEARCH;
	}
	else
		return CommandType::INVALID;
}

//extract the command word from user input
string getFirstWord(string command){                    
	return command.substr(0, command.find(' '));
}

//extract content from user input
string removeFirstWord(string command){                 
	return command.substr(command.find_first_of(" ")+1);
}


void showToUser(string text) {                          
	cout << text << endl;
}

