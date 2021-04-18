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