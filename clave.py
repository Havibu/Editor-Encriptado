#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Mar  3 08:32:25 2022

@author: javier
"""

import sys
from PyQt5.QtWidgets import (QWidget, QDesktopWidget, 
                             QApplication, QLabel, QLineEdit,
                             QTextEdit, QGridLayout)
from PyQt5.Qt import *

class Clave(QWidget):
    
    def __init__(self):
        super().__init__()
        
        self.iniciarIU()
        
    def iniciarIU(self):
        
        self.resize(400, 200)
        self.centrar()
        
        titulo = QLabel('Introduzca la contraseña')
        clave = QLineEdit()
        
        #Establecemos el layout
        grid = QGridLayout()
        grid.setSpacing(5)
        
        grid.addWidget(titulo, 0, 0, 1, 1, alignment=Qt.AlignHCenter)
        grid.addWidget(clave, 1, 0, 1, 1)
        grid.addWidget(QLabel(' '), 2, 0, 3, 1)
        
        self.setLayout(grid)
        self.setWindowTitle('DIAR - I/0')
        self.show()
        
    def centrar(self):
        ventana = self.frameGeometry()
        pantalla = QDesktopWidget().availableGeometry().center()
        ventana.moveCenter(pantalla)
        self.move(ventana.topLeft())
        
def main():
        
    app = QApplication(sys.argv)
    programa = Clave()
    sys.exit(app.exec_())
        
if __name__ == '__main__':
    main()       