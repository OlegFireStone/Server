#include "stdafx.h" 
#pragma comment(lib, "ws2_32.lib")
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <winsock2.h> 
#include <iostream> 
#include <fstream>
#include <string>
#include <experimental/filesystem>
using namespace std;
#pragma warning(disable: 4996) 

void send_file(SOCKET* sock, const string& file_name) {
	fstream file;
	file.open(file_name, ios_base::in | ios_base::binary);

	if (file.is_open()) {
		int file_size = experimental::filesystem::file_size(file_name) + 1;
		char* bytes = new char[file_size];
		file.read((char*)bytes, file_size);

		cout << "size: " << file_size << endl;
		cout << "name: " << file_name << endl;

		send(*sock, to_string(file_size).c_str(), 16, 0);
		send(*sock, file_name.c_str(), 32, 0);
		send(*sock, bytes, file_size, 0);
		delete[] bytes;
	}
	else
		cout << "Error file open" << endl;

	file.close();
}

int main(int argc, char* argv[]) {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1); 
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr; 
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); 

	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));

	listen(sListen, SOMAXCONN);


	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		cout << "Error #2\n";
	}
	else {
		cout << "Client Connected!\n";
		
		string path;
		cout << "PATH_FILE: ";
		cin >> path;
		send_file(&newConnection, path);
	}

	system("pause");
	return 0;
}