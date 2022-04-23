#!/bin/bash
#Questo script serve per aggiornare tutti i file che hanno un markdown modificato.
#Anche aggiorna il README.md in base a tutti i file.
#E finalmente compila il programma.
python3 babel/babel.py tangle Structs.md
python3 babel/babel.py tangle User.md
python3 babel/babel.py tangle Node.md
python3 babel/babel.py tangle main.md
python3 babel/babel.py readme Structs.md Node.md User.md main.md
make
