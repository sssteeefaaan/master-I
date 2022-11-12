import sys
from PyQt5 import QtCore, QtGui, QtWidgets, uic
from PyQt5.QtCore import Qt


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, size):
        super().__init__()

        self.windowSize = size
        self.label = QtWidgets.QLabel()
        self.canvas = QtGui.QPixmap(size.width(), size.height())
        self.canvas.fill(QtGui.QColor("white"))
        self.label.setPixmap(self.canvas)
        self.setCentralWidget(self.label)
        self.draw_something()

    def draw_something(self):
        points = list()
        MAXITER = 10
        width, height = self.windowSize.width(), self.windowSize.height()
        for x in range(width):
            for y in  range(height):
                c = QtCore.QPointF(-2.0 + 2.5 * x / width, 1.125 * y / height)
                z = QtCore.QPointF(c.x(), c.y())
                iter = 0
                run = False
                while iter < MAXITER and not run:
                    temp = (z.x() * z.x()) - (z.y() * z.y()) + c.x()
                    z.setY(z.x() * z.y() * 2 + c.y())
                    z.setX(temp)
                    run = (z.x() * z.x() + z.y() * z.y()) > 4.0
                    if run:
                        points.append(z)
                    iter += 1
        painter = QtGui.QPainter(self.label.pixmap())
        painter.drawPoints(points)
        painter.end()


app = QtWidgets.QApplication(sys.argv)
screenSize = QtCore.QSize(500, 400)#app.primaryScreen().size()
window = MainWindow(screenSize)
window.show()
app.exec_()