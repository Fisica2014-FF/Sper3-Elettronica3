#!/bin/bash
pwd
cd ./gnuplot/immagini

for f in * 
do
testo="
\begin{grafico}
  \centering
  \input{../gnuplot/immagini/${f}}
  \caption{Grafico ${f}}
  \label{gr:${f}}
\end{grafico}
"

echo $testo >>../../latex/sezioni/pres_dati_grafici.tex

done

exit 0;
