unit Unit1;

//{$mode objfpc}{$H+}

interface

uses
    Forms, StdCtrls, Classes, SysUtils, strutils,lazutils, Controls, Graphics, Dialogs,lazutf8,
 ExtCtrls, lcltype, DCPsha1, DCPrc4,FileUtil,
  titokrutin, hinta ; // ,  windows, ,
  //,versionsupport


type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    Button6: TButton;
    DCP_rc4_1: TDCP_rc4;
    DCP_sha1_1: TDCP_sha1;
    Edit1: TEdit;
    Edit2: TEdit;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    rmemo1: TMemo;
    Memo2: TMemo;
    Memo3: TMemo;
    OpenDialog1: TOpenDialog;
    Panel1: TPanel;
    Panel2: TPanel;
    SaveDialog1: TSaveDialog;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Edit1Enter(Sender: TObject);
    procedure Edit1KeyPress(Sender: TObject; var Key: char);
    procedure FormCreate(Sender: TObject);
    procedure FormKeyPress(Sender: TObject; var Key: char);

    procedure GroupBox1Click(Sender: TObject);
    procedure rMemo1Change(Sender: TObject);
    procedure rMemo1KeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);

  private

  public

  end;

var
  Form1: TForm1;
  nyitni : utf8string;
  fajlnev : utf8string;
  kertal : integer; // ez fogja a keresésben a talált poziciókat léptetni
  kerstr : string;  // keresés stringje

implementation

{$R *.lfm}

{ TForm1 }



procedure TForm1.Edit1Enter(Sender: TObject);
begin

end;

procedure TForm1.Button2Click(Sender: TObject);
var x : utf8string;
begin
  // DECODE
       memo2.text:=hintaki(rmemo1.text,edit1.text);
      rmemo1.text:=memo2.text;

      x:=replacetext(fajlnev,'.wtx','.txt');
      label4.caption:=x;
      rmemo1.SetFocus;
end;


 //function FindMemoLineNumber(AMemo: trichMemo; const AString: String; StartPos: Integer): Integer;
 function FindMemoLineNumber(AMemo: tMemo; const AString: String; StartPos: Integer): Integer;
begin
  AMemo.HideSelection := False;
  Result := PosEx(AString, AMemo.Text, StartPos);

   if result > 0 then begin
        AMemo.SelStart := UTF8Length(PChar(AMemo.Text), Pred(Result));
        AMemo.SelLength := UTF8Length(AString);
        Result := Succ(AMemo.CaretPos.Y); // indexed from 1
        kertal:=kertal+AMemo.SelLength+AMemo.SelStart+1;
    end else begin
      kertal:=1;
    end;
 end;




 procedure TForm1.Button6Click(Sender: TObject);
var
  i: Integer;
  s : string;
begin
      kerstr:=edit2.text;
      groupbox1.visible:=false;
      rmemo1.setfocus;
  i:=findmemolinenumber(rmemo1,kerstr,1);


end;






procedure TForm1.Button5Click(Sender: TObject);
var x : utf8string;
begin
  // ENCODE
     memo2.text:=hintabe(rmemo1.text,edit1.text);
      rmemo1.text:=memo2.text;
      x:=replacetext(fajlnev,'.txt','.wtx');
      label4.caption:=x;
end;





procedure TForm1.Button3Click(Sender: TObject);
var n : utf8string;
begin
      n:= opendialog1.filename;
      if (pos('.wtx',n)=0) then
         savedialog1.filename:= n +'.wtx'
           else
         savedialog1.filename:= n;

   if savedialog1.execute then begin
       // SAVE
     fajlnev:=n;
       rmemo1.Lines.SaveToFile(savedialog1.filename);
     label4.caption:=fajlnev;
end;

end;

procedure TForm1.Button4Click(Sender: TObject);
begin
  close;
end;

procedure TForm1.Button1Click(Sender: TObject);
var n : string;
begin
     if opendialog1.execute then begin
       // LOAD
        n:= opendialog1.filename;

      if pos('.wtx',n)>0 then begin
       rmemo1.Lines.LoadFromFile(opendialog1.filename);

        memo2.text:=hintaki(rmemo1.text,edit1.text);
        rmemo1.text:=memo2.text;
       fajlnev:= opendialog1.filename;
     end;
     label4.caption:=fajlnev;

     end;
end;

procedure TForm1.Edit1KeyPress(Sender: TObject; var Key: char);
begin
   //  ENTER lett nyomva a jelszóbevitelen
  if key=#13 then begin
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^3
    // GYAKORLATILAG ITT KEZDŐDIK
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^3
    rmemo1.enabled:=true;
    button1.enabled:=true;
    button2.enabled:=true;
    button3.enabled:=true;
    button5.enabled:=true;
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  rmemo1.enabled:=false;
  rmemo1.text:='';
  kertal:=0;

 // form1.caption:='WERTHEIM '+getfileversion;
      button1.enabled:=false;
    button2.enabled:=false;
    button3.enabled:=false;
    button5.enabled:=false;
    if nyitni<>'' then begin
         memo2.Lines.LoadFromFile(nyitni);
       rmemo1.Text:=SysToUTF8(memo2.text);
    end;
    groupbox1.visible:=false;
end;

procedure TForm1.FormKeyPress(Sender: TObject; var Key: char);

begin


end;



procedure TForm1.GroupBox1Click(Sender: TObject);
begin

end;

procedure TForm1.rMemo1Change(Sender: TObject);
begin

end;


procedure TForm1.rMemo1KeyDown(Sender: TObject; var Key: Word; Shift: TShiftState
  );
var
  n,x : utf8string;
  i : integer;

// GyorsGombok definíciója // -------------------------------------------
begin
   if (Key = VK_F) and (ssCtrl in Shift) then begin
      edit2.text:='';
       groupbox1.visible:=enabled;
       edit2.SetFocus;
   end;

   if (Key = VK_L) and (ssCtrl in Shift) then begin   // CTRL L

       i:=FindMemoLineNumber(rmemo1, kerstr, kertal) ;
   end;

   if (Key = VK_S) and (ssCtrl in Shift) then begin
      n:= label4.caption;
      memo3.text:=rmemo1.text;
      // Ha text formátum, akkor előszor elmenti, elkodolja, a kodoltat elmenti majd visszaallitja az elmentett szovegformatumut
      if pos('.txt',n)>0 then begin
          memo2.text:=hintabe(rmemo1.text,edit1.text);
          rmemo1.text:=memo2.text;
      end;
      savedialog1.filename:=n;
      if savedialog1.execute then begin
       // SAVE
        fajlnev:=n;
        x:=replacetext(fajlnev,'.txt','.wtx');
        rmemo1.Lines.SaveToFile(x);
      end;
      rmemo1.text:=memo3.text;
    end;
// GyorsGombok definíciója // -------------------------------------------
end;




begin
if paramcount<>0 then nyitni := paramstr(1) else nyitni:='';

end.

