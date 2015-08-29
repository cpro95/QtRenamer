#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QStringList>
#include <QStringListModel>
#include <QFileInfoList>
#include <QDropEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void closeEvent(QCloseEvent*);
    ~MainWindow();



private slots:
    void on_actionExit_triggered();

    void on_treeView_clicked(const QModelIndex &index);

    void on_subtitleDeleteButton_clicked();

    void on_movieDeleteButton_clicked();

    void on_movieUpButton_clicked();

    void on_subtitleUpButton_clicked();

    void on_movieDownButton_clicked();

    void on_subtitleDownButton_clicked();

    void on_actionAbout_triggered();

    void on_actionRenameByMovie_triggered();

    void on_actionRename_By_Subtitle_triggered();

    void on_renameMovieButton_clicked();

    void on_renameSubtitleButton_clicked();

private:

    void putMsg(QString msg);
    QString getNewName(int index);
    QString getNewName2(int index);
    void doRename();
    void doRename2();

    Ui::MainWindow *ui;
    QFileSystemModel *dirModel;

    QStringListModel *movieModel;
    QStringListModel *subtitleModel;

    QStringList movieList;
    QStringList subtitleList;

    QFileInfoList movieFileInfoList;
    QFileInfoList subtitleFileInfoList;

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

};

#endif // MAINWINDOW_H
