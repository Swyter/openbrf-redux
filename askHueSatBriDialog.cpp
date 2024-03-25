/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include "ui_askHueSatBriDialog.h"
#include "askHueSatBriDialog.h"

#include<QtGui>
#include<assert.h>


AskHueSatBriDialog::AskHueSatBriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog){

  ui->setupUi(this);
  connect(ui->sliderBrighness,SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->sliderContrast,SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->sliderHue,SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->sliderSat,SIGNAL(valueChanged(int)), this, SLOT(onAnySliderMove(int)));
  connect(ui->applyToLastSel, SIGNAL(stateChanged(int)), this, SLOT(onAnySliderMove(int))); /* swy: this checkbox should copy the behavior from the roto-rescale tool */
}

void AskHueSatBriDialog::onAnySliderMove(int){
  emit anySliderMoved(
    ui->sliderContrast->value(),
    ui->sliderHue->value(),
    ui->sliderSat->value(),
    ui->sliderBrighness->value(),
    ui->applyToLastSel->isChecked()
   );
}

AskHueSatBriDialog::~AskHueSatBriDialog(){}
