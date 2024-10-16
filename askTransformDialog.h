/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#ifndef ASKTRANSFORMDIALOG_H
#define ASKTRANSFORMDIALOG_H

#include <QDialog>


namespace Ui {
    class AskTransformDialog;
}

class QAbstractButton;

class AskTransformDialog : public QDialog {
    Q_OBJECT
public:
    AskTransformDialog(QWidget *parent = 0);
    ~AskTransformDialog();

    float* matrix;
    bool* applyToAll;
    bool* applyToAllFrames;

    void setApplyToAllLoc(bool *loc);
    void setSensitivityOne(double sens); // sets sensibility of transform when moving a single object
    void setSensitivityAll(double sens); // ... or when moving multiple objects
    void setOneFrameOnly(bool );
    void setMultiObj(bool);

    void applyAlignments();
    void setBoundingBox(float* minv, float * maxv);
    void setBoundingBoxForAllButLast(float* minv, float * maxv);
    void setRotCenterPoint(float lx, float ly, float lz, float gx, float gy, float gz);

protected:
    void changeEvent(QEvent *e);
    float bb_min[3], bb_max[3];
    float bb_min_all_but_last[3], bb_max_all_but_last[3];
    float rot_center_point[3][3];

public slots:
    int exec();
    void reset();
    void update();

private:
    Ui::AskTransformDialog *ui;
    double sensitivityOne, sensitivityAll;
private slots:
    void updateSensitivity();
    void onAlignmentLX(bool);
    void onAlignmentLY(bool);
    void onAlignmentLZ(bool);
    void onAlignmentMX(bool);
    void onAlignmentMY(bool);
    void onAlignmentMZ(bool);
    void onAlignmentRX(bool);
    void onAlignmentRY(bool);
    void onAlignmentRZ(bool);
    void disableAlignX();
    void disableAlignY();
    void disableAlignZ();
    void onButton(QAbstractButton*);
signals:
    void changed();

};

#endif // ASKTRANSFORMDIALOG_H
