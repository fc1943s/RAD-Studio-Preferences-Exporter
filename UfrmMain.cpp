#include <fmx.h>
#pragma hdrstop

#include "UfrmMain.h"
#include "Winapi.ShellAPI.hpp"
#include "System.Zip.hpp"
#include "Registry.hpp"

#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMain* frmMain;

__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{ }

enum class Directories
{
	SYSTEM32, WINDOWS, TEMP, WINLETTER
};

UnicodeString getDirectory(Directories directory)
{
	wchar_t result[256];
	switch(directory)
	{
		case Directories::SYSTEM32:
		{
			GetSystemDirectoryW(result, 256);
			break;
		}
		case Directories::WINDOWS:
		{
			GetWindowsDirectoryW(result, 256);
			break;
		}
		case Directories::TEMP:
		{
			GetTempPathW(256, result);
			break;
		}
		case Directories::WINLETTER:
		{
			GetWindowsDirectoryW(result, 256);
			result[1] = L'\0';
			break;
		}
	}
	return UnicodeString(result);

}

UnicodeString getTempFile()
{ return getDirectory(Directories::TEMP) + "temp" + FormatDateTime("hhnnsszzz", Now()); }

void __fastcall TfrmMain::btnExportClick(TObject* Sender)
{
	if(svdExport->Execute())
	{
		if(FileExists(svdExport->FileName))
		{
			DeleteFileW(svdExport->FileName);
		}
		TZipFile* zip = new TZipFile;
		zip->Open(svdExport->FileName, zmWrite);

		// Files
		TSearchRec sr;
		UnicodeString dataPath = GetEnvironmentVariableW("AppData") + "\\Embarcadero\\BDS\\9.0\\";
		if(FindFirst(dataPath + "*.*", faAnyFile, sr) == 0)
		{
			do
			{
				if(ExtractFileExt(sr.Name) == ".config" || ExtractFileExt(sr.Name) == ".dst")
				{
					zip->Add(dataPath + sr.Name, sr.Name, zcStored);
				}
			}
			while(FindNext(sr) == 0);
			FindClose(sr);
		}

		// Registry
		UnicodeString tempFile = getTempFile();

		SHELLEXECUTEINFOW info;
		info.cbSize       = sizeof(SHELLEXECUTEINFOW);
		info.lpFile       = L"regedit.exe";
		info.lpParameters = UnicodeString("/e \"" + tempFile + "\" \"HKEY_CURRENT_USER\\Software\\Embarcadero\"").c_str();
		info.nShow        = 0;
		info.lpVerb       = L"runas";
		info.hwnd         = NULL;
		info.fMask        = SEE_MASK_NOCLOSEPROCESS;
		info.lpDirectory  = NULL;
		int exe           = ShellExecuteExW(&info);
		if(exe)
		{
			WaitForSingleObject(info.hProcess, INFINITE);
			CloseHandle(info.hProcess);

			TStringList* stl = new TStringList;
			stl->LoadFromFile(tempFile);
			for(int i = stl->Count - 1; i >= 0; i--)
			{
				if(stl->Strings[i].Pos((UnicodeString)"\"" + "\\\\") > 0 || stl->Strings[i].Pos("C:\\") > 0 || stl->Strings[i].Pos("c:\\") > 0 || stl->Strings[i].Pos("D:\\") > 0 || stl->Strings[i].Pos("d:\\") > 0 || stl->Strings[i].Pos("\"RegOwner\"=\"") > 0)
				{
					stl->Delete(i);
				}
			}
			stl->SaveToFile(tempFile);
			delete stl;
		}

		zip->Add(tempFile, "registry.reg", zcDeflate);

		zip->Close();
		delete zip;

		DeleteFileW(tempFile);

		ShowMessage("File exported successfully.");
	}
}

void __fastcall TfrmMain::ImportClick(TObject* Sender)
{

	if(opdImport->Execute())
	{
		TZipFile* zip = new TZipFile;
		zip->Open(opdImport->FileName, zmRead);

		UnicodeString tempDir = getTempFile() + "\\";

		zip->ExtractAll(tempDir);

		// Files
		TSearchRec sr;
		UnicodeString dataPath = GetEnvironmentVariableW("AppData") + "\\Embarcadero\\BDS\\9.0\\";
		if(FindFirst(tempDir + "*.*", faAnyFile, sr) == 0)
		{
			do
			{
				if(ExtractFileExt(sr.Name) == ".config" || ExtractFileExt(sr.Name) == ".dst")
				{
					DeleteFileW(dataPath + sr.Name);
					MoveFileW(UnicodeString(tempDir + sr.Name).c_str(), UnicodeString(dataPath + sr.Name).c_str());
				}
				if(ExtractFileExt(sr.Name) == ".reg")
				{
					SHELLEXECUTEINFOW info;
					info.cbSize       = sizeof(SHELLEXECUTEINFOW);
					info.lpFile       = L"regedit.exe";
					info.lpParameters = UnicodeString("/s \"" + tempDir + sr.Name + "\"").c_str();
					info.nShow        = 0;
					info.lpVerb       = L"runas";
					info.hwnd         = NULL;
					info.fMask        = SEE_MASK_NOCLOSEPROCESS;
					info.lpDirectory  = NULL;
					int exe           = ShellExecuteExW(&info);
					if(exe)
					{
						WaitForSingleObject(info.hProcess, INFINITE);
						CloseHandle(info.hProcess);
					}
					DeleteFileW(tempDir + sr.Name);
				}
			}
			while(FindNext(sr) == 0);
			FindClose(sr);
		}
		// Registry


		zip->Close();
		delete zip;
		RemoveDir(tempDir);

		ShowMessage("File imported successfully.");
	}
}


void __fastcall TfrmMain::FormCreate(TObject* Sender)
{
	typedef bool(*_IsUserAnAdmin)(VOID);
	_IsUserAnAdmin IsUserAnAdmin;
	HMODULE hModule = NULL;
	hModule         = LoadLibraryW(L"shell32.dll");

	IsUserAnAdmin = (_IsUserAnAdmin)GetProcAddress(hModule, "IsUserAnAdmin");

	if(!IsUserAnAdmin())
	{
		SHELLEXECUTEINFOW info;
		info.cbSize       = sizeof(SHELLEXECUTEINFOW);
		info.lpFile       = ParamStr(0).c_str();
		info.lpParameters = NULL;
		info.nShow        = 0;
		info.lpVerb       = L"runas";
		info.hwnd         = NULL;
		info.fMask        = SEE_MASK_NOCLOSEPROCESS;
		info.lpDirectory  = NULL;
		ShellExecuteExW(& info);
		Application->Terminate();
	}
	FreeLibrary(hModule);
}
