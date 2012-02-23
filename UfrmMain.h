// ---------------------------------------------------------------------------

#ifndef UfrmMainH
#define UfrmMainH
#include <FMX.Controls.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Types.hpp>
#include <System.Classes.hpp>
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
class TfrmMain : public TForm {
__published: // IDE-managed Components

	TButton *Button1;
	TListBox *ListBox1;
	TDropTarget *drpExport;
	TButton *btnExport;
	TStyleBook *StyleBook1;
	TDropTarget *drpImport;
	TSaveDialog *svdExport;
	TOpenDialog *opdImport;
	TButton *Button2;
	TButton *Import;
	TButton *Button4;
	void __fastcall drpExportDragDrop(TObject *Sender,
		const TDragObject &Data, const TPointF &Point);
	void __fastcall drpExportDragOver(TObject *Sender,
		const TDragObject &Data, const TPointF &Point, bool &Accept);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall btnExportClick(TObject *Sender);
	void __fastcall ImportClick(TObject *Sender);

private: // User declarations
		public : // User declarations
	__fastcall TfrmMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
// ---------------------------------------------------------------------------
#endif
