# -*- coding: utf-8 -*-
import os
import sys
import subprocess

from subprocess import Popen, PIPE, STDOUT

from PyQt5 import QtWidgets, QtCore
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import Qt, QTimer, QPoint

import syntax

from overlay import Overlay
from code_editor import CodeEditor
from colors import colors

import os.path


class CppCompiler(QWidget):

    TITLE = "CppCompiler"
    BACKGROUND_COLOR = colors["grey"]

    def __init__(self, args):
        super(QWidget, self).__init__()

        # Compiler path
        self.cpp_compiler_path = "CppCompiler.exe" if sys.platform == "win32" else "CppCompiler"
        self.cpp_compiler_path = os.path.join(os.path.dirname(__file__), self.cpp_compiler_path)
        if sys.platform != "win32":
            os.chmod(self.cpp_compiler_path, 0o777)

        self.cpp_quadruples_path = "out.txt"
        self.cpp_symbols_table_path = "symbols.txt"
        self.cpp_tmp_path = "CppCompilerTmp.txt"
        self.file_path = ""

        self.left = 60
        self.top = 30
        self.width = int(self.getWindowSize()[1])
        self.height = int(self.getWindowSize()[0])

        # Animation
        self.compilingTask = TaskThread()
        self.compilingTask.task_finished.connect(self.on_compilation_finished)

        self.init_gui()

    def init_gui(self):
        self.setup_window()
        self.setup_window_title()
        self.setup_window_exit_btn()
        self.setup_code_editor()
        self.setup_quadruples_view()
        self.setup_symbol_table_view()
        self.setup_logs_view()
        self.setup_switch_btn()
        self.setup_compile_btn()
        self.setup_select_file_btn()
        self.setup_loading_animation()
        self.show()

    def setup_window(self):
        self.setWindowTitle(CppCompiler.TITLE)
        self.setGeometry(int(self.left), int(self.top), int(self.width), int(self.height))

        # Set window background color
        self.setAutoFillBackground(True)
        p = self.palette()
        p.setColor(self.backgroundRole(), CppCompiler.BACKGROUND_COLOR)
        self.setPalette(p)

        self.setWindowFlags(QtCore.Qt.FramelessWindowHint)

    def setup_loading_animation(self):
        self.overlay = Overlay(self)
        self.overlay.hide()

    def setup_window_title(self):
        self.title = QLabel(self)
        self.title.setText(CppCompiler.TITLE)
        self.title.setStyleSheet("font-size: 28px; padding-bottom: 6px; font-weight: bold; color: #ccc; font-family: Courier New;")
        self.title.setGeometry(QtCore.QRect(20, 20, int(self.width - 40), 55))
        self.title.setAlignment(QtCore.Qt.AlignLeft | QtCore.Qt.AlignVCenter)

    def setup_window_exit_btn(self):
        self.windowExitBtn = QPushButton('×', parent=self)
        self.windowExitBtn.move(int(self.width - 55), 28)
        self.windowExitBtn.setCursor(Qt.PointingHandCursor)
        self.windowExitBtn.setStyleSheet(
            "border-radius: 0px; font-family: Courier New; font-size: 48px; color: #ffffff; background: #00ffffff; text-decoration: none;")

        self.windowExitBtn.clicked.connect(app.quit)

    def setup_code_editor(self):
        font = self.getFont()
        self.input_code_editor = CodeEditor(self)
        self.input_code_editor.setFont(font)
        self.input_code_editor.setGeometry(QtCore.QRect(
            20, 100, int(self.width / 2 - 10), int(self.height * 5.5 / 9)))
        self.input_code_editor.setGraphicsEffect(self.getShadow())
        self.input_code_editor.show()

        self.highlight = syntax.CPPHighlighter(self.input_code_editor.document())

    def setup_quadruples_view(self):
        font = self.getFont()
        self.quadruples_view = CodeEditor(self)
        self.quadruples_view.setFont(font)
        self.quadruples_view.setGeometry(QtCore.QRect(
            int(self.width / 2 + 40), 100, int(self.width / 2 - 59), int(self.height * 5.5 / 9)))
        self.quadruples_view.setGraphicsEffect(self.getShadow())
        self.quadruples_view.setReadOnly(True)
        self.quadruples_view.show()

    def setup_symbol_table_view(self):
        font = self.getFont()
        self.symbols_view = CodeEditor(self)
        self.symbols_view.setFont(font)
        self.symbols_view.setGeometry(QtCore.QRect(
            int(self.width / 2 + 40), 100, int(self.width / 2 - 59), int(self.height * 5.5 / 9)))
        self.symbols_view.setGraphicsEffect(self.getShadow())
        self.symbols_view.setReadOnly(True)
        self.symbols_view.hide()

    def setup_logs_view(self):
        font = self.getFont()
        self.logs_view = CodeEditor(self)
        self.logs_view.setFont(font)
        self.logs_view.setGeometry(QtCore.QRect(
            20, int(self.height * 7 / 9 + 20), self.width - 37, 167))
        self.logs_view.setGraphicsEffect(self.getShadow())
        self.logs_view.setReadOnly(True)
        self.logs_view.show()

    def setup_switch_btn(self):
        self.select_symbol_table_btn = HoverButton(
            'Symbols', color="#000000", BKColor="#e0e0e0", hoverBKColor="#c8e0e0e0", parent=self)
        self.select_symbol_table_btn.move(
            self.width - self.select_symbol_table_btn.width() - 57, self.height - 278)
        self.select_symbol_table_btn.setCursor(Qt.PointingHandCursor)
        self.select_symbol_table_btn.setStyleSheet(
            "border-radius: 0px;  font-family: Courier New; color: #000000; font-size: 18px; background: #e0e0e0; padding: 8px 18px 8px 18px; text-decoration: none;")

        self.select_quadruples_btn = HoverButton(
            'Quadruples', color="#ffffff", BKColor="#50ae55", hoverBKColor="#c850ae55", parent=self)
        self.select_quadruples_btn.move(self.width - self.select_symbol_table_btn.width(
        ) - self.select_quadruples_btn.width() - 126, self.height - 278)
        self.select_quadruples_btn.setCursor(Qt.PointingHandCursor)
        self.select_quadruples_btn.setStyleSheet(
            "border-radius: 0px; font-family: Courier New; color: #ffffff; font-size: 18px; background: #50ae55; padding: 8px 18px 8px 18px; text-decoration: none;")

        self.select_quadruples_btn.clicked.connect(self.select_quadruples)
        self.select_symbol_table_btn.clicked.connect(self.select_symbol_table)

    def setup_select_file_btn(self):
        self.select_file_btn = HoverButton('Select', parent=self)
        self.select_file_btn.move(
            self.width - self.select_file_btn.width() - self.compile_btn.width() - 1508, self.height - 278)
        self.select_file_btn.setCursor(Qt.PointingHandCursor)
        self.select_file_btn.setStyleSheet(
            "border-radius: 0px; font-family: Courier New; color: #ffffff; font-size: 18px; background: #3498db; padding: 8px 25px 8px 25px; text-decoration: none;")

        self.select_file_btn.clicked.connect(self.select_file)

    def setup_compile_btn(self):
        self.compile_btn = HoverButton('Compile', parent=self)
        self.compile_btn.move(
            (self.width - self.compile_btn.width() - 896), self.height - 278)
        self.compile_btn.setCursor(Qt.PointingHandCursor)
        self.compile_btn.setStyleSheet(
            "border-radius: 0px;  font-family: Courier New; color: #ffffff; font-size: 18px; background: #3498db; padding: 8px 20px 8px 20px; text-decoration: none;")

        self.compile_btn.clicked.connect(self.on_compilation_started)

    def getWindowSize(self):
        size = QtWidgets.QDesktopWidget().screenGeometry(-1)
        return (size.height() * 0.9, size.width() * 0.9)

    def getFont(self):
        font = QFont()
        font.setFamily('Courier New')
        font.setFixedPitch(True)
        font.setPointSize(12)

        return font

    def getShadow(self):
        shadow = QGraphicsDropShadowEffect(self)
        shadow.setBlurRadius(2)
        shadow.setOffset(0, 0)

        return shadow

    def select_quadruples(self):
        self.quadruples_view.show()
        self.symbols_view.hide()
        self.switch_btns(self.select_quadruples_btn,
                         self.select_symbol_table_btn)

    def select_symbol_table(self):
        self.symbols_view.show()
        self.quadruples_view.hide()
        self.switch_btns(self.select_symbol_table_btn,
                         self.select_quadruples_btn)

    def switch_btns(self, active, inactive):
        active.color = "#ffffff"
        active.BKColor = "#50ae55"
        active.hoverBKColor = "#c850ae55"
        active.setStyleSheet(
            "border-radius: 0px; font-family: Courier New; color: #ffffff; font-size: 18px; background: #50ae55; padding: 8px 18px 8px 18px; text-decoration: none;")

        inactive.color = "#000000"
        inactive.BKColor = "#e0e0e0"
        inactive.hoverBKColor = "#c8e0e0e0"
        inactive.setStyleSheet(
            "border-radius: 0px;  font-family: Courier New; color: #000000; font-size: 18px; background: #e0e0e0; padding: 8px 18px 8px 18px; text-decoration: none;")

    def select_file(self, args=None):
        path = QFileDialog.getOpenFileName()[0]

        if (len(path.strip()) != 0):
            self.file_path = path
            with open(path, 'r') as content_file:
                content = content_file.read()

                self.input_code_editor.clear()
                self.quadruples_view.clear()
                self.logs_view.clear()
                self.symbols_view.clear()

                self.input_code_editor.insertPlainText(content)

    def on_compilation_started(self):
        if (len(self.input_code_editor.toPlainText()) == 0):
            return

        self.overlay.show()
        self.compilingTask.start()

    def on_compilation_finished(self):
        self.overlay.hide()

    def compile(self):
        print("Compiling")

        # Empty the output files
        open("out.txt", 'w').close()
        open("symbols.txt", 'w').close()


        with open(self.cpp_tmp_path, 'w') as f:
            f.write(self.input_code_editor.toPlainText())

        # Compile
        process = subprocess.run([self.cpp_compiler_path, self.cpp_tmp_path], text=True, capture_output=True)

        self.logs_view.clear()

        self.logs_view.setCurrentCharFormat(QTextCharFormat())
        self.logs_view.insertPlainText(process.stdout)

        errors_format = QTextCharFormat()
        errors_format.setForeground(Qt.red)
        errors_format.setFontWeight(QFont.Bold)
        self.logs_view.setCurrentCharFormat(errors_format)
        self.logs_view.insertPlainText(process.stderr)

        format = QTextCharFormat()
        format.setForeground(Qt.green) if process.returncode == 0 else format.setForeground(Qt.red)
        self.logs_view.setCurrentCharFormat(format)

        self.logs_view.insertPlainText(f"\n\nCompilation finished with exit code {process.returncode}\n")

        if not os.path.isfile(self.cpp_quadruples_path):
            return

        with open(self.cpp_quadruples_path, 'r') as f:
            content = f.read()

            self.quadruples_view.clear()
            self.quadruples_view.insertPlainText(content)

        if os.path.isfile(self.cpp_symbols_table_path):
            with open(self.cpp_symbols_table_path, 'r') as f:
                content = f.read()

                self.symbols_view.clear()
                self.symbols_view.insertPlainText(content)

    def resizeEvent(self, event):
        self.overlay.resize(event.size())
        event.accept()

    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())

    def mousePressEvent(self, event):
        self.oldPos = event.globalPos()

    def mouseMoveEvent(self, event):
        if (hasattr(self, 'oldPos')):
            delta = QPoint(event.globalPos() - self.oldPos)
            self.move(self.x() + delta.x(), self.y() + delta.y())
            self.oldPos = event.globalPos()


class HoverButton(QToolButton):

    def __init__(self, text, BKColor="#3498db", hoverBKColor="#3cb0fd", color="#ffffff", padding="8px 18px 8px 18px",
                 fontSize="18px", parent=None):
        super(HoverButton, self).__init__(parent)

        self.setMouseTracking(True)
        self.setText(text)
        self.BKColor = BKColor
        self.hoverBKColor = hoverBKColor
        self.color = color
        self.fontSize = fontSize
        self.padding = padding

    def enterEvent(self, event):
        self.setStyleSheet("border-radius: 0px;  font-family: Courier New; color: " + self.color + "; font-size: " +
                           self.fontSize + "; background: " + self.hoverBKColor + "; padding: " + self.padding + "; text-decoration: none;")

    def leaveEvent(self, event):
        self.setStyleSheet("border-radius: 0px;  font-family: Courier New; color: " + self.color + "; font-size: " +
                           self.fontSize + "; background: " + self.BKColor + "; padding: " + self.padding + "; text-decoration: none;")


class TaskThread(QtCore.QThread):

    task_finished = QtCore.pyqtSignal()

    def run(self):
        cpp.compile()

        self.task_finished.emit()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    cpp = CppCompiler(sys.argv)
    sys.exit(app.exec_())
