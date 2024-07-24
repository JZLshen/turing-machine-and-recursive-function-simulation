#include "widget.h"
#include "ui_widget.h"

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
    int k=1;
    num--;
    for(int i=0;i<(InPutList[0]+1)*InPutList[1];i++)
    {
        OutPt+=QString::number(k);
        if(i!=num)
        {
            OutPt+=":  \n";
        }
        else
        {
            if(k>9)
            {
                OutPt+=": >\n";
            }
            else
            {
                OutPt+=":  >\n";
            }
        }
        k++;
    }
    ui->WPTEdit->setText(OutPt);
}


void Widget::ShowCH()
{
    QString OutPt,OutPt1;
    int i=0, j=0,k=1;
    for(i=0;i<InPutList[1];i++)
    {
        for(j=0;j<=InPutList[0];j++)
        {
            OutPt+=QString::number(WorkList[i][j]);
            OutPt+="\t";
            //OutPt+=" ";
        }
        OutPt+="\n";
    }
    ui->CharEdit->setText(OutPt);

    for(i=0;i<InPutList[1];i++)
    {
        for(j=0;j<=InPutList[0];j++)
        {
            OutPt1+=QString::number(k);
            OutPt1+=": ";
            OutPt1+=QString::number(WorkList[i][j]);
            OutPt1+="\n";
            k++;
        }
        //OutPt+="\n";
    }
    ui->WorkEdit->setText(OutPt1);
}


