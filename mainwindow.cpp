#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QMimeData>
#include <QUrl>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    // for drop action
    setAcceptDrops(true);

    setWindowTitle("QtRenamer");


    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath("C:/");

    dirModel->setFilter(QDir::AllDirs |QDir::NoDotAndDotDot | QDir::NoSymLinks);
    dirModel->sort(0,Qt::AscendingOrder);

    ui->treeView->setModel(dirModel);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    movieModel = new QStringListModel(this);
    subtitleModel = new QStringListModel(this);


    restoreState(settings.value("mainWindowState").toByteArray());

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);


    QSettings settings;

    settings.setValue("mainWindowGeometry",saveGeometry());
    settings.setValue("mainWindowState",saveState());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QDir movieDir(dirModel->filePath(index));

    // movie
    movieFileInfoList.clear();
    movieFileInfoList = movieDir.entryInfoList(QStringList() << "*.mkv" << "*.avi" << "*.mp4", QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    movieList.clear();
    for(int i=0; i < movieFileInfoList.size();i++)
    {
        movieList << movieFileInfoList.at(i).fileName();
    }
    movieModel->setStringList(movieList);
    ui->listView->setModel(movieModel);


    // subtitle
    subtitleFileInfoList.clear();
    subtitleFileInfoList = movieDir.entryInfoList(QStringList() << "*.smi" << "*.smil" << "*.srt", QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    subtitleList.clear();
    for(int i=0; i < subtitleFileInfoList.size();i++)
    {
        subtitleList << subtitleFileInfoList.at(i).fileName();
    }
    subtitleModel->setStringList(subtitleList);
    ui->listView_2->setModel(subtitleModel);


    QString count = QString("동영상 : %1개     /     자막 : %2개").arg(movieList.length()).arg(subtitleList.length());

    putMsg(count);

}

void MainWindow::putMsg(QString msg)
{
    ui->lineEdit->setText(msg);
}

QString MainWindow::getNewName(int index)
{
    QFileInfo src = subtitleFileInfoList.at(index);

    QString smiExt;
    smiExt = src.suffix();

    QString smiNewName = movieFileInfoList.at(index).absolutePath();
    smiNewName += QString("%1").arg(QDir::separator());

    QString smiBaseName = movieFileInfoList.at(index).completeBaseName();

    smiNewName += smiBaseName;
    smiNewName += ".";
    smiNewName += smiExt;

//    qDebug() << smiNewName;
    return smiNewName;
}

QString MainWindow::getNewName2(int index)
{
    QFileInfo src = movieFileInfoList.at(index);

    QString movExt;
    movExt = src.suffix();

    QString movNewName = subtitleFileInfoList.at(index).absolutePath();
    movNewName += QString("%1").arg(QDir::separator());

    QString movBaseName = subtitleFileInfoList.at(index).completeBaseName();

    movNewName += movBaseName;
    movNewName += ".";
    movNewName += movExt;

//    qDebug() << movNewName;
    return movNewName;
}

void MainWindow::doRename() // rename subtitle file by movie
{


    for(int i=0;i<movieFileInfoList.size();i++)
    {
        QFile::rename(subtitleFileInfoList.at(i).filePath(),getNewName(i));
    }

    QMessageBox::warning(this,"Rename","renaming by movie has been done");
}

void MainWindow::doRename2() // rename movie file by subtitle
{

    for(int i=0;i<subtitleFileInfoList.size();i++)
    {
        QFile::rename(movieFileInfoList.at(i).filePath(),getNewName2(i));
    }

    QMessageBox::warning(this,"Rename","renaming by subtitle has been done");
}


// drop event
void MainWindow::dropEvent(QDropEvent *event)
{
//    const QMimeData *mimeData = event->mimeData();

//    if(mimeData->hasUrls())
//    {
//        putMsg("Urls");
//    }

    QList<QUrl> urls = event->mimeData()->urls();
        foreach(QUrl url, urls)
        {
            //qDebug()<<url.toString();
        }

//    QString path = path1.adjusted(QUrl::RemoveFilename).toString();
//    putMsg(path);


    // 첫번째 파일에서 경로를 얻는다.
    QFileInfo urlfile(urls.at(0).toLocalFile());
    putMsg(urlfile.path());

//    qDebug() << urlfile.absoluteFilePath();
//    qDebug() << urlfile.absolutePath();
//    qDebug() << urlfile.path();

    // 트리뷰의 currentIndex를 조정하고
    ui->treeView->scrollTo(dirModel->index(urlfile.path()));

    // 해당 treeview를 클릭한 것처럼 함수 호출
    on_treeView_clicked(dirModel->index(urlfile.path()));


}

// drop 만 원할때도 아래 함수 필요
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->accept();
}

void MainWindow::on_movieDeleteButton_clicked()
{
    // Movie ListView Delete clicked

    QModelIndex index = ui->listView->currentIndex();

    if(!index.isValid()) return;

    movieFileInfoList.removeAt(index.row());
    movieList.removeAt(index.row());
    movieModel->setStringList(movieList);
}

