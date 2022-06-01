from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtCore import QSize

import syntax
from colors import colors

class LineNumberArea(QWidget):

    def __init__(self, editor):
        super(QWidget, self).__init__(editor)
        self.myeditor = editor

    def paintEvent(self, event):
        self.myeditor.lineNumberAreaPaintEvent(event)


class CodeEditor(QPlainTextEdit):


    def __init__(self, parent=None):
        super(QPlainTextEdit, self).__init__(parent)
        self.lineNumberArea = LineNumberArea(self)
        self.blockCountChanged.connect(self.updateLineNumberAreaWidth)
        self.updateRequest.connect(self.updateLineNumberArea)
        self.cursorPositionChanged.connect(self.highlightCurrentLine)
        # self.setBackgroundVisible(True)
        self.setPalette(QPalette(colors["black"]))
        self.setStyleSheet("font-family: Consolas; font-size: 18px; color: #BDBDBD;")
        self.setTabStopWidth(40)
        self.updateLineNumberAreaWidth(0)

    def onTextChange(self):
        syntax.CPPHighlighter(self.document())

    def lineNumberAreaWidth(self):
        space = 10 + self.fontMetrics().width('9') * 3
        return space


    def updateLineNumberAreaWidth(self, _):
        self.setViewportMargins(self.lineNumberAreaWidth(), 0, 0, 0)


    def updateLineNumberArea(self, rect, dy):

        if dy:
            self.lineNumberArea.scroll(0, dy)
        else:
            self.lineNumberArea.update(0, rect.y(), self.lineNumberArea.width(),
                       rect.height())

        if rect.contains(self.viewport().rect()):
            self.updateLineNumberAreaWidth(0)


    def resizeEvent(self, event):
        super(QPlainTextEdit, self).resizeEvent(event)

        cr = self.contentsRect()
        self.lineNumberArea.setGeometry(QRect(cr.left(), cr.top(),
                    self.lineNumberAreaWidth(), cr.height()))


    def lineNumberAreaPaintEvent(self, event):
        mypainter = QPainter(self.lineNumberArea)

        mypainter.fillRect(event.rect(), colors["black"])

        block = self.firstVisibleBlock()
        blockNumber = block.blockNumber()
        top = self.blockBoundingGeometry(block).translated(self.contentOffset()).top()
        bottom = top + self.blockBoundingRect(block).height()

        # Just to make sure I use the right font
        height = self.fontMetrics().height()
        while block.isValid() and (top <= event.rect().bottom()):
            if block.isVisible() and (bottom >= event.rect().top()):
                number = str(blockNumber + 1)
                mypainter.setPen(colors["white"])
                mypainter.drawText(0, int(top), int(self.lineNumberArea.width()), int(height),
                 Qt.AlignHCenter, number)

            block = block.next()
            top = bottom
            bottom = top + self.blockBoundingRect(block).height()
            blockNumber += 1


    def highlightCurrentLine(self):

        extraSelections = []

        if not self.isReadOnly():
            selection = QTextEdit.ExtraSelection()

            selection.format.setBackground(colors['dark-purple'])
            selection.format.setProperty(QTextFormat.FullWidthSelection, True)
            selection.cursor = self.textCursor()
            selection.cursor.clearSelection()
            extraSelections.append(selection)
        self.setExtraSelections(extraSelections)