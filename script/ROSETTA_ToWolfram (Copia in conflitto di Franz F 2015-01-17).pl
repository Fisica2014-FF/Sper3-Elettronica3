#!/usr/bin/perl
use strict;
use warnings;
use Data::Dumper; # Package per testare gli hash vari
use File::Basename;
use 5.010;

# Cosa voglio:
# ./script/ROSETTA_ToWolfram.pl ./variabili/NomeFileRosetta
# Dato un file .rosetta

open FILEVAR, "<", $ARGV[0] or die "[Errore]: Errore nell'apertura del file di dati";

# Apri il file di dati in sola lettura con la funzione separa_ai_puntievirgola definita sotto
#@lines = separa_ai_puntievirgola($ARGV[0]);

# se ./ciao/Darth.fener allora $name = Dart, $path = ./ciao/, $suffix = vader
my ($namev,$pathv,$suffixv) = fileparse($ARGV[0],qr/\.[^.]*/); #File delle variabili

my @lines = <FILEVAR>; # Array delle righe
my @expressions;	# Array delle expressions, che non coincidono con le righe perché non ci sono i commenti e finiscono ai punti e virgola
my $operaz = qr/[\+\*\/\-\^\(\),]/; # Variabile regex che contiene i vari "delimitatori di variabile" (operazioni, parentesi...)
my $variabile = qr/[a-zA-Z_][a-zA-Z_0-9]*/; # Nome di variabile, alfanumerica, non può iniziare per un numero
my $numeroint = qr/\-?\d+/;		   # Numero come -4
my $numerofloat = qr/\-?\d+\.\d+/;	   # Numero della forma -3.42 o 42.0
my $numero = qr/$numeroint|$numerofloat/;   # Sia -3 che 3.0, per esempio
my $espressione = qr/$operaz?($numero|$variabile)/;
# Matcha una funzione f(x,y) (MA NON LA DEFINIZIONE, eg "f(x) = ..." solo f(x) ma NON la parte dopo l'uguale), con dei named groups
# (?<args> ... ) = gruppo di cattura chiamato "args", usabile con \g{args} o con $+{args}
# Parentesi tonde incasinate ma sono necessarie per salvare la roba giusta
my $deffunzione = qr/(?<nomefunzione>$variabile)\((?<argomentifunzione>(($variabile)\,?)*)\)/;
# Include anche f(2), non solo f,x
my $deffunzione2 = qr/(?<nomefunzione>$variabile|$numero)\((?<argomentifunzione>(($variabile|$numero)\,?)*)\)/;
# Funzione generica , valide anche robe tipo f( ,,x[[ )
my $deffunzione3 = qr/(?<nomefunzione>$variabile|$numero)\((?<argomentifunzione>(([a-zA-Z0-9\+\*\/\-\^\(\),])\,?)*)\)/;

# Qui togliamo le righe vuote e dei commenti
my $filesenzacommenti;
CICLOCOMMENTI:
foreach my $linea (@lines) {
	next CICLOCOMMENTI if ($linea =~ /^$/ or $linea =~ /^\#.*/); # Salta le righe vuote coi commenti che iniziano con #
	$filesenzacommenti = $filesenzacommenti.$linea;		      # Creiamo una mega stringa col contenuto del file meno commenti e righe vuote
	@expressions = split(';',$filesenzacommenti);		      # La splittiamo in un array (nota che non ho tolto gli spazi)
}
# say $filesenzacommenti;
# say '-' x 30;
# say @expressions;

CICLOVAR:
foreach my $expr (@expressions) {
	# la x indica che permettiamo spazi e commenti
	next CICLOVAR if ($expr =~ /^$/ or $expr =~ /^\# .*/); #Salta le righe vuote coi commenti che iniziano con #
	# Altrimenti siamo nei casi sotto
	$expr =~ s/\s//g; # Togli gli spazi
	
	# a = 1.23
	if ($expr =~ / ($variabile)  \=  ($numerofloat) /x) {
		#       ^^^^^      ^^^^^^^^^^^^^^^^
		#       $1               $2
		# Memorizza il primo gruppo di cattura come nome di variabile, e associaci il valore del dopo l'uguale
		# gruppo
		print $expr."\n";
	}
	 # a=3 (e non a=3.0)
	 # WOLFRAM_SPECIFIC Non dovrebbe cambiare niente per wolfram
	elsif	($expr =~ / ($variabile)  \=  ($numeroint) /x) {
		print $expr."\n";
	} 
	
	# Funzione, es fun32(x,y) = x^2+a*x
	# (?<args> ... ) = gruppo di cattura chiamato "args", usabile con \g{args} o con $+{args}
	elsif	
	($expr =~ /($deffunzione) \= (?<corpofunzione> [\+ \* \/ \- \^ \( \) , _ a-z A-Z 0-9]+ )/x) {
		
		
		my @listavar = split(',',$+{argomentifunzione});
		# say $+{argomentifunzione};
		my @nomef = $+{nomefunzione};
		my @argf = $+{argomentifunzione};
		while ($expr =~ / ($deffunzione3) /x ) {
			my @nomef2 = $+{nomefunzione};
			my @argf2 = $+{argomentifunzione};
			$expr =~ s/$deffunzione3/@nomef2\[@argf2\]/x;
		} 
		foreach my $variabile (@listavar) {
			$expr =~ s/$variabile/${variabile}_/g;
		}
		$expr =~ s/\=/:=/;
		print $expr."\n";
	}
	else
	{
		die "[Errore]: Riga in formato sconosciuto";
	}
}