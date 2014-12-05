# Para ejecutarlo perl gen_xml_qh_esc_aspersion.pl 1 30 120 1900 5100 5100 0.9 1.0 
# Significa escenario 1 de aspersión de Enrique, con 30 minutos de ancho de cada escalon y 120 minutos de ancho total. 1900 segundos de desfase de Almudévar,5100 segundos de desfase de Gurrea y 5100 segundos de desfase de el Temple. Altura del agua para calibración inicial apertura en Almudévar 0.9m y en Gurrea y Temple 1.0 m

$escenario=$ARGV[0];
$d_alm=$ARGV[1]; # desfase de Almudevar respecto de la entrada de caudal (en segundos)
$d_gur=$ARGV[2]; # desfase de Gurrea respecto de la entrada de caudal (en segundos)
$d_tem=$ARGV[3]; # desfase de el Temple respecto de la entrada de caudal (en segundos)

open(FILE,'GEOMETRIA.xml');
@tomas=<FILE>;
close(FILE);

foreach $toma(@tomas)
{
	print $toma;
}

print <<END1;
    </geometry>
    <initial type="steady"/>
    <inlet type="qt">
END1

open(FILE,'SumaAGT2semJunio.csv');
@lf=<FILE>;  #almacenas en el array @ls todas las lineas del fichero caudales_junio.csv 
close(FILE);
$i=0;
foreach $la(@lf)
{
   	$la=~s/[\r\n ]+//;  #eliminas retornos de carro, lineas nuevas y espacios en blanco
   	@ds=split(/\s/,$la);  #almacenas en el array @ds cada elemento de una linea
	
	$p[$i]=$ds[7-$escenario]/1000.; #Es el caudal total de las 3 comunidades para cada dia (en m3/s)
	$i++;
}
for($i=0;$i<14;$i++) 
{
	printf "      2010 1 %d 0 0 0 %g\n",$i+1,$p[$i]+2.; #Le sumo 2 m3/s que es lo que supongo que sale a través de la compuerta transversal final
	printf "      2010 1 %d 0 0 0 %g\n",$i+2,$p[$i]+2.; #Le sumo 2 m3/s que es lo que supongo que sale a través de la compuerta transversal final
}
print "    <\/inlet>\n";

open(FILE,'Almudevar2PrSemJun.csv');
@lf=<FILE>;
close(FILE);
$i=0;
foreach $la(@lf)
{
   	$la=~s/[\r\n ]+//;  #eliminas retornos de carro, lineas nuevas y espacios en blanco
   	@ds=split(/\s/,$la);  #almacenas en el array @ds cada elemento de una linea
	
	$p[$i]=$ds[7-$escenario]/1000.; #Es el caudal para Almudevar en m3/s
	$i++;
}
printf "    <boundary name=\"extAlm\" type=\"qt\" initial=\"9\" delay=\"%g\">\n",$d_alm;
for($i=0;$i<14;$i++) 
{
	printf "      2010 1 %d 0 0 0 %g\n",$i+1,$p[$i];
	printf "      2010 1 %d 0 0 0 %g\n",$i+2,$p[$i];
}
printf "    </boundary>\n";

open(FILE,'Gurrea2PrSemJun.csv');
@lf=<FILE>;
close(FILE);
$i=0;
foreach $la(@lf)
{
   	$la=~s/[\r\n ]+//;  #eliminas retornos de carro, lineas nuevas y espacios en blanco
   	@ds=split(/\s/,$la);  #almacenas en el array @ds cada elemento de una linea
	
	$p[$i]=$ds[7-$escenario]/1000.; #Es el caudal para Gurrea en m3/s
	$i++;
}
printf "    <boundary name=\"extGur\" type=\"qt\" initial=\"32\" delay=\"%g\">\n",$d_gur;
for($i=0;$i<14;$i++) 
{
	printf "      2010 1 %d 0 0 0 %g\n",$i+1,$p[$i];
	printf "      2010 1 %d 0 0 0 %g\n",$i+2,$p[$i];
}
printf "    </boundary>\n";

open(FILE,'Temple2PrSemJun.csv');
@lf=<FILE>;
close(FILE);
$i=0;
foreach $la(@lf)
{
   	$la=~s/[\r\n ]+//;  #eliminas retornos de carro, lineas nuevas y espacios en blanco
   	@ds=split(/\s/,$la);  #almacenas en el array @ds cada elemento de una linea
	
	$p[$i]=$ds[7-$escenario]/1000.; #Es el caudal para El Temple en m3/s
	$i++;
}
printf "    <boundary name=\"extTem\" type=\"qt\" initial=\"33\" delay=\"%g\">\n",$d_tem;
for($i=0;$i<14;$i++) 
{
	printf "      2010 1 %d 0 0 0 %g\n",$i+1,$p[$i];
	printf "      2010 1 %d 0 0 0 %g\n",$i+2,$p[$i];
}
printf "    </boundary>\n";

print <<END5;
    <outlet type = "gate" height = "0.85" width = "1.5">
      0 0.55
    </outlet>
  </channel>
END5
print "<\/system>";

sub apert #apertura en función de (Q,h,B)
{
	my($Q,$h,$B);
	($Q, $h, $B) = @_;
	($Q*$Q+$Q*sqrt($Q*$Q+8.*$B*$B*$h*$h*$h*9.81))/(2.44*$h*$h*$B*$B*9.81);
}

##Ejemplo de subrutina en Perl
##printf "\n\n\n%g \n",&max(15,569);
##
##sub max {
##  my($m, $n);       # new, private variables for this block
##  ($m, $n) = @_;    # give names to the parameters
##  if ($m > $n) { $m } else { $n }
#}

