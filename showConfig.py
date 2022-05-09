#!/usr/bin/python3
import sys
"""
El objetivo de este script es para poder confirmar los valores de cada
configuracion que se pasa como unico parametro. Esto nos sirve tanto 
para ver los valores del archivo como para controlar que los valores 
estan en el orden correcto, que seria:
    1)  SO_USER_NUM
    2)  SO_NODES_NUM
    3)  SO_BUDGET_INIT
    4)  SO_REWARD
    5)  SO_MIN_TRANS_GEN_NSEC
    6)  SO_MAX_TRANS_GEN_NSEC
    7)  SO_RETRY
    8)  SO_TP_SIZE
    9)  SO_MIN_TRANS_PROC_NSEC
    10) SO_MAX_TRANS_PROC_NSEC
    11) SO_SIM_SEC
    12) SO_FRINDS_NUM
    13) SO_HOPS
"""
archivo = open(sys.argv[1],'r')
contenido = archivo.readlines()

print("SO_USER_NUM:",contenido[0][0:-1])
print("SO_NODES_NUM:",contenido[1][0:-1])
print("SO_BUDGET_INIT:",contenido[2][0:-1])
print("SO_REWARD:",contenido[3][0:-1])
print("SO_MIN_TRANS_GEN_NSEC:",contenido[4][0:-1])
print("SO_MAX_TRANS_GEN_NSEC:",contenido[5][0:-1])
print("SO_RETRY:",contenido[6][0:-1])
print("SO_TP_SIZE:",contenido[7][0:-1])
print("SO_MIN_TRANS_GEN_NSEC:",contenido[8][0:-1])
print("SO_MAX_TRANS_GEN_NSEC:",contenido[9][0:-1])
print("SO_SIM_SEC:",contenido[10][0:-1])
print("SO_FRINDS_NUM:",contenido[11][0:-1])
print("SO_HOPS:",contenido[12][0:-1])

archivo.close()
