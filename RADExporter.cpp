#include <fmx.h>
#pragma hdrstop
#include <tchar.h>

USEFORM(
	"UfrmMain.cpp",
	frmMain);

extern "C" int FMXmain()
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TfrmMain), & frmMain);
		Application->Run();
	}
	catch(Exception& exception)
	{
		Application->ShowException(& exception);
	}
	catch(...)
	{
		try
		{
			throw Exception("");
		}
		catch(Exception& exception)
		{
			Application->ShowException(& exception);
		}
	}
	return 0;
}
