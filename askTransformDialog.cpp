/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include "askTransformDialog.h"
#include "ui_askTransformDialog.h"

#include <math.h>
#include <vcg/space/point3.h>
#include <vcg/math/matrix44.h>
#include <vcg/space/box3.h>


void AskTransformDialog::setApplyToAllLoc(bool *loc){
	applyToAll = loc;
	ui->applyToLastSel->setChecked(!(*loc));
	updateSensitivity();
}

void AskTransformDialog::setBoundingBox(float *minv, float *maxv){
  for (int i=0; i<3; i++) {
    bb_min[i]= minv[i];
    bb_max[i]= maxv[i];
  }
}

void AskTransformDialog::setBoundingBoxForAllButLast(float *minv, float *maxv){
  for (int i=0; i<3; i++) {
    bb_min_all_but_last[i]= minv[i];
    bb_max_all_but_last[i]= maxv[i];
  }
}

/* swy: save the position/coords of the middle of the bounding box so that we
        can go from global to local coordinates and do local rotations */
void AskTransformDialog::setRotCenterPoint(float lx, float ly, float lz, float ax, float ay, float az){
  rot_center_point[1][0] = lx;
  rot_center_point[1][1] = ly;
  rot_center_point[1][2] = lz; /* swy: the center coordinates of the bounding box of just the last selected object */

  rot_center_point[0][0] = ax; /* swy: the center coordinates of the bounding box of the sum of all the selected elements together */
  rot_center_point[0][1] = ay;
  rot_center_point[0][2] = az;
}

AskTransformDialog::AskTransformDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskTransformDialog)
{
		sensitivityOne = sensitivityAll = 0.25;
		applyToAll = NULL;

    ui->setupUi(this);
    //reset();

    /* swy: make the alignment icons vertical for the Y axis; making them way more intuitive to use.
            suggested by @tos1 in the TaleWorlds forum. https://stackoverflow.com/a/66429169/674685
                                                        https://stackoverflow.com/a/57692067/674685 */
    QTransform tr; tr.rotate(90); /* swy: clockwise */
    ui->  midY->setIcon(ui->  midY->icon().pixmap(16, 16, QIcon::Mode::Normal, QIcon::State::On).transformed(tr));
    ui-> leftY->setIcon(ui-> leftY->icon().pixmap(16, 16, QIcon::Mode::Normal, QIcon::State::On).transformed(tr));
    ui->rightY->setIcon(ui->rightY->icon().pixmap(16, 16, QIcon::Mode::Normal, QIcon::State::On).transformed(tr));

    //connect(ui->rotx,SIGNAL())
    connect(ui->rotx,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->roty,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->rotz,SIGNAL(valueChanged(double)),this,SLOT(update()));

    connect(ui->trax,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->tray,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->traz,SIGNAL(valueChanged(double)),this,SLOT(update()));

    connect(ui->trax,SIGNAL(valueChanged(double)),this,SLOT(disableAlignX()));
    connect(ui->tray,SIGNAL(valueChanged(double)),this,SLOT(disableAlignY()));
    connect(ui->traz,SIGNAL(valueChanged(double)),this,SLOT(disableAlignZ()));

    connect(ui->scx,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->scy,SIGNAL(valueChanged(double)),this,SLOT(update()));
    connect(ui->scz,SIGNAL(valueChanged(double)),this,SLOT(update()));

    connect(ui->midX,SIGNAL(toggled(bool)),this,SLOT(onAlignmentMX(bool)));
    connect(ui->midY,SIGNAL(toggled(bool)),this,SLOT(onAlignmentMY(bool)));
    connect(ui->midZ,SIGNAL(toggled(bool)),this,SLOT(onAlignmentMZ(bool)));
    connect(ui->leftX,SIGNAL(toggled(bool)),this,SLOT(onAlignmentLX(bool)));
    connect(ui->leftY,SIGNAL(toggled(bool)),this,SLOT(onAlignmentLY(bool)));
    connect(ui->leftZ,SIGNAL(toggled(bool)),this,SLOT(onAlignmentLZ(bool)));
    connect(ui->rightX,SIGNAL(toggled(bool)),this,SLOT(onAlignmentRX(bool)));
    connect(ui->rightY,SIGNAL(toggled(bool)),this,SLOT(onAlignmentRY(bool)));
    connect(ui->rightZ,SIGNAL(toggled(bool)),this,SLOT(onAlignmentRZ(bool)));

    connect(ui->localRot,SIGNAL(stateChanged(int)),this,SLOT(update()));
    connect(ui->localScl,SIGNAL(stateChanged(int)),this,SLOT(update()));
    connect(ui->applyToLastSel,SIGNAL(stateChanged(int)),this,SLOT(update()));
		connect(ui->checkBox,SIGNAL(clicked()),this,SLOT(update()));

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButton(QAbstractButton*)));
}

