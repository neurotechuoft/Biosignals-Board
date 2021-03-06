import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QMessageBox
from gui_layout import Ui_MainWindow
import pyqtgraph as pg
import numpy as np

class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):

    #placeholder function, used to test functions in app
    def testfunction(self):
        print("Works")    

    #closes whole application
    def close(self):    

        choice = QMessageBox.question(self, 'Message',
                                     "Are you sure?", QMessageBox.Yes |
                                     QMessageBox.No, QMessageBox.No)

        if choice == QMessageBox.Yes:
            print('quit application')
            sys.exit()
        else:
            pass

    #run on start up    
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent=parent)
        self.setupUi(self)

        self.pushButton_2.clicked.connect(self.testfunction)
        self.pushButton.clicked.connect(self.close)    

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())
