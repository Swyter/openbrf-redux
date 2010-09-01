

#include <QtGui>
#include "brfdata.h"
#include "selector.h"

#include "tablemodel.h"



void Selector::addToRefMeshA(){ emit(addToRefMesh(0)); }
void Selector::addToRefMeshB(){ emit(addToRefMesh(1)); }
void Selector::addToRefMeshC(){ emit(addToRefMesh(2)); }
void Selector::addToRefMeshD(){ emit(addToRefMesh(3)); }
void Selector::addToRefMeshE(){ emit(addToRefMesh(4)); }
void Selector::addToRefMeshF(){ emit(addToRefMesh(5)); }
void Selector::addToRefMeshG(){ emit(addToRefMesh(6)); }
void Selector::addToRefMeshH(){ emit(addToRefMesh(7)); }
void Selector::addToRefMeshI(){ emit(addToRefMesh(8)); }
void Selector::addToRefMeshJ(){ emit(addToRefMesh(9)); }

Selector::Selector(QWidget *parent)
   : QTabWidget(parent)
{
  for (int i=0; i<N_TOKEN; i++) {
    tab[i]=NULL;
    //tabIndex[i]=-1;
  }
  connect(this, SIGNAL(currentChanged(int)),
          this, SLOT(onChanged()) );

  breakAniWithIniAct = new QAction(tr("Split via action.txt"), this);
  breakAniWithIniAct ->setStatusTip(tr("Split sequence following the action.txt file. A new \"action [after split].txt\" file is also produced, which use the new animation."));

  breakAniAct = new QAction(tr("Auto-split"), this);
  breakAniAct->setStatusTip(tr("Auto-split sequence into its separated chunks, separating it at lasge gaps in frames."));

  renameAct = new QAction(tr("Rename"), this);
  renameAct->setShortcut(QString("F2"));
  addAction(renameAct);

  removeAct = new QAction(tr("Remove"), this);
  removeAct->setShortcut(QString("delete"));
  addAction(removeAct);

  duplicateAct = new QAction(tr("Duplicate"), this);

  goNextTabAct = new QAction(tr("next tab"),this);
  goNextTabAct->setShortcut(QString("right"));
  addAction(goNextTabAct);

  goPrevTabAct = new QAction(tr("left tab"),this);
  goPrevTabAct->setShortcut(QString("left"));
  addAction(goPrevTabAct);

  moveUpAct = new QAction(tr("Move up"), this);
  moveUpAct->setShortcut(QString("Alt+up"));
  moveUpAct->setStatusTip("Move this object upward in the list");
  addAction(moveUpAct);

  moveDownAct = new QAction(tr("Move down"), this);
  moveDownAct->setShortcut(QString("Alt+down"));
  moveDownAct->setStatusTip("Move this object one step down in the list");
  addAction(moveDownAct);

  addToRefAnimAct = new QAction(tr("Add to reference animations"), this);
  addToRefAnimAct->setStatusTip(tr("Add this animation to reference animations (to use it later to display rigged meshes)."));

  addToRefSkelAct = new QAction(tr("Add to reference skeletons"), this);
  addToRefSkelAct->setStatusTip(tr("Add this animation to reference skeletons (to use it later for animations)."));

  for (int i=0; i<10; i++) {
    this->addToRefMeshAct[i]= new QAction(tr("set %1").arg(char('A'+i)), this);
    addToRefMeshAct[i]->setStatusTip(tr("Add this mesh to reference skins (to use it later to display animations)."));

  }
  connect(addToRefMeshAct[0], SIGNAL(triggered()), this, SLOT(addToRefMeshA()));
  connect(addToRefMeshAct[1], SIGNAL(triggered()), this, SLOT(addToRefMeshB()));
  connect(addToRefMeshAct[2], SIGNAL(triggered()), this, SLOT(addToRefMeshC()));
  connect(addToRefMeshAct[3], SIGNAL(triggered()), this, SLOT(addToRefMeshD()));
  connect(addToRefMeshAct[4], SIGNAL(triggered()), this, SLOT(addToRefMeshE()));
  connect(addToRefMeshAct[5], SIGNAL(triggered()), this, SLOT(addToRefMeshF()));
  connect(addToRefMeshAct[6], SIGNAL(triggered()), this, SLOT(addToRefMeshG()));
  connect(addToRefMeshAct[7], SIGNAL(triggered()), this, SLOT(addToRefMeshH()));
  connect(addToRefMeshAct[8], SIGNAL(triggered()), this, SLOT(addToRefMeshI()));
  connect(addToRefMeshAct[9], SIGNAL(triggered()), this, SLOT(addToRefMeshJ()));
  connect(this, SIGNAL(addToRefMesh(int)), parent, SLOT(addToRefMesh(int)));

  exportBodyAct = new QAction(tr("Export"), this);

  exportImportMeshInfoAct = new QAction(tr("Info on mesh import/export"), this);

  exportStaticMeshAct = new QAction(tr("Export static mesh"), this);
  exportStaticMeshAct->setStatusTip(tr("Export this model (or this frame) as a 3D static mesh."));

  exportMovingMeshAct = new QAction(tr("Export vertex ani"), this);
  exportMovingMeshAct->setStatusTip(tr("Export this model as a mesh with vertex animation."));

  exportMeshGroupAct = new QAction(tr("Export combined mesh"), this);
  exportMeshGroupAct->setStatusTip(tr("Export this group of models in a single OBJ."));
  exportMeshGroupManyFilesAct = new QAction(tr("Export all meshes"), this);
  exportMeshGroupManyFilesAct->setStatusTip(tr("Export each of these models as separate OBJs."));

  exportRiggedMeshAct = new QAction(tr("Export rigged mesh"), this);
  exportRiggedMeshAct->setStatusTip(tr("Export this model (or this frame) as a rigged mesh."));

  exportSkeletonAct = new QAction(tr("Export (nude) skeleton"), this);
  exportSkeletonAct ->setStatusTip(tr("Export this skeleton (as a set of nude bones)."));
  exportSkinAct = new QAction(tr("Export skeleton with skin"), this);
  exportSkinAct->setStatusTip(tr("Export this skeleton (as a rigged skin)."));
  exportSkinForAnimationAct     = new QAction(tr("Export a skin for this ani"), this);
  exportSkinForAnimationAct->setStatusTip(tr("Export a rigged skin which can be used for this animation."));

  exportAnimationAct = new QAction(tr("Export animation"), this);
  exportAnimationAct->setStatusTip(tr("Export this animation."));

  reskeletonizeAct = new QAction(tr("Reskeletonize"), this);
  reskeletonizeAct->setStatusTip(tr("Adapt this rigged mesh to a new skeleton"));

  transferRiggingAct = new QAction(tr("Transfer rigging"), this);
  transferRiggingAct->setStatusTip(tr("Copy rigging from one mesh to another"));


  flipAct = new QAction(tr("Mirror"), this);
  flipAct->setStatusTip(tr("Mirror this object on the X axis."));

  transformAct = new QAction(tr("Roto-translate-rescale"),this);
  flipAct->setStatusTip(tr("Apply a gemoetric transform."));

  scaleAct = new QAction(tr("Rescale"), this);
  scaleAct->setStatusTip(tr("Rescale this object."));

  shiftAniAct = new QAction(tr("Shift time interval"), this);
  shiftAniAct->setStatusTip(tr("Shift a time interval for this animation"));

  meshRecomputeNormalsAndUnify = new QAction(tr("Recompute normals"), this);
  meshRecomputeNormalsAndUnify->setStatusTip(tr("Recompute normals for this model, and unify pos and vertices"));

  meshUnify = new QAction(tr("Unify vertices"), this);
  meshUnify->setStatusTip(tr("Unify identical vertices and pos."));

  meshMerge = new QAction(tr("Combine meshes"), this);
  meshMerge->setStatusTip(tr("Make a combined mesh unifying these meshes."));

  meshMountOnBone = new QAction(tr("Mount on one bone"), this);
  meshMountOnBone->setStatusTip(tr("Put this mesh on top of a single skeleton bone."));

  meshRemoveBackfacing = new QAction(tr("remove all"), this);
  meshRemoveBackfacing->setStatusTip(tr("Remove all faces that are backfacing (e.g. in beard meshes)."));
  meshAddBackfacing = new QAction(tr("add (x2 faces)"), this);
  meshAddBackfacing->setStatusTip(tr("Duplicate all faces: for each current face, add a backfacing face."));


  discardColAct = new QAction(tr("per-vertex color"), this);
  discardRigAct = new QAction(tr("rigging"), this);
  discardAniAct = new QAction(tr("vertex animation"), this);
  discardAniAct->setStatusTip(tr("Discard vertex animation (keeps only current frame)"));
  //exportAnyBrfAct = new QAction(tr("in a BRF"), this);
  //exportAnyBrfAct->setStatusTip(tr("Export this object in a BRF file."));

  exportSkeletonModAct = new QAction(tr("Make a skeleton-modification mesh"), this);
  importSkeletonModAct = new QAction(tr("Modify from a skeleton-modification mesh"), this);

  connect(goNextTabAct, SIGNAL(triggered()),this,SLOT(goNextTab()));
  connect(goPrevTabAct, SIGNAL(triggered()),this,SLOT(goPrevTab()));


  connect(breakAniAct, SIGNAL(triggered()),this,SLOT(onBreakAni()));
  connect(breakAniWithIniAct, SIGNAL(triggered()),this,SLOT(onBreakAniWithIni()));
  connect(meshRecomputeNormalsAndUnify,  SIGNAL(triggered()),parent,SLOT(meshRecomputeNormalsAndUnify()));
  connect(meshUnify,  SIGNAL(triggered()),parent,SLOT(meshUnify()));
  connect(meshMerge,  SIGNAL(triggered()),parent,SLOT(meshMerge()));
  connect(meshMountOnBone,SIGNAL(triggered()),parent,SLOT(meshMountOnBone()));
  connect(meshRemoveBackfacing,SIGNAL(triggered()),parent,SLOT(meshRemoveBack()));
  connect(meshAddBackfacing,SIGNAL(triggered()),parent,SLOT(meshAddBack()));


  //connect(exportAnyBrfAct, SIGNAL(triggered()),parent,SLOT(exportBrf()));
  connect(exportStaticMeshAct, SIGNAL(triggered()),parent,SLOT(exportStaticMesh()));
  connect(exportRiggedMeshAct, SIGNAL(triggered()),parent,SLOT(exportRiggedMesh()));
  connect(exportMovingMeshAct, SIGNAL(triggered()),parent,SLOT(exportMovingMesh()));
  connect(exportMeshGroupAct, SIGNAL(triggered()),parent,SLOT(exportMeshGroup()));
  connect(exportMeshGroupManyFilesAct, SIGNAL(triggered()),parent,SLOT(exportMeshGroupManyFiles()));
  connect(exportSkeletonModAct, SIGNAL(triggered()),parent,SLOT(exportSkeletonMod()));
  connect(exportSkeletonAct, SIGNAL(triggered()),parent,SLOT(exportSkeleton()));
  connect(exportAnimationAct, SIGNAL(triggered()),parent,SLOT(exportAnimation()));
  connect(reskeletonizeAct, SIGNAL(triggered()),parent,SLOT(reskeletonize()));
  connect(transferRiggingAct, SIGNAL(triggered()),parent,SLOT(transferRigging()));

  connect(flipAct, SIGNAL(triggered()),parent,SLOT(flip()));
  connect(transformAct, SIGNAL(triggered()),parent,SLOT(transform()));
  connect(scaleAct, SIGNAL(triggered()),parent,SLOT(scale()));
  connect(exportBodyAct, SIGNAL(triggered()), parent, SLOT(exportCollisionBody()));
  connect(shiftAniAct, SIGNAL(triggered()),parent,SLOT(shiftAni()));

  connect(discardAniAct,SIGNAL(triggered()),parent,SLOT(meshDiscardAni()));
  connect(discardColAct,SIGNAL(triggered()),parent,SLOT(meshDiscardCol()));
  connect(discardRigAct,SIGNAL(triggered()),parent,SLOT(meshDiscardRig()));

  connect(exportSkinAct, SIGNAL(triggered()), parent, SLOT(exportSkeletonAndSkin()));
  connect(exportSkinForAnimationAct, SIGNAL(triggered()), parent, SLOT(exportSkeletonAndSkin()));

  connect(importSkeletonModAct, SIGNAL(triggered()),parent,SLOT(importSkeletonMod()));

  connect(moveUpAct, SIGNAL(triggered()), parent, SLOT(moveUpSel()));
  connect(moveDownAct, SIGNAL(triggered()), parent, SLOT(moveDownSel()));
  connect(removeAct, SIGNAL(triggered()), parent, SLOT(deleteSel()));
  connect(duplicateAct, SIGNAL(triggered()), parent, SLOT(duplicateSel()));
  //
  connect(renameAct, SIGNAL(triggered()), parent, SLOT(renameSel()));


  connect(addToRefAnimAct, SIGNAL(triggered()), parent, SLOT(addToRef()));
  connect(addToRefSkelAct, SIGNAL(triggered()), parent, SLOT(addToRef()));
  this->setMinimumWidth(200);

  // prepare all tabs (w/o attaching them for now)
  for (int ti=0; ti<N_TOKEN; ti++) {

    tab[ti] =// new QListWidget;
    new QListView;

    tableModel[ti] = new TableModel(this);

    tab[ti]->setModel(tableModel[ti]);

    if (ti==MESH || ti==MATERIAL || ti==TEXTURE || ti==SKELETON ) {
      tab[ti]->setSelectionMode(QAbstractItemView::ExtendedSelection);
      tab[ti]->setStatusTip(QString("[Right-Click]: tools for %1. Multiple selections with [Shift]-[Ctrl].").arg(tokenFullName[ti]));
    }
    else
      tab[ti]->setStatusTip(QString("[Right-Click]: tools for %1.").arg(tokenFullName[ti]));

    connect(tab[ti]->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SLOT(onChanged()) );

  }
}

