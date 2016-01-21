#include "negativefoldersparser.h"

NegativeFoldersParser::NegativeFoldersParser(QUrl rootFolder)
{
    mRootFolder = rootFolder;
}


pr::folder_vector *NegativeFoldersParser::getFolders()
{
    QDir dir(mRootFolder.path());

    if(dir.exists()) {

//        QStringList all_dirs;
//        QDirIterator directories(mRootFolder.path().toStdString(), QDir::Dirs | QDir::NoSymLinks
//                                 | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

//        while(directories.hasNext()){
//            directories.next();
//            all_dirs << directories.filePath();
//            qDebug() << "Adding "  << directories.filePath();
//        }



    } else {
        return nullptr;
    }

    return nullptr;
}
