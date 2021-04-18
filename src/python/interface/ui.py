from PyQt5.QtWidgets import (QWidget, QSlider, QHBoxLayout,
                             QLabel, QApplication)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
import sys

from PyQt5.QtGui import QColor
from widgets.slider import Slider

def main():
    app = QApplication(sys.argv)
    slider1 = Slider()
    slider2 = Slider()
    app.addWidget(slider1)
    app.addWidget(slider2)
    app.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()