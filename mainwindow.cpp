#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "codeanalyzer.h"
#include <QtDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    connect (ui->actionRun,SIGNAL (triggered()), this, SLOT (onRun()));
    connect (ui->actionExit, SIGNAL (triggered()), this, SLOT (exit()));
    connect (ui->actionAbout, SIGNAL(triggered()),this, SLOT (aboutme()));
    connect (ui->actionSave, SIGNAL (triggered()), this, SLOT (saveFile()));
    connect (ui->actionOpen, SIGNAL (triggered()), this, SLOT (openFile()));
    connect (ui->actionNew, SIGNAL (triggered()), this, SLOT (newFile()));
    connect (ui->actionSave_As, SIGNAL (triggered()), this, SLOT (saveAs()));
    connect (ui->plainTextEdit->document(), SIGNAL (contentsChanged()), this, SLOT (wasModified()));
    connect (ui->actionCut, SIGNAL(triggered()), ui->plainTextEdit, SLOT (cut()));
    connect (ui->actionCopy, SIGNAL(triggered()), ui->plainTextEdit, SLOT (copy()));
    connect (ui->actionPaste, SIGNAL(triggered()), ui->plainTextEdit, SLOT (paste()));
    connect(ui->actionRedo, SIGNAL (triggered()), ui->plainTextEdit, SLOT (redo()));
    connect (ui->actionUndo, SIGNAL (triggered()), ui->plainTextEdit, SLOT (undo()));
    connect (ui->actionFind, SIGNAL (triggered()), this, SLOT (findText()));

    setCentralWidget(ui->plainTextEdit);
    setCurrent("");

    //fill ToolBar
    ui->mainToolBar->addAction(ui->actionNew);
    ui->mainToolBar->addAction(ui->actionOpen);
    ui->mainToolBar->addAction(ui->actionSave);
    ui->mainToolBar->addAction(ui->actionSave_As);
    ui->mainToolBar->addAction(ui->actionExit);

    //create edit actions
createEditToolbar();
    ui->actionUndo->setEnabled(ui->plainTextEdit->document()->isUndoAvailable());
    ui->actionRedo->setEnabled(ui->plainTextEdit->document()->isRedoAvailable());
    ui->statusBar->showMessage(tr("QtQBasic started"));




}
QString MainWindow::strippedName(const QString &fileName) {
    return QFileInfo (fileName).fileName();
}
void MainWindow::setCurrent(const QString &fileName) {
    curFile = fileName;
    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);
    QString title = tr("QtQBasic - GIT - ");
    if (curFile.isEmpty())
        title.append(tr("noname.bas"));
    else
        title.append(strippedName(curFile));

    title.append("[*]");
    setWindowTitle(title);
update();
}
void MainWindow::wasModified() {
    setWindowModified(ui->plainTextEdit->document()->isModified());
    ui->actionUndo->setEnabled(ui->plainTextEdit->document()->isUndoAvailable());
    ui->actionRedo->setEnabled(ui->plainTextEdit->document()->isRedoAvailable());
}

bool MainWindow::saveFile() {
    if  (curFile.isEmpty()) {return saveAs();}
    else {return save(curFile);}
    }
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRun() {
    qDebug("qtqbasic: run program");
   CodeAnalyzer *c = new CodeAnalyzer(ui->plainTextEdit->toPlainText(), this);

}

void MainWindow::exit () {
    qApp->exit();
}
bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty()){return false;}
    return save (fileName);
}
bool MainWindow::save(const QString &fileName) {
    QFile file (fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QtQBasic"), tr ("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out (&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->plainTextEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    setCurrent(fileName);
    statusBar()->showMessage(tr("File saved: %1").arg(fileName), 3000);
    return true;
}
bool MainWindow::maybeSave() {
    if (ui->plainTextEdit->document()->isModified()) {
        ;
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("QtQBasic"), tr("Save changes in file?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {event->accept();}
    else {event->ignore();}
}
void MainWindow::newFile () {
    if (maybeSave()) {
        ui->plainTextEdit->clear();
        setCurrent("");
    }
}
void MainWindow::openFile() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            load (fileName);

    }

}
void MainWindow::aboutme() {
    about *a = new about (this);
    a->show();
}
void MainWindow::load(const QString &fileName) {
    QFile file (fileName);
       if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("QtQBasic"), tr ("Cannot open file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return;
    }
       QTextStream in (&file);
       QApplication::setOverrideCursor(Qt::WaitCursor);
       ui->plainTextEdit->setPlainText(in.readAll());
       QApplication::restoreOverrideCursor();
       setCurrent(fileName);    statusBar()->showMessage(tr("File loaded: %1").arg(fileName), 3000);


}

void MainWindow::createEditToolbar() {
editToolBar = addToolBar(tr ("Edit"));
editToolBar->addAction(ui->actionUndo);
editToolBar->addAction(ui->actionRedo);
editToolBar->addAction(ui->actionCut);
editToolBar->addAction(ui->actionCopy);
editToolBar->addAction(ui->actionPaste);
editToolBar->addAction(ui->actionFind);
}

void MainWindow::findText() {
  /*  Finder *f = new Finder (this);
    f->show();

if (f->result() == QDialog::Accepted)
    {
    QString findText = f->getFindText();
   // QTextDocument::FindFlag flags = f->getFlags();
    ui->plainTextEdit->document()->find(findText, 0, 0);
    */;
}



