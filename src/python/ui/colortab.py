from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QWidget

class ColorTab(QWidget):
    def paintEvent(self, event):
        self.qp = QPainter(self)

        self.qp.setBrush(QColor.black)
        self.qp.drawRect(0, 0, 100, 100)

    def updateColor(self, color):
        self.qp.setBrush(color)