void AskTransformDialog::onButton(QAbstractButton *b){
  if (b==ui->buttonBox->button(QDialogButtonBox::Reset)) reset();
}

void AskTransformDialog::setMultiObj(bool multObj){
  ui->applyToLastSel->setEnabled(multObj);
}

void AskTransformDialog::disableAlignX(){
  ui->midX->setChecked(false);
  ui->leftX->setChecked(false);
  ui->rightX->setChecked(false);
}

void AskTransformDialog::disableAlignY(){
  ui->midY->setChecked(false);
  ui->leftY->setChecked(false);
  ui->rightY->setChecked(false);
}

void AskTransformDialog::disableAlignZ(){
  ui->midZ->setChecked(false);
  ui->leftZ->setChecked(false);
  ui->rightZ->setChecked(false);
}

int AskTransformDialog::exec(){
  applyAlignments();
  return QDialog::exec();
}

void AskTransformDialog::applyAlignments(){
  ui->trax->blockSignals(true);
  ui->tray->blockSignals(true);
  ui->traz->blockSignals(true);

  if (ui->applyToLastSel->isChecked()) {
    /* swy: new! when the «Apply to last selected object only» mode is on; we center/align around the cumulative
            bounding box of all the other meshes; that way we can recenter objects around other objects */
    if (ui->midX->isChecked())   ui->trax->setValue( ((bb_min_all_but_last[0]+bb_max_all_but_last[0])/2.f) - rot_center_point[1][0] );
    if (ui->midY->isChecked())   ui->tray->setValue( ((bb_min_all_but_last[1]+bb_max_all_but_last[1])/2.f) - rot_center_point[1][1] );
    if (ui->midZ->isChecked())   ui->traz->setValue( ((bb_min_all_but_last[2]+bb_max_all_but_last[2])/2.f) - rot_center_point[1][2] );

    if (ui->leftX->isChecked())  ui->trax->setValue( bb_min_all_but_last[0] - rot_center_point[1][0]);
    if (ui->leftY->isChecked())  ui->tray->setValue( bb_min_all_but_last[1] - rot_center_point[1][1]);
    if (ui->leftZ->isChecked())  ui->traz->setValue( bb_min_all_but_last[2] - rot_center_point[1][2]);

    if (ui->rightX->isChecked()) ui->trax->setValue( bb_max_all_but_last[0] - rot_center_point[1][0]);
    if (ui->rightY->isChecked()) ui->tray->setValue( bb_max_all_but_last[1] - rot_center_point[1][1]);
    if (ui->rightZ->isChecked()) ui->traz->setValue( bb_max_all_but_last[2] - rot_center_point[1][2]);
    
  } else {
    /* swy: when not, keep using the classic alignment mode that Marco wrote */
    if (ui->midX->isChecked()) ui->trax->setValue( - (bb_min[0]+bb_max[0])/2 );
    if (ui->midY->isChecked()) ui->tray->setValue( - (bb_min[1]+bb_max[1])/2 );
    if (ui->midZ->isChecked()) ui->traz->setValue( - (bb_min[2]+bb_max[2])/2 );

    if (ui->leftX->isChecked()) ui->trax->setValue( - bb_min[0] );
    if (ui->leftY->isChecked()) ui->tray->setValue( - bb_min[1] );
    if (ui->leftZ->isChecked()) ui->traz->setValue( - bb_min[2] );

    if (ui->rightX->isChecked()) ui->trax->setValue( - bb_max[0] );
    if (ui->rightY->isChecked()) ui->tray->setValue( - bb_max[1] );
    if (ui->rightZ->isChecked()) ui->traz->setValue( - bb_max[2] );
  }

  ui->trax->blockSignals(false);
  ui->tray->blockSignals(false);
  ui->traz->blockSignals(false);

  update();
}

