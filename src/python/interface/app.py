from PyQt5.QtWidgets import (QWidget, QSlider, QHBoxLayout,
                             QLabel, QApplication)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
import sys

from PyQt5.QtGui import QColor
from kelvin import kelvin_table
from colortab import ColorTab


class Example(QWidget):

    def __init__(self):
        super().__init__()

        self.initUI()

    def initUI(self):

        hbox = QHBoxLayout()

        sld = QSlider(Qt.Horizontal, self)
        sld.setRange(0, 110)
        sld.setFocusPolicy(Qt.NoFocus)
        sld.setPageStep(5)

        sld.valueChanged.connect(self.updateLabel)

        self.label = QLabel('0', self)
        self.label.setAlignment(Qt.AlignCenter | Qt.AlignVCenter)
        self.label.setMinimumWidth(80)
        self.colorTab = ColorTab(self)
        self.colorTab.updateColor(QColor.red)
        hbox.addWidget(sld)
        hbox.addWidget(self.colorTab)
        hbox.addSpacing(15)
        hbox.addWidget(self.label)

        self.setLayout(hbox)

        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle('QSlider')
        self.show()

    def updateLabel(self, value):
        kelvin_value = kelvin_table[(value + 10) * 100]
        self.label.setText(str((value + 10) * 100) + ' kelvin')


def main():

    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
# app = QApplication([])

# my_color_picker = ColorPicker(useAlpha=False)
# my_color_picker_light = ColorPicker(lightTheme=False)


# old_color = (255,255,255,)
# picked_color = my_color_picker.getColor(old_color)
# print(picked_color)


# old_color = (255,0,255)
# picked_color = my_color_picker_light.getColor(old_color)
# print(picked_color)


# # Don't have your color in RGB format?
# my_color = (50, 50, 100) # HSV Color in percent
# old_color = my_color_picker.hsv2rgb(my_color)
# picked_color = my_color_picker.rgb2hsv(my_color_picker.getColor(old_color))
# print(picked_color)