#include "widget.h"
#include "ui_widget0.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


/*
void Widget::on_pushButton_clicked()
{
    //QString InPutTape = ui->StartMachine->text();

}
*/

void Widget::ShowPt(int num)
{
    QString OutPt;
    for(int i=0;i<Pthigh;i++)
    {
        if(i!=num)
        {
            OutPt+=" \n";
        }
        else
        {
            OutPt+=">\n";
        }
    }
    ui->IPTEdit->setText(OutPt);
}

void Widget::ShowWPt(int num)
{
    QString OutPt;
    for(int i=0;i<=Pthigh;i++)
    {
        if(i!=num)
        {
            OutPt+=" \n";
        }
        else
        {
            OutPt+=">\n";
        }
    }
    ui->WPTEdit->setText(OutPt);
}

void Widget::OutPutWorTape()
{
    QString SOutPut;
    for (int i = 0; i < WorkTapePt; i++)
    {
        SOutPut+=WorkTape[i]+"\n";
        //cout << "Stack" << RecStackPt << ": " << RecStack[i] << endl;
    }
    ui->WorkEdit->setText(SOutPut);
}


void Widget::on_StartMachine_clicked()
{


    int numlist[1000]={0};
    int i = 0, f = 0, m = 0, n=0;
    int RecStep = 0;
    WorkTapePt=3;
    ShowWPt(0);
    QString TempInput;
    QString TempOutput;
    PreStep=0;
    InPutLine = " ";
    InPutLine += ui->InputEdit->text();

    for(n=0;n<=RecStackPt;n++)
    {
        RecStack[n]=" ";
    }
    RecStackPt=0;
    ui->StartMachine->text();
    for(m=0;m<InPutLine.size();m++)
    {
        if(InPutLine[m]>='0'&&InPutLine[m]<='9')
        {
        TempInput=InPutLine[m];
        numlist[i]=numlist[i]*10+TempInput.toInt();
        }
        else if(m==0)
        {

        }
        else if(InPutLine[m-1]>='0'&&InPutLine[m-1]<='9')
        {
            i++;
        }
    }
    NumSort(numlist, i);

    for (int j = 0; j <= i; j++)
    {
        TempOutput+=QString::number(numlist[j]);
        TempOutput+="\n";
    }
    InPutLine = " ";
    InPutLine = ui->TofindEdit->text();
    f=InPutLine.toInt();

    //TempOutput+="\t";
    ui->InputtapeEdit->setText(TempOutput);
    //cin >> f;

    //TempOutput=" ";
    Pthigh=i;
    ShowPt(0);
    TempOutput=QString::number(BFind(numlist, 0, i, f, &RecStep));
    ui->ResEdit->setText(TempOutput);
}
/*
void Widget::on_NextStep_clicked()
{

}
*/

void Widget::on_NextStep_clicked()
{
    //int i=0;

    /*
    for(i=0;i<10;i++)
    {
        while(1)
        {
            if(ConSig=="0")
            {
                break;
            }
        }

    }
    */

    //ui->InputEdit->setText("step");


    //InPutLine += ui->StartMachine->text();
    //ui->InputEdit->setText(InPutLine);
    //ConSig = ui->StartMachine->text();
    PreStep++;


}
/*
int Widget::BFind(int numlist[], int lo, int hi, int tofind, int *RecStep)
{

}
*/
void Widget::NumSort(int numlist[], int hi)
{
    int t = 0;
    for (int j = hi; j > 0; j--)
    {
        for (int i = 0; i < j; i++)
        {
            if (numlist[i] > numlist[i + 1])
            {
                t = numlist[i];
                numlist[i] = numlist[i + 1];
                numlist[i + 1] = t;
            }
        }
    }
}


void Widget::OutPutStack()
{
    QString SOutPut;
    for (int i = 1; i <= RecStackPt; i++)
    {
        SOutPut+="Stack"+QString::number(i)+": "+RecStack[i]+"\n";
        //cout << "Stack" << RecStackPt << ": " << RecStack[i] << endl;
    }
    ui->StackEdit->setText(SOutPut);
}