void Widget::OutPutWorTape()
{
    QString OutPt;
    int i=0, j=0;
    for(i=0;i<InPutList[1];i++)
    {
        for(j=0;j<InPutList[0];j++)
        {
            OutPt+=QString::number(WorkList[i][j]);
            OutPt+=" ";
        }
        //OutPt+="\n";
    }
    ui->WorkEdit->setText(OutPt);
    /*
    QString SOutPut;
    for (int i = 0; i < WorkTapePt; i++)
    {
        SOutPut+=WorkTape[i]+"\n";
        //cout << "Stack" << RecStackPt << ": " << RecStack[i] << endl;
    }
    ui->WorkEdit->setText(SOutPut);
    */
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

int Widget::CulPos(int x, int y)
{
    return x * (InPutList[0] + 1) + y+1;
}

void Widget::on_StartMachine_clicked()
{

    PreStep=1;
    //ui->ResEdit->setText("0");
/*
    int numlist[1000]={0};
    int i = 0, f = 0,m=0;
    int RecStep = 0;
    WorkTapePt=3;
    ShowWPt(0);
    QString TempInput;
    QString TempOutput;
    InPutLine = " ";
    InPutLine += ui->InputEdit->text();

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
   */


    int i = 0, m = 0, n = 0;
    int RecStep = 1;
    int TapGrid = 9;
    QString TempInput;
    QString TempOutput;
    TempInput=" ";
    InPutLine=" ";
    for (int t1 = 0; t1 < 100; t1++)
    {
        for (int t2 = 0; t2 < 100; t2++)
        {
            WorkList[t1][t2] = -1;
        }
    }
    for(n=1;n<=RecStackPt;n++)
    {
        RecStack[n]=" ";
    }
    RecStackPt=0;
    for(n=0;n<1000;n++)
    {
        InPutList[n]=0;
    }
    InPutList[0]=(ui->CEdit->text()).toInt();
    InPutList[1]=(ui->NEdit->text()).toInt();
    TapGrid=InPutList[0]+InPutList[1];
    InPutLine = ui->InputEdit->text();
    i=2;
    for(m=0;m<InPutLine.size();m++)
    {
        if(InPutLine[m]>='0'&&InPutLine[m]<='9')
        {
            TempInput=InPutLine[m];
            InPutList[i]=InPutList[i]*10+TempInput.toInt();
        }
        else if(m==0)
        {

        }
        else if(InPutLine[m-1]>='0'&&InPutLine[m-1]<='9')
        {
            i++;
        }
    }
    Pthigh=i;
    if (i != 2 * InPutList[1] + 1 || i < 3)
    {
        ui->StepNameEdit->setText("unvalid input");

        ui->ResEdit->setText("#");
        ui->CharEdit->setText(" ");
        ui->InputtapeEdit->setText(" ");
        ui->IPTEdit->setText(" ");
        ui->WorkEdit->setText(" ");
        ui->WPTEdit->setText(" ");
        ui->StackEdit->setText(" ");
        ui->GridsEdit->setText(" ");
        ui->StepsEdit->setText(" ");

        //cout << "unvalid input" << endl;
    }
    else
    {

        ui->ResEdit->setText("0");

        for (int j = 0; j <= i; j++)
        {
            TempOutput+=QString::number(InPutList[j]);
            TempOutput+="\n";
        }
        ui->InputtapeEdit->setText(TempOutput);



        for (int j = 0; j < InPutList[1]; j++)
        {
            WorkList[j][0] = 0;
        }
        for (int j = 0; j <= InPutList[0]; j++)
        {
            if (j < InPutList[i - 1])
            {
                WorkList[InPutList[1] - 1][j] = 0;
            }
            else
            {
                WorkList[InPutList[1] - 1][j] = InPutList[i];
            }
        }
        //(RecStep)++;


        /*
        while((RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        */
        //OutPutWorTape();
        ShowPt(0);
        ShowWPt(1);
        //
        while((RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        TempOutput="ReadCapacity";
        ui->StepsEdit->setText(QString::number((RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(TapGrid));

        //OutPutWorTape();
        ShowPt(0);
        ShowWPt(1);








        //cout << "Step" << (RecStep) << ": ReadCapacity" << endl;
        //cout << "Tap Grid :" << TapGrid << endl;




        ShowCH();
        (RecStep)++;
        while((RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        (RecStep)++;

        TempOutput="ReadNum";
        ui->StepsEdit->setText(QString::number((RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(TapGrid));

        //OutPutWorTape();
        ShowPt(1);
        ShowWPt(1);
        //cout << "Step" << (RecStep) << ": ReadNum" << endl;
        //cout << "Tap Grid :" << TapGrid << endl;
        MemoBag(0, InPutList[0], &RecStep, &TapGrid);
        //cout << "res:" << WorkList[0][InPutList[0]] << endl;
        TempOutput=QString::number(WorkList[0][InPutList[0]]);
        ShowCH();
        ui->ResEdit->setText(TempOutput);
    }







}

void Widget::on_NextStep_clicked()
{
    PreStep++;
}






void Widget::MemoBag(int x, int y, int* RecStep, int * TapGrid)
{
    QString TempOutput;
    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;

    ShowCH();

    TempOutput="push fun(" + QString::number(x)+","+QString::number(y)+") | "+QString::number(CulPos(x, y));
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));
    ShowWPt(CulPos(x,y));


    //cout << "Step" << (*RecStep) << ": push fun" << CulPos(x, y) << ';' << endl;
    /////////////////////
    //cout << "************************Step" << (*RecStep) << ": push fun (" << x << "\t" << y << ");" << endl;
    ///////////////////////////
    //cout << "Tap Grid :" << *TapGrid << endl;
    RecStackPt++;

    RecStack[RecStackPt] = "fun(" + QString::number(x)+","+QString::number(y)+") | "+QString::number(CulPos(x, y));

    //RecStack[RecStackPt] = "fun \t" + to_string(CulPos(x, y));
    OutPutStack();
    //cout << endl;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;

    ShowCH();
    TempOutput="ReadWeight";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));

    ShowPt(x*2+2);
    //ShowWPt(0);
    //OutPutWorTape();

    //cout << "Step" << (*RecStep) << ": ReadWeight" << endl;
    //cout << "Tap Grid :" << *TapGrid << endl;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    (*RecStep)++;


    ShowCH();
    TempOutput="ReadValue";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));


    //OutPutWorTape();
    ShowPt(2*x+3);
    //ShowWPt(0);

    //cout << "Step" << (*RecStep) << ": ReadValue" << endl;
    //cout << "Tap Grid :" << *TapGrid << endl;


    int w = InPutList[2 * (x+1)];
    int v = InPutList[2 * (x+1) + 1];

    if (y == 0)
    {
        (*TapGrid)++;

        ui->GridsEdit->setText(QString::number(*TapGrid));
        WorkList[x][y] = 0;
    }
    else if (w > y)
    {
        if (WorkList[x + 1][y] == -1)
        {
            MemoBag(x + 1, y, RecStep, TapGrid);
        }
        (*TapGrid)++;

        ui->GridsEdit->setText(QString::number(*TapGrid));
        WorkList[x][y] = WorkList[x + 1][y];



        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        (*RecStep)++;
        ShowCH();
        TempOutput="WriteM(" + QString::number(x)+","+QString::number(y)+") | "+QString::number(CulPos(x, y));
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));




    }
    else
    {
        if (WorkList[x + 1][y] == -1)
        {
            MemoBag(x + 1, y, RecStep,TapGrid);
        }

        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

        (*RecStep)++;

        ShowCH();

        TempOutput="ReadM(" + QString::number(x+1)+","+QString::number(y)+") | "+QString::number(CulPos(x+1, y));
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));

        //OutPutWorTape();
        //ShowPt(0);
        ShowWPt(CulPos(x+1,y));

        //cout << "Step" << (*RecStep) << ": ReadM(" << x + 1 << "," << y << ")" << endl;
        //cout << "Tap Grid :" << *TapGrid << endl;
        if (WorkList[x][y - w] == -1)
        {
            MemoBag(x, y - w, RecStep, TapGrid);
        }

        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }


        (*RecStep)++;

        ShowCH();

        TempOutput="ReadM(" + QString::number(x)+","+QString::number(y-w)+") | "+QString::number(CulPos(x, y-w));
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));

        //OutPutWorTape();
        //ShowPt(0);
        ShowWPt(CulPos(x, y-w));

        //cout << "Step" << (*RecStep) << ": ReadM(" << x << "," << y - w << ")" << endl;
        //cout << "Tap Grid :" << *TapGrid << endl;


        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }



        (*RecStep)++;

        ShowCH();

        TempOutput="CalM(" + QString::number(x)+","+QString::number(y-w)+") | "+QString::number(CulPos(x, y-w));
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));


        //OutPutWorTape();
        //ShowPt(0);
        ShowWPt(CulPos(x, y-w));

        //cout << "Step" << (*RecStep) << ": CalM(" << x << "," << y - w << ")" << endl;
        //cout << "Tap Grid :" << *TapGrid << endl;

        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }


        (*RecStep)++;

        ShowCH();

        TempOutput="Cmp";
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));


        //OutPutWorTape();
        //ShowPt(0);
        //ShowWPt(0);

        //cout << "Step" << (*RecStep) << "Cmp" << endl;
        //cout << "Tap Grid :" << *TapGrid << endl;
        if (WorkList[x + 1][y] > WorkList[x+1][y - w] + v)
        {
            WorkList[x][y] = WorkList[x + 1][y];
            (*TapGrid)++;

            ui->GridsEdit->setText(QString::number(*TapGrid));

            while((*RecStep)!=PreStep)
            {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }


            (*RecStep)++;

            ShowCH();

            TempOutput="WriteM(" + QString::number(x)+","+QString::number(y)+") | "+QString::number(CulPos(x, y));
            ui->StepsEdit->setText(QString::number((*RecStep)));
            ui->StepNameEdit->setText(TempOutput);
            ui->GridsEdit->setText(QString::number(*TapGrid));

            //cout << "Step" << (*RecStep) << ": WriteM(" << x << "," << y << ")" << endl;
            //cout << "Tap Grid :" << *TapGrid << endl;
        }
        else
        {
            WorkList[x][y] = WorkList[x + 1][y - w] + v;
            (*TapGrid)++;

            ui->GridsEdit->setText(QString::number(*TapGrid));

            while((*RecStep)!=PreStep)
            {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }


            (*RecStep)++;

            ShowCH();

            TempOutput="WriteM(" + QString::number(x)+","+QString::number(y)+") | "+QString::number(CulPos(x, y));
            ui->StepsEdit->setText(QString::number((*RecStep)));
            ui->StepNameEdit->setText(TempOutput);
            ui->GridsEdit->setText(QString::number(*TapGrid));

            //OutPutWorTape();
            //ShowPt(0);
            ShowWPt(CulPos(x, y));

            //cout << "Step" << (*RecStep) << ": WriteM(" << x << "," << y << ")" << endl;
            //cout << "Tap Grid :" << *TapGrid << endl;
        }
    }
    WorkTap[CulPos(x, y)] = WorkList[x][y];

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }


    (*RecStep)++;

    ShowCH();

    TempOutput="pop fun(" + QString::number(x)+","+QString::number(y)+") | "+QString::number(CulPos(x, y));
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));


    //OutPutWorTape();
    //ShowPt(0);
    ShowWPt(CulPos(x, y));


    //cout << "Step" << (*RecStep) << "pop fun" << CulPos(x, y) << ';' << endl;
    RecStackPt--;
    OutPutStack();
    //cout << endl;
    ///////////////////////////////
    //cout << "//////////////////////////M(" << x << "," << y << ")=" << WorkList[x][y] << endl;
    ///////////////////////////////
}




int Widget::BFind(int numlist[], int lo, int hi, int tofind, int *RecStep)
{
    QString OutPutStep;
    int res;
    int TapGrid = 0;
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





