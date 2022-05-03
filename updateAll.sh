#!/bin/bash
#Questo script serve per aggiornare tutti i file che hanno un markdown modificato.
#Anche aggiorna il README.md in base a tutti i file.
#E finalmente compila il programma.
python3 babel/babel.py translate Structs.md
python3 babel/babel.py translate User.md
python3 babel/babel.py translate Node.md
python3 babel/babel.py translate main.md
python3 babel/babel.py translate compilazione.md
python3 babel/babel.py readme Structs.md Node.md User.md main.md compilazione.md
if [ $# != 0 ]; then
make $1
else
make 1
fi
