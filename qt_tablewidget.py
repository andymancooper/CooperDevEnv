##
## qt_tablewidget.py
##
## Downloaded from http://www.saltycrane.com/blog/2006/10/qtablewidget-example-using-python-24/
##

import sys
from PyQt4.QtGui import *

lista = ['aa', 'ab', 'ac']
listb = ['ba', 'bb', 'bc']
listc = ['ca', 'cb', 'cc']
mystruct = {'A':lista, 'B':listb, 'C':listc}

class MyTable(QTableWidget):
    def __init__(self, thestruct, *args):
        QTableWidget.__init__(self, *args)
        self.data = thestruct
        self.setmydata()
        
    def setmydata(self):
        n = 0
        for key in self.data:
            m = 0
            for item in self.data[key]:
                newitem = QTableWidgetItem(item)
                self.setItem(m, n, newitem)
                m += 1
            n += 1

def main(args):
    app = QApplication(args)
    table = MyTable(mystruct, 5, 3)
    table.show()
    sys.exit(app.exec_())
    
if __name__=="__main__":
    main(sys.argv)
