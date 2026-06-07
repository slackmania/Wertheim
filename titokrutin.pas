unit titokrutin;

//{$H+}


interface

uses
  Classes, SysUtils,DCPrc4, DCPsha1;

var
   sazonosito : string;

   function besecure(mit : string; pw : string) : string;
   function kisecure(mit : string; pw : string) : string;

implementation

 function besecure(mit : string; pw : string) : string;
 var
   kulcs: String;
   Cipher: TDCP_rc4;

 begin

       kulcs:=pw;

       Cipher:= TDCP_rc4.Create(nil);
       Cipher.InitStr(pw,TDCP_sha1);

       besecure:= Cipher.EncryptString(mit);


       Cipher.Burn;
       Cipher.Free;

 end;

 function kisecure(mit : string; pw : string) : string;
 var     Cipher: TDCP_rc4;
    s: string;
  begin

      Cipher:= TDCP_rc4.Create(nil);
      Cipher.InitStr(pw,TDCP_sha1);

        kisecure:= Cipher.DecryptString(mit);

      Cipher.Burn;
      Cipher.Free;
end;


begin
   sazonosito:='Mihályi József * Titkosító Unit * 2019';
end.