void MainWindow::on_subtitleDeleteButton_clicked()
{
    // Subtitle ListView Delete clicked

    QModelIndex index = ui->listView_2->currentIndex();

    if(!index.isValid()) return;


    subtitleFileInfoList.removeAt(index.row());
    subtitleList.removeAt(index.row());
    subtitleModel->setStringList(subtitleList);
}

void MainWindow::on_movieUpButton_clicked()
{
    // movie ListView UP Clicked

    QModelIndex index = ui->listView->currentIndex();

    if(!index.isValid()) return;

    if(index.row() == 0)
    {
        putMsg("It's in the Top of the List");
        return;
    }
    else
    {
        movieFileInfoList.swap(index.row(), index.row()-1);
        movieList.swap(index.row(), index.row()-1);

        putMsg("swap has been done");
        movieModel->setStringList(movieList);
    }

}

void MainWindow::on_subtitleUpButton_clicked()
{
    // subTitle ListView UP Clicked

    QModelIndex index = ui->listView_2->currentIndex();

    if(!index.isValid()) return;

    if(index.row() == 0)
    {
        putMsg("It's in the Top of the List");
        return;
    }
    else
    {
        subtitleFileInfoList.swap(index.row(), index.row()-1);
        subtitleList.swap(index.row(), index.row()-1);

        putMsg("swap has been done");
        subtitleModel->setStringList(subtitleList);
    }
}

void MainWindow::on_movieDownButton_clicked()
{
    // movie Listview DOWN clicked

    QModelIndex index = ui->listView->currentIndex();

    if(!index.isValid()) return;

    if(index.row() == (movieList.size()-1))
    {
        putMsg("It's in the Bottom of the List");
        return;
    }
    else
    {
        movieFileInfoList.swap(index.row(), index.row()+1);
        movieList.swap(index.row(), index.row()+1);

        putMsg("swap has been done");
        movieModel->setStringList(movieList);
    }
}

void MainWindow::on_subtitleDownButton_clicked()
{
    // subtitle Listview DOWN clicked

    QModelIndex index = ui->listView_2->currentIndex();

    if(!index.isValid()) return;

    if(index.row() == (subtitleList.size()-1))
    {
        putMsg("It's in the Bottom of the List");
        return;
    }
    else
    {
        subtitleFileInfoList.swap(index.row(), index.row()+1);
        subtitleList.swap(index.row(), index.row()+1);

        putMsg("swap has been done");
        subtitleModel->setStringList(subtitleList);
    }
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"About...",
                             "Hello.\n"
                             "This program is for helping rename files more easily\n"
                             "especially with movies and subtitles\n"
                             "This program is made by cpro95@gmail.com\n"
                             "if you have any questions, feel free to contact me.\n"
                             "bye", QMessageBox::Ok);
}

void MainWindow::on_actionRenameByMovie_triggered()
{
    // rename trigger slot


    if(movieFileInfoList.size() == 0 || subtitleFileInfoList.size() ==0)
    {
        QMessageBox::warning(this,"Error","No files exist");
    }
    else if(movieFileInfoList.size() != subtitleFileInfoList.size())
    {
        QMessageBox::warning(this,"Error",QString("Movie : %1 vs Subtitle : %2").arg(QString::number(movieFileInfoList.size()),QString::number(subtitleFileInfoList.size())));
    }
    else
    {
        int ret = QMessageBox::question(this,"Are you Sure for rename?","Are you Sure for rename?",QMessageBox::Yes,QMessageBox::No);

        switch(ret)
        {
        case QMessageBox::Yes :
            doRename();
            break;
        case QMessageBox::No :

            break;
        default: break;
        }
    }


    // after renaming, treeView must do refresh(update)
    QModelIndex index = ui->treeView->currentIndex();
    on_treeView_clicked(index);
}

void MainWindow::on_actionRename_By_Subtitle_triggered()
{
    // rename trigger slot


    if(movieFileInfoList.size() == 0 || subtitleFileInfoList.size() ==0)
    {
        QMessageBox::warning(this,"Error","No files exist");
    }
    else if(movieFileInfoList.size() != subtitleFileInfoList.size())
    {
        QMessageBox::warning(this,"Error",QString("Movie : %1 vs Subtitle : %2").arg(QString::number(movieFileInfoList.size()),QString::number(subtitleFileInfoList.size())));
    }
    else
    {
        int ret = QMessageBox::question(this,"Are you Sure for rename?","Are you Sure for rename?",QMessageBox::Yes,QMessageBox::No);

        switch(ret)
        {
        case QMessageBox::Yes :
            doRename2();
            break;
        case QMessageBox::No :

            break;
        default: break;
        }
    }

    // after renaming, treeView must do refresh(update)
    QModelIndex index = ui->treeView->currentIndex();
    on_treeView_clicked(index);

}

void MainWindow::on_renameMovieButton_clicked()
{
    on_actionRenameByMovie_triggered();
}

void MainWindow::on_renameSubtitleButton_clicked()
{
    on_actionRename_By_Subtitle_triggered();
}
