qt6 = import('qt6')
qt6_dep = dependency('qt6', modules: ['Core', 'Gui', 'Widgets'])

sudoku_qt_sources = [
  'main.cpp', 'gridgen.cpp', 'mainwindow.cpp'
]

sudoku_qt_deps = [
  qt6_dep
]

sudoku_qt_sources += qt6.preprocess(moc_headers: 'mainwindow.h',
                             ui_files: 'mainwindow.ui')

executable('sudoku-qt', sudoku_qt_sources,
  dependencies: sudoku_qt_deps,
  install: true,
)

