#ifndef UfrmMainH
#define UfrmMainH
#include <FMX.Controls.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>

class TfrmMain : public TForm
{
	__published:
	TButton*     btnExport;
	TSaveDialog* svdExport;
	TOpenDialog* opdImport;
	TButton*     Import;

	void __fastcall btnExportClick(TObject* Sender);
	void __fastcall ImportClick(TObject* Sender);
	void __fastcall FormCreate(TObject* Sender);

	private:
	public:
	__fastcall TfrmMain(TComponent* Owner);
};

extern PACKAGE TfrmMain* frmMain;

#endif
