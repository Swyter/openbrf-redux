/* OpenBRF -- by marco tarini. Provided under GNU General Public License */


#include "tablemodel.h"
#include <QColor>
#include <QApplication>
#include <QPalette>
#include <QFont>

MyTableModel::MyTableModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void MyTableModel::updateChanges(){
  int t=vec.size();
  emit(this->dataChanged(createIndex(0,0),createIndex(1,t+100)));
  emit(layoutChanged());
}

void MyTableModel::clear()
{
  int t=vec.size();
  vec.clear();
  vecUsed.clear();
  this->dataChanged(createIndex(0,0),createIndex(1,t));
}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return vec.size();
}

int MyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

Qt::DropActions MyTableModel::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
  static QFont alternate; QPalette palette = QApplication::palette();
  static bool firstTime=true; bool isDarkMode = palette.color(QPalette::WindowText).lightness() > palette.color(QPalette::Window).lightness();
  if (firstTime) {
    alternate= QApplication::font();
    //alternate.setItalic(!alternate.italic());
    alternate.setBold(true);
    firstTime = false;
  }
  int i = index.row();
  if (i>=(int)vecUsed.size()) i = vecUsed.size()-1;
  if (i<0) {
    return QVariant();
  }
  if (role==Qt::DisplayRole)
    return vec[ i ];
  if (role==Qt::FontRole){
    return (vecUsed[ i ]!=1)?QApplication::font():alternate;
  }
  if (role==Qt::BackgroundRole) return palette.color (QPalette::Base); /* swy: use the default background color for list entries, this was originally QColor(255,255,255,255), always pure white */
   // //return (index.row()%2==0)?QColor(128,128,128,255):QColor(0,0,0,255);
   // return (vecUsed[ index.row() ]!=0)?
    //    QApplication::palette().color (QPalette::Base):
    //    QApplication::palette().color (QPalette::AlternateBase);
  if (role==Qt::ForegroundRole) {
    switch(vecUsed[ i ]){
    case  1 /* swy: [used in mod   ] e.g. eventually used by some asset chaining back to a .txt file    */: { return isDarkMode ? QColor(207, 207, 252, 255) /* swy: light blue  */ : QColor(  0,   0, 150, 255) /* swy: dark blue  */;                          } 
    case  0 /* swy: [no usage check] normal/neutral list text color before using the check function     */: { return palette.brush(QPalette::Active, QPalette::WindowText); /* swy: normal list color, use the default (usually black-on-white in light mode) */ } 
    case -1 /* swy: [used by asset ] e.g. material used by some (unused) mesh that the mod doesn't load */: { return isDarkMode ? QColor(156, 209, 156, 255) /* swy: muted green */ : QColor( 40,  50,  40, 255) /* swy: dark green */;                          }
    case -2 /* swy: [unused        ] not used by anything, not even other unused stuff                  */: { return isDarkMode ? QColor(163, 163, 163, 255) /* swy: light gray  */ : QColor(140, 150, 140, 255) /* swy: dark gray  */;                          } 
    //return QApplication::palette().color(QPalette::Text);
    }
  }
  return QVariant();
}

QVariant MyTableModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{
  return tr("HEADER");

  //  return QVariant();
}




