#include <fmx.h>
#pragma hdrstop

#include "UfrmMain.h"
#include "Winapi.ShellAPI.hpp"
#include "System.Zip.hpp"
#include "Registry.hpp"
#include "process.h"

#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMain* frmMain;


__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{ }

UnicodeString expandPath(UnicodeString path)
{
	wchar_t result[MAX_PATH];
	ExpandEnvironmentStringsW(path.c_str(), result, MAX_PATH);
	return (UnicodeString)result;
}

enum class Directories
{
	SYSTEM32, WINDOWS, TEMP, WINLETTER, DOCUMENTS
};

UnicodeString getDirectory(Directories directory)
{
	wchar_t result[MAX_PATH];
	memset(result, 0, MAX_PATH* 2);
	switch(directory)
	{
		case Directories::SYSTEM32:
		{
			GetSystemDirectoryW(result, MAX_PATH);
			break;
		}
		case Directories::WINDOWS:
		{
			GetWindowsDirectoryW(result, MAX_PATH);
			break;
		}
		case Directories::TEMP:
		{
			GetTempPathW(MAX_PATH, result);
			break;
		}
		case Directories::WINLETTER:
		{
			GetWindowsDirectoryW(result, MAX_PATH);
			result[1] = L'\0';
			break;
		}
		case Directories::DOCUMENTS:
		{
			TRegistry* reg = new TRegistry;
			reg->RootKey   = HKEY_CURRENT_USER;
			if(reg->OpenKey("\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders", false))
			{
				UnicodeString a = reg->ReadString("Personal");
				if(a != "")
				{
					a = expandPath(a);
					memcpy(result, a.c_str(), a.Length()* 2);
				}
			}
			delete reg;
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
			if(!DeleteFileW(svdExport->FileName))
			{
				throw new Exception("File couldn't be create. Please try again.");
			}
		}

		UnicodeString dataPath = GetEnvironmentVariableW("AppData") + "\\Embarcadero\\BDS\\9.0\\";
		UnicodeString templatesPath = getDirectory(Directories::DOCUMENTS) + "\\RAD Studio\\code_templates\\";
		if(!DirectoryExists(dataPath) || !DirectoryExists(templatesPath))
		{
			throw new Exception("Embarcadero data folder couldn't be found. An RAD Studio installation is required.");
		}

		//
		// Registry
		//
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

			if(!FileExists(tempFile))
			{
				throw new Exception("Registry file couldn't be create. Please try again.");
			}

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
		else
		{
			throw new Exception("Registry file couldn't be create. Please try again.");
		}


		TZipFile* zip = new TZipFile;
		try
		{
			TZipFile::ZipDirectoryContents(svdExport->FileName, templatesPath, zcDeflate);
			zip->Open(svdExport->FileName, zmReadWrite);
			zip->Add(tempFile, "registry.reg", zcDeflate);

			TSearchRec sr;
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
		}
		__finally
		{
			zip->Close();
			delete zip;
		}

		if(!DeleteFileW(tempFile))
		{
			ShowMessage("Temp registry file couldn't be deleted.");
		}

		ShowMessage("File exported successfully.");
	}
}

void __fastcall TfrmMain::ImportClick(TObject* Sender)
{

	if(opdImport->Execute())
	{
		UnicodeString dataPath      = GetEnvironmentVariableW("AppData") + "\\Embarcadero\\BDS\\9.0\\";
		UnicodeString templatesPath = getDirectory(Directories::DOCUMENTS) + "\\RAD Studio\\code_templates\\";
		if(!DirectoryExists(dataPath) || !DirectoryExists(templatesPath))
		{
			throw new Exception("Embarcadero data folder couldn't be found. An RAD Studio installation is required.");
		}

		TZipFile* zip = new TZipFile;
		try
		{
			if(TZipFile::IsValid(opdImport->FileName))
			{
				throw new Exception("Invalid file format.");
			}

			zip->Open(opdImport->FileName, zmRead);

			UnicodeString tempDir = getTempFile() + "\\";

			zip->ExtractAll(tempDir);
			zip->Extract("/C", templatesPath + "C", true);
			zip->Extract("/Delphi", templatesPath + "Delphi", true);

			if(!DirectoryExists(tempDir))
			{
				throw new Exception("File couldn't be imported. Try again later.");
			}

			TSearchRec sr;
			if(FindFirst(tempDir + "*.*", faAnyFile, sr) == 0)
			{
				do
				{
					if(sr.Name == "C" || sr.Name == "Delphi")
					{
						if(!MoveFileExW(UnicodeString(tempDir + sr.Name).c_str(), UnicodeString(templatesPath).c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
						{
							throw new Exception("Files couldn't be moved. Try again later.");
						}
						continue;
					}
					if(ExtractFileExt(sr.Name) == ".config" || ExtractFileExt(sr.Name) == ".dst")
					{
						if(!MoveFileExW(UnicodeString(tempDir + sr.Name).c_str(), UnicodeString(dataPath + sr.Name).c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
						{
							throw new Exception("Files couldn't be moved. Try again later.");
						}
						continue;
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
							DeleteFileW(tempDir + sr.Name);
						}
						else
						{
							throw new Exception("Registry file couldn't be imported. Try again later.");
						}
						continue;
					}
				}
				while(FindNext(sr) == 0);
				FindClose(sr);
			}

			RemoveDir(tempDir);
			ShowMessage("File imported successfully.");
		}
		__finally
		{
			zip->Close();
			delete zip;
		}
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
		info.nShow        = 1;
		info.lpVerb       = L"runas";
		info.hwnd         = NULL;
		info.fMask        = SEE_MASK_NOCLOSEPROCESS;
		info.lpDirectory  = NULL;
		// ShellExecuteExW(& info);
		// Application->Terminate();
	}
	FreeLibrary(hModule);
}
