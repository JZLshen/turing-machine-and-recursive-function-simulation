#include "widget.h"
#include "ui_widget3.h"

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
    for(int i=0;i<2*InPutList[1]+2;i++)
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
    //OutPt+=QString::number(num);
    ui->IPTEdit->setText(OutPt);
}

void Widget::ShowWPt(int num)
{
    QString OutPt;
    int k=0;
    for(int i=0;i<pow(2,InPutList[1]+1);i++)
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
    //OutPt+=QString::number(num);
    ui->WPTEdit->setText(OutPt);
}


void Widget::ShowOPt(int num)
{
    QString OutPt;
    int k=1;
    num--;
    for(int i=0;i<InPutList[1];i++)
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
    ui->OtpEdit->setText(OutPt);
}

void Widget::ShowOTape()
{
    QString OutPt1;
    int i=0,k=1;
    for(i=0;i<InPutList[1];i++)
    {
        OutPt1+=QString::number(k);
        OutPt1+=": ";
        OutPt1+=QString::number(OutTape[i]);
        OutPt1+="\n";
        k++;
    }
    ui->OutEdit->setText(OutPt1);
}


void Widget::ShowCH()
{
    QString OutPt1;
    int i=0, k=1;
    for(i=0;i<pow(2,InPutList[1]+1);i++)
    {
        OutPt1+=QString::number(i);
        OutPt1+=": v:";
        OutPt1+=QString::number(WorkTapV[i]);
        OutPt1+=" w:";
        OutPt1+=QString::number(WorkTapW[i]);
        OutPt1+="\n";
        k++;
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
    }
    ui->WorkEdit->setText(OutPt);
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

    PreStep=0;
    BestV = 0;
    BestPos = 0;
    int i = 0, n = 0, m = 0;
    int RecStep = 0;
    int TapGrid = 0;
    int TraceBack=0;
    int PtLev=0;
    QString TempInput;
    QString TempOutput;
    ShowPt(2);
    PtLev=0;
    TraceBack=0;
    for(n=0;n<InPutList[1];n++)
    {
        OutTape[n]=0;

    }
    ShowOPt(InPutList[1]);
    ShowOTape();
    for(n=0;n<pow(2,InPutList[1]+1);n++)
    {
        WorkTapV[n]=0;
        WorkTapW[n]=0;
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
        //ui->CharEdit->setText(" ");
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

        NumSort();
        for (int j = 0; j <= i; j++)
        {
            TempOutput+=QString::number(InPutList[j]);
            TempOutput+="\n";
        }
        ui->InputtapeEdit->setText(TempOutput);



        RecBag(1, 0, 0, &RecStep, &TapGrid,&PtLev);
        TempOutput=QString::number(BestV);
        ui->ResEdit->setText(TempOutput);
        TraceBake(&TraceBack, &RecStep);
    }
}

void Widget::TraceBake(int *i, int *RecStep)
{
    QString TempOutput;

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;

    TempOutput="TraceBack:"+QString::number(InPutList[1]-*i);
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);




    if(BestPos%2==0)
    {
        OutTape[InPutList[1]-*i-1]=1;
    }
    else
    {
        OutTape[InPutList[1]-*i-1]=0;
    }


    ShowOPt(InPutList[1]-*i);
    ShowOTape();

    BestPos=BestPos/2;
    (*i)++;
    if(BestPos!=1)
    {
        TraceBake(i,RecStep);
    }
}


void Widget::on_NextStep_clicked()
{
    //ui->ResEdit->setText(QString::number(BestV));
    PreStep++;
}




void Widget::NumSort()
{
    float t = 0;
    int i = 0;
    float RecTap[1000][3];
    int hi = InPutList[1];
    for (i = 1; i <= InPutList[1]; i++)
    {
        RecTap[i][0] = InPutList[2 * i];
        RecTap[i][1] = InPutList[2 * i+1];
    }
    for (i = 1; i <=hi; i++)
    {
        RecTap[i][2] = RecTap[i][1] / RecTap[i][0];
    }
    for (int j = hi+1; j > 1; j--)
    {
        for (i = 1; i < j; i++)
        {
            if (RecTap[i][2] < RecTap[i + 1][2])
            {
                t = RecTap[i][0];
                RecTap[i][0] = RecTap[i + 1][0];
                RecTap[i + 1][0] = t;

                t = RecTap[i][1];
                RecTap[i][1] = RecTap[i + 1][1];
                RecTap[i + 1][1] = t;

                t = RecTap[i][2];
                RecTap[i][2] = RecTap[i + 1][2];
                RecTap[i + 1][2] = t;
            }
        }
    }
    for (i = 1; i <= InPutList[1]; i++)
    {
        InPutList[2 * i] = RecTap[i][0];
        InPutList[2 * i + 1] = RecTap[i][1];
    }
}