int Widget::BFind(int numlist[], int lo, int hi, int tofind, int *RecStep)
{
    QString OutPutStep;
    int res;
    int TapGrid = 2;
    WorkTape[0]=QString::number(lo);
    WorkTape[1]=QString::number(hi);
    WorkTape[2]=" ";
    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    OutPutWorTape();
    ShowWPt(0);
    WorkTapePt--;
    (*RecStep)++;
    OutPutStep="push fun("+QString::number(lo)+','+QString::number(hi)+");\n";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(OutPutStep);
    ui->GridsEdit->setText(QString::number(TapGrid));
    //cout << "Step" << (*RecStep) << ": push fun" << lo << ',' << hi << ';' << endl;
    //cout << "Tap Grid :" << TapGrid << endl;
    RecStackPt++;
    RecStack[RecStackPt] = "fun \t" + QString::number(lo)+ '\t' +QString::number(hi);
    OutPutStack();
    //cout << endl;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    WorkTapePt-=2;
    WorkTapePt+=2;
    OutPutWorTape();
    (*RecStep)++;

    OutPutStep="ReadLow";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(OutPutStep);
    ui->GridsEdit->setText(QString::number(TapGrid));
    ShowWPt(0);
    //cout << "Step" << (*RecStep) << ": ReadLow" << endl;
    //cout << "Tap Grid :" << TapGrid << endl;
    OutPutStack();
    //cout << endl;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;

    OutPutStep="ReadHigh";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(OutPutStep);
    ui->GridsEdit->setText(QString::number(TapGrid));
    //ShowPt(hi);
    ShowWPt(1);
    //cout << "Step" << (*RecStep) << ": ReadHigh" << endl;
    //cout << "Tap Grid :" << TapGrid << endl;
    OutPutStack();
    //cout << endl;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;

    OutPutStep="CalMid";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(OutPutStep);
    WorkTape[2]=QString::number((hi+lo)/2);
    WorkTapePt++;
    ShowWPt(2);
    OutPutWorTape();
    //cout << "Step" << (*RecStep) << ": CalMid" << endl;
    TapGrid++;
    ui->GridsEdit->setText(QString::number(TapGrid));
    //cout << "Tap Grid :" << TapGrid << endl;
    OutPutStack();
    //cout << endl;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;
    ShowPt((lo + hi) / 2);
    OutPutStep="ReadMid";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(OutPutStep);
    ui->GridsEdit->setText(QString::number(TapGrid));

    //cout << "Step" << (*RecStep) << ": ReadMid" << endl;
    //cout << "Tap Grid :" << TapGrid << endl;
    OutPutStack();
    //cout << endl;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;

    OutPutStep="CompareMid";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(OutPutStep);
    ui->GridsEdit->setText(QString::number(TapGrid));

    //cout << "Step" << (*RecStep) << ": CompareMid" << endl;
    //cout << "Tap Grid :" << TapGrid << endl;
    OutPutStack();
    //cout << endl;

    if ((hi - lo) < 2)
    {
        if (tofind == numlist[hi])
        {
            res = numlist[hi];
        }
        else if (tofind == numlist[lo])
        {
            res = numlist[lo];
        }
        else
        {
            res = -1;
        }
    }
    else
    {
        if (numlist[(lo + hi) / 2] > tofind)
        {
            res = BFind(numlist, lo, (lo + hi) / 2, tofind, RecStep);
        }
        else if (numlist[(lo + hi) / 2] < tofind)
        {
            res = BFind(numlist, (lo + hi) / 2, hi, tofind, RecStep);
        }
        else
        {
            res = (lo + hi) / 2;
        }
    }

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;

    OutPutStep="pop fun("+QString::number(lo)+','+QString::number(hi)+");\n";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(OutPutStep);

    //cout << "Step" << (*RecStep) << ": pop fun" << lo << ',' << hi << ';' << endl;
    RecStackPt--;
    OutPutStack();
    //cout << endl;
    return res;
}





