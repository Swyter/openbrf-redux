/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include "ui_askHueSatBriDialog.h"
#include "askHueSatBriDialog.h"

#include<QtGui>
#include<assert.h>


AskHueSatBriDialog::AskHueSatBriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog){

  ui->setupUi(this);
  connect(ui->sliderBrightness, SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->sliderContrast,   SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->sliderHue,        SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->sliderSat,        SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->applyToLastSel,   SIGNAL(clicked(bool)),     this, SLOT(onClicked())); /* swy: this checkbox should copy the behavior from the roto-rescale tool */
  this->setStyleSheet(
    "* { color: black; background-color: aliceblue }"  /* swy: make the so that the dialog uses light mode to make QSlider markers visible */
  );
}
void AskHueSatBriDialog::onClicked(){
    emit onAnySliderMove(0); /* swy: the checkbox connect clicked(bool) signal can't seemingly handle the onAnySliderMove() directly on Qt6 for some reason */
}
void AskHueSatBriDialog::onAnySliderMove(int){
  emit anySliderMoved(
    ui->sliderContrast->value(),
    ui->sliderHue->value(),
    ui->sliderSat->value(),
    ui->sliderBrightness->value(),
    ui->applyToLastSel->isChecked()
   );
}

AskHueSatBriDialog::~AskHueSatBriDialog(){}
