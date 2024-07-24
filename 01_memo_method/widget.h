#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    int BFind(int numlist[], int lo, int hi, int tofind, int *RecStep);
    //void NumSort(int numlist[], int hi);
    void OutPutStack();
    void ShowPt(int num);
    void ShowWPt(int num);
    void OutPutWorTape();
    int CulPos(int x, int y);
    void MemoBag(int x, int y, int* RecStep, int * TapGrid);
    void ShowCH();


private slots:
    //void on_pushButton_clicked();

    void on_StartMachine_clicked();

    void on_NextStep_clicked();

private:
    Ui::Widget *ui;
    QString InPutLine;
    QString TuringPointer;
    QString MachineStack;
    QString OutPutTape;
    QString MachineGrade;
    QString MachineStaps;
    QString CulRes;
    QString ConSig;

    QString RecStack[1000];
    int RecStackPt=0;

    QString WorkTape[1000];
    int WorkTapePt=0;

    int PreStep=0;
    int Pthigh=0;


    int InPutList[1000];
    int WorkList[100][100];
    int WorkTap[1000];
};
#endif // WIDGET_H
