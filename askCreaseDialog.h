#ifndef ASKCREASEDIALOG_H
#define ASKCREASEDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class AskCreaseDialog;
}
class QSlider;

class AskCreaseDialog : public QDialog {
    Q_OBJECT
public:
    AskCreaseDialog(QWidget *parent = 0);
    ~AskCreaseDialog();
    QSlider* slider();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskCreaseDialog *m_ui;
};

#endif // ASKCREASEDIALOG_H
