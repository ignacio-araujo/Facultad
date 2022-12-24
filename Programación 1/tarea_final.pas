function generadora (n: integer): integer;
type digitos = array[1..3] of integer;
var 
    d : digitos;
    a, b : Integer;
    
	(* Separa el natural recibido como parámetro por valor en sus dígitos, y los retorna en un arreglo de tipo digitos *)
    function Separar(n : Integer) : digitos;
    var d : digitos;
    begin
        d[1] := n div 100; (* Obtenemos el primer dígito *)
        d[2] := n mod 100 div 10; (* Obtenemos el segundo dígito *)
        d[3] := n mod 10; (* Obtenemos el tercer dígito *)
        Separar := d
    end;
    
	(* Ordena el arreglo recibido como parámetro por referencia en forma ascendente *)
    procedure Ordenar(var d : digitos);
    var i, j, aux : Integer;
    begin
		(* Repetimos las instrucciones 2 veces para contemplar casos donde por ejemplo d[1]>d[3] *)
        for i:=1 to 2 do
        begin
			(* Comparamos los dígitos en los índices: 1 y 2, 2 y 3 *)
            for j:=1 to 2 do
            begin
                if d[j] > d[j+1] then
                begin
					(* Con la ayuda de una variable auxiliar intercambiamos de lugar d[j] y d[j+1] *)
                    aux := d[j];
                    d[j] := d[j+1];
                    d[j+1] := aux
                end
            end
        end
    end;

begin
    d := Separar(n);
    Ordenar(d);
    a := d[3]*100+d[2]*10+d[1]; (* a es el mayor natural con los dígitos de n. Multiplicamos para trabajarlo como integer *)
    b := d[1]*100+d[2]*10+d[3]; (* b es el menor natural con los dígitos de n *)
    generadora := a-b;
end;

function longitud (semilla: integer; limite: integer): integer;
var n, anterior, i : Integer;
begin
    i := 0;
    n := semilla;
    repeat
        anterior := n;
        n := generadora(anterior);
        i := succ(i)
    until (n = anterior) or (i > limite);
	
    if i > limite then longitud := -1
    else longitud := i
end;