#!/bin/bash
pwd
cd ./src

echo -e "\n" >>../latex/sezioni/codice.tex
for f in * 
do
testo="\lstinputlisting[language=C++]{../src/${f}}"

echo $testo >>../latex/sezioni/codice.tex

done

exit 0;

