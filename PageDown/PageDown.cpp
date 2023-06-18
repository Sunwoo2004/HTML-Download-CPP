#include <iostream>
#include <Windows.h>
#include <WinInet.h>
#include <fstream>
#pragma comment(lib,"Wininet.lib")
#include <gumbo.h>
#ifdef _DEBUG
#pragma comment(lib,"gumboD.lib")
#else
#pragma comment(lib,"gumbo.lib")
#endif

using namespace std;

void OnPageDownload(const char* szUrl, const char* szFileName);
void extractElement(GumboNode* node, const char* tagName);
void OnParsingPage(const char * szFileName);

int main()
{
    OnPageDownload("https://www.coupang.com/", "test.txt");
    cout << "Download OK" << endl;

    Sleep(100);

    OnParsingPage("test.txt");

    system("PAUSE");
    return 0;
}

void OnParsingPage(const char* szFileName)
{
    std::ifstream file(szFileName);
    if (!file)
    {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    std::string htmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    GumboOutput* output = gumbo_parse(htmlContent.c_str());
    extractElement(output->root, "HTML");

    gumbo_destroy_output(&kGumboDefaultOptions, output);
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

void extractElement(GumboNode* node, const char* tagName)
{
    if (node->type == GUMBO_NODE_ELEMENT)
    {
        GumboElement* element = &node->v.element;
        if (strcmp((const char *)element->tag, tagName) == 0)
        {
            std::cout << "Tag : " << element->original_tag.data << std::endl;
        }
    }
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i)
    {
        extractElement(static_cast<GumboNode*>(children->data[i]), tagName);
    }
}