/*
void Selector::onRenameSel(){
  QListView* tab = (QListView*)currentWidget();
  if (!tab) return;
  QModelIndexList li = tab->selectionModel()->selectedIndexes();
  if (li.size()!=1) return;
  QModelIndex i = li[0];
  //tab->e
  tab->edit(i);
}
*/


int Selector::firstSelected() const{
  if (!this->currentWidget()) return -1;
  unsigned int s =
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes().size();
  if (s==0) return -1;
  return
      (((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes())[0].row();
}

int Selector::lastSelected() const{
  if (!this->currentWidget()) return -1;
  unsigned int s =
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes().size();
  if (s==0) return -1;
  return
      (((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes())[s-1].row();
}

QModelIndexList Selector::selectedList() const{
  if (!this->currentWidget()) return QModelIndexList();
  return
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes();
}

int Selector::onlySelected(int kind) const{
  if (!currentWidget()) return -1;
  if (currentTabName()!=kind) return -1;
  if (numSelected()!=1) return -1;
  int i= ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes()[0].row();
  return i;
}

int Selector::numSelected() const{
  if (!this->currentWidget()) return 0;
  return
      ((QListView*)(this->currentWidget()))
      ->selectionModel()->selectedIndexes().size();
}


void Selector::updateData(const BrfData &data){

  setup(data);

}

void Selector::selectOne(int kind, int i){

  assert(kind>=0 && kind<N_TOKEN);
  QListView* c=tab[kind];
  if (c) {
    c->clearSelection();
    QModelIndex li = tableModel[kind]->pleaseCreateIndex(i,0);
    c->selectionModel()->setCurrentIndex(li,QItemSelectionModel::NoUpdate);
    c->selectionModel()->select(li,QItemSelectionModel::Select);
    c->scrollTo(li,QAbstractItemView::PositionAtCenter);

    this->setCurrentWidget(c);
    c->setFocus();

  }
  onChanged();
}


void Selector::goNextTab(){
  int j=currentIndex();
  if (j<count()) setCurrentIndex(j+1);
}

void Selector::goPrevTab(){
  int j=currentIndex();
  if (j>0) setCurrentIndex(j-1);
}

void Selector::moveSel(int dx){
  QListView* c=(QListView*)this->currentWidget();

  if (c) {
    QModelIndex li=c->selectionModel()->selectedIndexes()[0];
    QModelIndex lj=li.sibling(li.row()+dx,li.column());
    if (lj.isValid()) {
      c->clearSelection();
      c->selectionModel()->select(lj,QItemSelectionModel::Select);
      c->selectionModel()->setCurrentIndex(lj,QItemSelectionModel::NoUpdate);
      c->setFocus();
    }
  }
}

int Selector::currentTabName() const{
  for (int i=0; i<N_TOKEN; i++)
    if (this->currentWidget()==tab[i]) return i;
  return NONE;
}

void Selector::contextMenuEvent(QContextMenuEvent *event)
{

   QMenu menu(this);

   if (!this->currentWidget()) { event->ignore(); return; }

   QModelIndexList sel=
       ((QListView*)(this->currentWidget()))->selectionModel()->selectedIndexes() ;

   bool onesel = sel.size()==1;
   bool mulsel = sel.size()>1;
   bool nosel = sel.size()==0;
   int seli = 0;
   if (!nosel) seli = sel[0].row();
   int t = currentTabName();


   QString title ("title");
   menu.setTitle(title);

   menu.addAction(removeAct);
   if (onesel) renameAct->setText(tr("Rename")); else renameAct->setText(tr("Group rename"));
   menu.addAction(renameAct);
   if (onesel) {
     menu.addAction(duplicateAct);
     //menu.addSeparator();
     menu.addAction(moveUpAct);
     menu.addAction(moveDownAct);
   }

   if (onesel) {

     menu.addSeparator();

     if (t==MESH) {
       menu.addAction(exportStaticMeshAct);
       if (data->mesh[ seli ].isRigged)
         menu.addAction(exportRiggedMeshAct);
       if (data->mesh[ seli ].frame.size()>0)
          menu.addAction(exportMovingMeshAct);
     }

     if (t==SKELETON) {
       menu.addAction(exportSkeletonAct);
       menu.addAction(exportSkinAct);
       menu.addSeparator();
       menu.addAction(exportSkeletonModAct);
       menu.addAction(importSkeletonModAct);
     }
     if (t==ANIMATION){
       menu.addAction(exportAnimationAct);
       menu.addAction(exportSkinForAnimationAct);
     }
     if (t==BODY) {
       menu.addAction(exportBodyAct);
     }
   } else {
     if (t==MESH) {
       menu.addSeparator();
       menu.addAction(exportMeshGroupAct);
       menu.addAction(exportMeshGroupManyFilesAct);
     }
   }

   // tool section
   bool sep = false;
   if (!nosel) {
   if (t==MESH) {
     const BrfMesh &mesh(data->mesh[ seli ]);
     if (mesh.isRigged) {
       if (!sep) menu.addSeparator(); sep=true;
       menu.addAction(reskeletonizeAct);
     }
     //menu.addAction(transferRiggingAct);
     if (!sep) menu.addSeparator(); sep=true;
     menu.addAction(flipAct);
     menu.addAction(transformAct);
     //menu.addAction(scaleAct);
     menu.addAction(meshRecomputeNormalsAndUnify);
     menu.addAction(meshUnify);
     if (!onesel && !nosel) { menu.addAction(meshMerge); }
     menu.addAction(meshMountOnBone);
     QMenu *m = menu.addMenu("Backfacing faces");
     m->addAction(meshRemoveBackfacing);
     m->addAction(meshAddBackfacing);

     m = menu.addMenu("Discard");

     m->addAction(discardAniAct);
     discardAniAct->setEnabled(mulsel || (mesh.frame.size()>1));
     m->addAction(discardRigAct);
     discardRigAct->setEnabled(mulsel || mesh.isRigged);
     m->addAction(discardColAct);
     discardColAct->setEnabled(mulsel || mesh.hasVertexColor);

   }

   if (t==BODY) {
     if (!sep) menu.addSeparator(); sep=true;
     menu.addAction(flipAct);
     menu.addAction(transformAct);
   }
   if (t==ANIMATION) {
     if (onesel) {
       if (!sep) menu.addSeparator(); sep=true;
       menu.addAction(breakAniAct);
       menu.addAction(breakAniWithIniAct);
       menu.addAction(shiftAniAct);
     }
   }
   }
   // add to reference
   if (onesel && t==MESH) {
     menu.addSeparator();
     QMenu* refMenu=menu.addMenu("Add to reference skins");
     int N=reference->GetFirstUnusedLetter();
     for (int i=0; i<N; i++) {
       addToRefMeshAct[i]->setText(tr("to Skin Set %1").arg(char('A'+i)) );
       refMenu->addAction(addToRefMeshAct[i]);
     }
     if (N<10) {
       addToRefMeshAct[N]->setText(tr("to Skin Set %1 [new set]").arg(char('A'+N)) );
       refMenu->addAction(addToRefMeshAct[N]);
     }
   }
   if (onesel && t==ANIMATION) {
     menu.addSeparator();
     menu.addAction(addToRefAnimAct);
   }
   if (onesel && t==SKELETON) {
     menu.addSeparator();
     menu.addAction(addToRefSkelAct);
   }




   //menu.addAction(moveUp);
   //menu.addAction(moveDown);

   menu.exec(event->globalPos());
   event->accept();


 }


template<class BrfType>
void Selector::addBrfTab(const vector<BrfType>  &v){

  int ti = BrfType::tokenIndex();

  tableModel[ti]->clear();
  if (v.size()!=0) {

    for (unsigned int k=0; k<v.size(); k++) {
      tableModel[ti]->vec.push_back( QString( v[k].name ) );
    }
    //tab[ti]->setModel(tableModel[ti]);

    tableModel[ti]->updateChanges();

    //tab[ti]->viewport()->update();

    if (tab[ti]->selectionModel()->selectedIndexes().size()==0)
    tab[ti]->selectionModel()->select(
        tableModel[ti]->pleaseCreateIndex(0,0),
        QItemSelectionModel::Select
    );


  } else {
  }

  /*QModelIndexList list = tab[ti]->selectionModel()->selectedIndexes();
  for (int i=0; i<list.size(); i++){
    if (list[i].row()>=tableModel[ti]->size()) tab[ti]->selectionModel()->select(
        tableModel[ti]->pleaseCreateIndex(i,0),QItemSelectionModel::Deselect);
  }*/
 //tab[ti]->selectionModel()->setCurrentIndex(tableModel[ti]->pleaseCreateIndex(0,0),QItemSelectionModel::NoUpdate);

}

void Selector::hideEmpty(){

  int n=0;
  for (int i=0; i<N_TOKEN; i++){
    if (tableModel[i]->size()) n++;
  }

  int reorder[N_TOKEN] = {
    BODY,
    MESH,
    MATERIAL,
    TEXTURE,
    SHADER,
    ANIMATION,
    SKELETON,
  };

  int prev = 0;
  for (int ii=0; ii<N_TOKEN; ii++) {
    int i = reorder[ii];
    int v = tableModel[i]->size();
    if (v==0) {
      int j = indexOf( tab[i] );
      if (j>=0) removeTab(j);
      //tab[i]->setVisible(false);

      //int j = indexOf( tab[i] );
      //if (j>=0) this->removeTab(  j );
    } else {
      QString title(tokenTabName[i]);
      if (n>3) title.truncate(3);
      else if (n>2) title.truncate(4);
      if (n>3)
        title = QString("%1%2").arg( title ).arg(v);
      else title = QString("%1(%2)").arg( title ).arg(v);
      int j = indexOf( tab[i] );
      if (j<0) insertTab(prev,tab[i], title);
      else setTabText(j,title);
      tab[i]->setVisible(true);
      prev++;

    }
  }
}

void Selector::onBreakAniWithIni(){
  int s = tab[ANIMATION]->selectionModel()->selectedIndexes().constBegin()->row();
  emit breakAni(s,true);
}

void Selector::onBreakAni(){
  int s = tab[ANIMATION]->selectionModel()->selectedIndexes().constBegin()->row();
  emit breakAni(s,false);
}

void Selector::onChanged(){
  static QModelIndexList empty;
  for(int ti=0; ti<N_TOKEN; ti++) if (tab[ti]) {
    //if (tab[ti]) tab[ti]->clearSelection();

    if (this->currentWidget()==tab[ti]) {
      QItemSelectionModel * tmp = tab[ti]->selectionModel();
      assert(tmp);
      emit setSelection(
          tmp->selectedIndexes()
          , ti );
      return;
    }
  }


  //emit setSelection(empty , NONE );

}




void Selector::setup(const BrfData &_data){


  addBrfTab<BrfMesh> (_data.mesh);
  addBrfTab<BrfShader> (_data.shader);
  addBrfTab<BrfTexture> (_data.texture);
  addBrfTab<BrfMaterial> (_data.material);
  addBrfTab<BrfSkeleton> (_data.skeleton);
  addBrfTab<BrfAnimation> (_data.animation);
  addBrfTab<BrfBody> (_data.body);
  data = &_data;

  hideEmpty();
  onChanged();

  //static QModelIndexList *empty = new QModelIndexList();
  //emit setSelection(*empty , NONE );

}