void Widget::RecBag(int num, int w, int v, int* RecStep, int * TapGrid, int *PtLev)
{
    int i = 0;
    int wt = 0, vt = 0, sig = 0;
    QString TempOutput;
    (*PtLev)+=2;


    //ui->ResEdit->setText(QString::number(* RecStep));
    ui->StepsEdit->setText(QString::number(* RecStep));






    /*
    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    if(num!=1)
    {
        ShowPt(2*(int)sqrt(num)+2);
    }
    else
    {
        ShowPt(2);
    }
    (*RecStep)++;
    ShowCH();
    TempOutput="ReadW"+QString::number(2*(int)sqrt(num)+2);
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));
    ShowWPt(num);




    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    if(num!=1)
    {
        ShowPt(2*(int)sqrt(num)+3);
    }
    else
    {
        ShowPt(3);
    }
    (*RecStep)++;
    ShowCH();
    TempOutput="Readv"+QString::number(2*(int)sqrt(num)+3);
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));
    ShowWPt(num);
    */




    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    //ShowPt();
    (*RecStep)++;
    ShowCH();
    TempOutput="push fun num:" + QString::number(num)+", w:"+QString::number(w)+" v:"+QString::number(v);
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));
    ShowWPt(num);


    RecStackPt++;
    RecStack[RecStackPt] = "fun num:" + QString::number(num)+", w:"+QString::number(w)+" v:"+QString::number(v);
    OutPutStack();









    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;
    ShowCH();
    TempOutput="TestLeave";
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));
    ShowWPt(num);











    if (pow(2, InPutList[1]) <= num)
    {


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
        ShowWPt(num);


        if (BestV < WorkTapV[num])
        {
            BestV = WorkTapV[num];
            BestPos = num;
        }
    }
    else
    {


        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        if(num!=1)
        {
            ShowPt(*PtLev);
        }
        else
        {
            ShowPt(2);
        }
        (*RecStep)++;
        ShowCH();
        TempOutput="ReadW"+QString::number(*PtLev);//+" num: "+QString::number(num);
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));
        ShowWPt(num);




        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        if(num!=1)
        {
            ShowPt((*PtLev)+1);
        }
        else
        {
            ShowPt(3);
        }
        (*RecStep)++;
        ShowCH();
        TempOutput="ReadV"+QString::number((*PtLev)+1);//+" num: "+QString::number(num);
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));
        ShowWPt(num);



        while((*RecStep)!=PreStep)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        (*RecStep)++;
        ShowCH();
        TempOutput="CulLeft";
        ui->StepsEdit->setText(QString::number((*RecStep)));
        ui->StepNameEdit->setText(TempOutput);
        ui->GridsEdit->setText(QString::number(*TapGrid));
        ShowWPt(num);


        if (num == 1)
        {
            if (w + InPutList[2] <= InPutList[0])
            {
                WorkTapV[2 * num] = v + InPutList[3];
                WorkTapW[2 * num] = w + InPutList[2];


                while((*RecStep)!=PreStep)
                {
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                (*RecStep)++;
                ShowCH();
                TempOutput="Write num:" + QString::number(2*num)+", w:"+QString::number(WorkTapW[2 * num])+" v:"+QString::number(WorkTapV[2 * num]);
                ui->StepsEdit->setText(QString::number((*RecStep)));
                ui->StepNameEdit->setText(TempOutput);
                (*TapGrid)++;
                ui->GridsEdit->setText(QString::number(*TapGrid));
                ShowWPt(2*num);


                RecBag(num * 2, WorkTapW[2 * num], WorkTapV[2 * num], RecStep, TapGrid,PtLev);
            }



            while((*RecStep)!=PreStep)
            {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
            (*RecStep)++;
            ShowCH();
            TempOutput="CulRight";
            ui->StepsEdit->setText(QString::number((*RecStep)));
            ui->StepNameEdit->setText(TempOutput);
            ui->GridsEdit->setText(QString::number(*TapGrid));
            ShowWPt(num);


            wt = w;
            vt = v;
            if (v > BestV && pow(2, InPutList[1]) <= 2 * num)
            {
                sig = 1;
            }
            for (i = 1; i < InPutList[1]; i++)
            {
                if (wt + InPutList[2 * 0 + 2 + 2 * i] < InPutList[0])
                {
                    wt += InPutList[2 * 0 + 2 + 2 * i];
                    vt += InPutList[2 * 0 + 2 + 2 * i + 1];
                }
                else
                {
                    if ((InPutList[0] - wt) * (InPutList[2 * 0 + 2 + 2 * i + 1] / InPutList[2 * 0 + 2 + 2 * i]) + vt > BestV)
                    {
                        sig = 1;
                    }
                }
            }
            if (sig == 1)
            {
                WorkTapV[2 * num + 1] = v;
                WorkTapW[2 * num + 1] = w;


                while((*RecStep)!=PreStep)
                {
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                (*RecStep)++;
                ShowCH();
                TempOutput="Write num:" + QString::number(2*num+1)+", w:"+QString::number(WorkTapW[2 * num+1])+" v:"+QString::number(WorkTapV[2 * num+1]);
                ui->StepsEdit->setText(QString::number((*RecStep)));
                ui->StepNameEdit->setText(TempOutput);
                (*TapGrid)++;
                ui->GridsEdit->setText(QString::number(*TapGrid));
                ShowWPt(2*num+1);


                RecBag(num * 2 + 1, WorkTapW[2 * num-1], WorkTapV[2 * num+1], RecStep, TapGrid,PtLev);
            }
        }
        else
        {


            while((*RecStep)!=PreStep)
            {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
            (*RecStep)++;
            ShowCH();
            TempOutput="CulLeft";
            ui->StepsEdit->setText(QString::number((*RecStep)));
            ui->StepNameEdit->setText(TempOutput);
            ui->GridsEdit->setText(QString::number(*TapGrid));
            ShowWPt(num);


            if (w + InPutList[(*PtLev)] <= InPutList[0])
            {
                WorkTapV[2 * num] = v + InPutList[(*PtLev)+1];
                WorkTapW[2 * num] = w + InPutList[(*PtLev)];


                while((*RecStep)!=PreStep)
                {
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                (*RecStep)++;
                ShowCH();
                TempOutput="Write num:" + QString::number(2*num)+", w:"+QString::number(WorkTapW[2 * num])+" v:"+QString::number(WorkTapV[2 * num]);
                ui->StepsEdit->setText(QString::number((*RecStep)));
                ui->StepNameEdit->setText(TempOutput);
                (*TapGrid)++;
                ui->GridsEdit->setText(QString::number(*TapGrid));
                ShowWPt(2*num);


                RecBag(num * 2, WorkTapW[2 * num], WorkTapV[2 * num], RecStep, TapGrid, PtLev);
            }


            while((*RecStep)!=PreStep)
            {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
            (*RecStep)++;
            ShowCH();
            TempOutput="CulRight";
            ui->StepsEdit->setText(QString::number((*RecStep)));
            ui->StepNameEdit->setText(TempOutput);
            ui->GridsEdit->setText(QString::number(*TapGrid));
            ShowWPt(num);

            //ui->ResEdit->setText(QString::number(BestV));

            wt = w;
            vt = v;
            if (v > BestV&&pow(2, InPutList[1]) <= 2*num)
            {
                sig = 1;
            }
            for (i = 1; 2*i+(*PtLev) < 2*InPutList[1]+2; i++)
            {
                if (wt + InPutList[(*PtLev)+2*i] <= InPutList[0])
                {
                    wt += InPutList[(*PtLev)+2*i];
                    vt += InPutList[(*PtLev)+2*i+1];
                    if ((2*(i+1) == 2*InPutList[1] - (*PtLev))&&(vt>BestV))
                    {
                        sig = 1;
                    }
                }
                else
                {
                    vt += (InPutList[0] - wt) * (InPutList[(*PtLev) + 2 * i + 1] / (float)InPutList[(*PtLev) + 2 * i]);
                    if (vt > BestV)
                    {
                        sig = 1;
                    }
                    break;
                }
            }
            if (vt > BestV)
            {
                sig = 1;
            }
            else
            {
                sig=0;
            }
            ui->ResEdit->setText(QString::number(vt));
            if (sig == 1)
            {
                WorkTapV[2 * num + 1] = v;
                WorkTapW[2 * num + 1] = w;


                while((*RecStep)!=PreStep)
                {
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                }
                (*RecStep)++;
                ShowCH();
                TempOutput="Write num:" + QString::number(2*num+1)+", w:"+QString::number(WorkTapW[2 * num+1])+" v:"+QString::number(WorkTapV[2 * num+1]);
                ui->StepsEdit->setText(QString::number((*RecStep)));
                ui->StepNameEdit->setText(TempOutput);
                (*TapGrid)++;
                ui->GridsEdit->setText(QString::number(*TapGrid));
                ShowWPt(2*num+1);


                RecBag(num * 2 + 1, WorkTapW[2 * num + 1], WorkTapV[2 * num + 1], RecStep, TapGrid,PtLev);
            }
        }
    }

    while((*RecStep)!=PreStep)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    (*RecStep)++;
    ShowCH();
    TempOutput="pop fun num:" + QString::number(num)+", w:"+QString::number(w)+" v:"+QString::number(v);
    ui->StepsEdit->setText(QString::number((*RecStep)));
    ui->StepNameEdit->setText(TempOutput);
    ui->GridsEdit->setText(QString::number(*TapGrid));
    ShowWPt(num);


    RecStackPt--;
    OutPutStack();
    (*PtLev)-=2;
}

















