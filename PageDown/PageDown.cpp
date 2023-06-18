#include <iostream>
#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib,"Wininet.lib")

using namespace std;

void OnPageDownload(const char* szUrl, const char* szFileName);


int main()
{
    OnPageDownload("https://site.com", "test.txt");
    cout << "Download OK" << endl;

    system("PAUSE");
    return 0;
}

void OnPageDownload(const char * szUrl, const char * szFileName)
{
    HINTERNET hSession, hURL;
    char* Buffer = new char[1024];
    DWORD BufferLen, BytesWritten;
    HANDLE FileHandle;

    hSession = InternetOpenA(NULL, 0, NULL, NULL, 0);
    hURL = InternetOpenUrlA(hSession, szUrl, NULL, 0, 0, 0);

    FileHandle = CreateFileA(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
    BytesWritten = 0;
    do
    {
        InternetReadFile(hURL, Buffer, 1024, &BufferLen);
        WriteFile(FileHandle, Buffer, BufferLen, &BytesWritten, NULL);
    } while (BufferLen != 0);
    CloseHandle(FileHandle);

    InternetCloseHandle(hURL);
    InternetCloseHandle(hSession);
}