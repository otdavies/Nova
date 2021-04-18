from PyQt5.QtWidgets import (QWidget, QSlider, QHBoxLayout, QLabel)
from PyQt5.QtCore import Qt

class Slider(QWidget):

    def __init__(self):
        super().__init__()
        self.initSlider()

    def initSlider(self):
        # Setup slider
        sld = QSlider(Qt.Horizontal, self)
        sld.setRange(0, 110)
        sld.setFocusPolicy(Qt.NoFocus)
        sld.setPageStep(5)
        sld.valueChanged.connect(self.valuechange)
        
        # Setup label
        self.label = QLabel('0', self)
        self.label.setAlignment(Qt.AlignCenter | Qt.AlignVCenter)
        self.label.setMinimumWidth(80)

        # Add to box
        hbox = QHBoxLayout()
        hbox.addWidget(sld)
        hbox.addSpacing(15)
        hbox.addWidget(self.label)
        self.setLayout(hbox)

    def valuechange(self, value):
        self.label.setText(str(value))