void AskTransformDialog::onAlignmentLX(bool b){ if (b) {
    ui->midX->setChecked(false);  ui->rightX->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentMX(bool b){ if (b) {
    ui->leftX->setChecked(false);  ui->rightX->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentRX(bool b){ if (b) {
    ui->midX->setChecked(false);  ui->leftX->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentLZ(bool b){ if (b) {
    ui->midZ->setChecked(false);  ui->rightZ->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentMZ(bool b){ if (b) {
    ui->leftZ->setChecked(false);  ui->rightZ->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentRZ(bool b){ if (b) {
    ui->midZ->setChecked(false);  ui->leftZ->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentLY(bool b){ if (b) {
    ui->midY->setChecked(false);  ui->rightY->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentMY(bool b){ if (b) {
    ui->leftY->setChecked(false);  ui->rightY->setChecked(false);
    applyAlignments();
}}
void AskTransformDialog::onAlignmentRY(bool b){ if (b) {
    ui->midY->setChecked(false);  ui->leftY->setChecked(false);
    applyAlignments();
}}

void AskTransformDialog::setSensitivityAll(double sens){
	sensitivityAll = sens;
}

void AskTransformDialog::setSensitivityOne(double sens){
	sensitivityOne = sens;
}

void AskTransformDialog::updateSensitivity(){
  //if (!applyToAll) return;
	float sens = (applyToAll)? sensitivityAll:sensitivityOne;
	ui->trax->setSingleStep(sens);
	ui->tray->setSingleStep(sens);
	ui->traz->setSingleStep(sens);
}

void AskTransformDialog::reset(){
  ui->scx->setValue(100);
  ui->scy->setValue(100);
  ui->scz->setValue(100);
  ui->trax->setValue(0);
  ui->tray->setValue(0);
  ui->traz->setValue(0);
  ui->rotx->setValue(0);
  ui->roty->setValue(0);
  ui->rotz->setValue(0);
  disableAlignX();
  disableAlignY();
  disableAlignZ();
  update();
}

static float toRad(float t) {return t*M_PI/180;}

void AskTransformDialog::update(){

	if (applyToAll) *applyToAll = ! ( ui->applyToLastSel->isChecked());
	updateSensitivity();

  if (ui->checkBox->isChecked()) {
    ui->scy->blockSignals(true);
    ui->scz->blockSignals(true);
    ui->scy->setValue( ui->scx->value());
    ui->scz->setValue( ui->scx->value());
    ui->scy->blockSignals(false);
    ui->scz->blockSignals(false);
  }
  ui->scy->setEnabled( !ui->checkBox->isChecked() );
  ui->scz->setEnabled( !ui->checkBox->isChecked() );

  vcg::Matrix44f rot_move_to_center;
  vcg::Matrix44f rot;
  vcg::Matrix44f t;
  vcg::Matrix44f sc;

  /* swy: compensate for the original mesh offset respect to the {0,0,0}
          world position and any extra movement we do here */
  rot_move_to_center.Identity();
  rot_move_to_center.SetTranslate(
    vcg::Point3f(
      rot_center_point[ui->applyToLastSel->isChecked()][0] + ui->trax->value(), /* swy: switch between the center of all the elements (index 0), or the center of just the last element (index 1) */
      rot_center_point[ui->applyToLastSel->isChecked()][1] + ui->tray->value(),
      rot_center_point[ui->applyToLastSel->isChecked()][2] + ui->traz->value())
  );

  rot.FromEulerAngles(toRad( ui->rotx->value() ),toRad( ui->roty->value() ),toRad( ui->rotz->value() ));

  /* swy: move it to the center of the universe before doing
          rotations and then move the rotated mesh back */
  if (ui->localRot->isChecked())
    rot = rot_move_to_center * rot * vcg::Inverse(rot_move_to_center);

  t.SetTranslate(ui->trax->value(),ui->tray->value(),ui->traz->value());
  sc.SetScale(vcg::Point3f(ui->scx->value(),ui->scy->value(),ui->scz->value())/100.0f);

  /* swy: move it to the center of the universe before doing
          re-scaling and then move the scaled mesh back */
  if (ui->localScl->isChecked())
    sc = rot_move_to_center * sc * vcg::Inverse(rot_move_to_center);

  /* swy: changed the order of operations from sc*rot*t to rot*sc*t, to avoid
          shearing while combining rotation with non-uniform scaling */
  vcg::Matrix44f res = rot*sc*t;

  for (int i=0,x=0; x<4; x++)
    for (int y=0; y<4; y++,i++)
      matrix[i]=res[y][x];

  emit changed();
}

AskTransformDialog::~AskTransformDialog()
{
  delete ui;
}

void AskTransformDialog::changeEvent(QEvent *e)
{

    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
