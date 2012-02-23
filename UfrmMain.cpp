#include <fmx.h>
#pragma hdrstop

#include "UfrmMain.h"
#include "Winapi.ShellAPI.hpp"
#include "System.Zip.hpp"
#include "Registry.hpp"

#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmMain *frmMain;

__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
}

void __fastcall TfrmMain::drpExportDragDrop(TObject *Sender, const TDragObject &Data, const TPointF &Point)
{

	if (Data.Files.Length != 1 || !DirectoryExists(Data.Files[0]))
	{
		return;
	}

	if (((TDropTarget*)(Sender))->Name == "drpExport")
	{
		SHELLEXECUTEINFOW info;
		info.cbSize = sizeof(SHELLEXECUTEINFOW);
		info.lpFile = L"regedit.exe";
		info.lpParameters = UnicodeString("/e \"" + Data.Files[0] + "\\registry.reg\" \"HKEY_CURRENT_USER\\Software\\Embarcadero\"").c_str();
		info.nShow = 0;
		info.lpVerb = L"open";
		info.hwnd = NULL;
		info.fMask = SEE_MASK_NOCLOSEPROCESS;
		info.lpDirectory = NULL;
		int exe = ShellExecuteExW(&info);
		if (exe)
		{
			WaitForSingleObject(info.hProcess, INFINITE);
			CloseHandle(info.hProcess);

			TStringList* stl = new TStringList;
			stl->LoadFromFile(Data.Files[0] + "\\registry.reg");
			for (int i = stl->Count - 1; i >= 0; i--)
			{
				if (stl->Strings[i].Pos((UnicodeString)"\"" + "\\\\") > 0 || stl->Strings[i].Pos("C:\\") > 0 || stl->Strings[i].Pos("c:\\") > 0 || stl->Strings[i].Pos("D:\\") > 0 || stl->Strings[i].Pos("d:\\") > 0 || stl->Strings[i].Pos("\"RegOwner\"=\"") > 0)
				{
					ListBox1->Items->Add(IntToStr(i) + " - " + stl->Strings[i]);
					stl->Delete(i);
				}
			}
			stl->SaveToFile(Data.Files[0] + "\\registry.reg");

			TSearchRec sr;
			UnicodeString dataPath = GetEnvironmentVariableW("AppData") + "\\Embarcadero\\BDS\\9.0\\";
			if (FindFirst(dataPath + "*.*", faAnyFile, sr) == 0)
			{
				do
				{
					ListBox1->Items->Add(dataPath + sr.Name);

					if (ExtractFileExt(sr.Name) == ".config" || ExtractFileExt(sr.Name) == ".dst")
					{
						// CopyFileW();
					}
				}
				while (FindNext(sr) == 0);
				FindClose(sr);
			}

			delete stl;
		}

	}
	else
	{

	}
}

void __fastcall TfrmMain::drpExportDragOver(TObject *Sender, const TDragObject &Data, const TPointF &Point, bool &Accept)
{
	Accept = true;
}

void __fastcall TfrmMain::Button1Click(TObject *Sender)
{
	TStringList* stl = new TStringList;
	stl->LoadFromFile("D:\\ae.reg");
	for (int i = stl->Count - 1; i >= 0; i--)
	{
		if (stl->Strings[i].Pos((UnicodeString)"\"" + "\\\\") > 0 || stl->Strings[i].Pos("C:\\") > 0 || stl->Strings[i].Pos("c:\\") > 0 || stl->Strings[i].Pos("D:\\") > 0 || stl->Strings[i].Pos("d:\\") > 0)
		{
			ListBox1->Items->Add(IntToStr(i) + " - " + stl->Strings[i]);
			stl->Delete(i);
		}
	}
	stl->SaveToFile("D:\\ae2.reg");
	delete stl;
}

void __fastcall TfrmMain::btnExportClick(TObject *Sender)
{
	if (svdExport->Execute())
	{
		if (FileExists(svdExport->FileName))
		{
			DeleteFileW(svdExport->FileName);
		}
		TZipFile* zip = new TZipFile;
		zip->Open(svdExport->FileName, zmWrite);

		// Files
		TSearchRec sr;
		UnicodeString dataPath = GetEnvironmentVariableW("AppData") + "\\Embarcadero\\BDS\\9.0\\";
		if (FindFirst(dataPath + "*.*", faAnyFile, sr) == 0)
		{
			do
			{
				if (ExtractFileExt(sr.Name) == ".config" || ExtractFileExt(sr.Name) == ".dst")
				{
					zip->Add(dataPath + sr.Name, sr.Name, zcStored);
				}
			}
			while (FindNext(sr) == 0);
			FindClose(sr);
		}

		// Registry
		// TRegistry reg;
		// reg.SaveKey("HKEY_CURRENT_USER\\Software\\Embarcadero", "C:\\aa.reg");

		zip->Close();
		delete zip;
	}

	return;
	typedef bool(*_IsUserAnAdmin)(VOID);
	_IsUserAnAdmin IsUserAnAdmin;
	HMODULE hModule = NULL;
	hModule = LoadLibraryW(L"shell32.dll");

	IsUserAnAdmin = (_IsUserAnAdmin)GetProcAddress(hModule, "IsUserAnAdmin");

	ShowMessage((int)IsUserAnAdmin());
	CloseHandle(hModule);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmMain::ImportClick(TObject *Sender)
{
	
	if (opdImport->Execute())
	{
		ShowMessage(opdImport->FileName);
	}
}
// ---------------------------------------------------------------------------
