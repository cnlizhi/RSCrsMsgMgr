//QRSCMMWindow.cpp

#include "QRSCMMWindow.h"
#include "ui_QRSCMMWindow.h"
#include <QMenu>
#include <QStringList>
#include <QLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <QDebug>

const double EPS = 0.000001;

QRSCMMWindow::QRSCMMWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::QRSCMMWindow)
{
    ui->setupUi(this);
    setWindowTitle("Remote Sensing Courses Massage Manager Program");
    this->resize(1200, 742);
    QMenu* File_Menu = menuBar()->addMenu(tr("&File"));
    m_OpenFile_Action = File_Menu->addAction(tr("OpenFile"), this, &QRSCMMWindow::OpenFile, Qt::Key_O);
    m_Clear_Action = File_Menu->addAction(tr("Clear"), this, &QRSCMMWindow::Clear, Qt::Key_C);
    m_Exit_Action = File_Menu->addAction(tr("Exit"), this, &QRSCMMWindow::Exit, Qt::Key_X);
    QMenu* Tool_Menu = menuBar()->addMenu(tr("&Tool"));
    m_PrintAll_Action = Tool_Menu->addAction(tr("PrintAll"), this, &QRSCMMWindow::PrintAll);
    m_SearchNumber_Action = Tool_Menu->addAction(tr("SearchNumber"), this, &QRSCMMWindow::SearchNumber);
    m_SearchName_Action = Tool_Menu->addAction(tr("SearchName"), this, &QRSCMMWindow::SearchName);
    m_SearchPeriod_Action = Tool_Menu->addAction(tr("SearchPeriod"), this, &QRSCMMWindow::SearchPeriod);
    m_SearchOBPcrs_Action = Tool_Menu->addAction(tr("SearchOBPcrs"), this, &QRSCMMWindow::SearchOBPcrs);
    m_SearchKP_Action = Tool_Menu->addAction(tr("SearchKP"), this, &QRSCMMWindow::SearchKP);
    m_Search_Action = Tool_Menu->addAction(tr("Search"), this, &QRSCMMWindow::Search);
    m_PacketSearch_Action = Tool_Menu->addAction(tr("PacketSearch"), this, &QRSCMMWindow::PacketSearch);
    m_Sort_Action = Tool_Menu->addAction(tr("Sort"), this, &QRSCMMWindow::Sort);
    m_Fix_Action = Tool_Menu->addAction(tr("Fix"), this, &QRSCMMWindow::Fix);
    QMenu* Help_Menu = menuBar()->addMenu(tr("&Help"));
    m_Help_Action = Help_Menu->addAction(tr("Help"), this, &QRSCMMWindow::Help);
    ActionControl(false);
    m_Exit_Action->setEnabled(true);
    m_Help_Action->setEnabled(true);
    m_View_Table = new QTableWidget(30, 5, this);
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    m_View_Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_View_Table->move(0, 45);
    m_View_Table->resize(this->width(), this->height() - 45);
    for (int i = 0; i < 5; i++)
    {
        m_View_Table->setColumnWidth(i, this->width() / 5);
    }
    m_View_Table->show();
}

QRSCMMWindow::~QRSCMMWindow()
{
    delete ui;
}

void QRSCMMWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    m_View_Table->resize(this->width(), this->height() - 45);
    for (int i = 0; i < 5; i++)
    {
        m_View_Table->setColumnWidth(i, this->width() / 5);
    }
    m_View_Table->show();
}

void QRSCMMWindow::OpenFile()
{

    QStringList path = QFileDialog::getOpenFileNames
            (this, tr("Please Choose Text"), "D:", tr("文本文件(* txt)"));
    for(QStringList::iterator iter = path.begin(); iter != path.end(); iter++)
    {
        QString qstr = *iter;
        if (!MsgMgr.Insert(qstr))
        {
            QErrorMessage* Dialog = new QErrorMessage(this);
            Dialog->setWindowTitle(tr("Error!"));
            Dialog->showMessage(tr("Open File Failed."));
        }
        else
        {
            PrintAll();
        }
    }
    MsgMgr.Packet();
    ActionControl(true);
}

void QRSCMMWindow::Clear()
{
    MsgMgr.Clear();
    m_View_Table->clear();
    ActionControl(false);
}

void QRSCMMWindow::Exit()
{
    this->close();
}

void QRSCMMWindow::PrintAll()
{
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    vector<CrsMsg> Vec = MsgMgr.getCrsMsg();
    unsigned count = 0;
    for (vector<CrsMsg>::iterator iter = Vec.begin(); iter != Vec.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
        m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
        m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
        m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
        m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
        count++;
    }
}

void QRSCMMWindow::SearchNumber()
{
    int value = QInputDialog::getInt(this, tr("Search"),tr("Please enter course number"));
    vector<CrsMsg>Vec = MsgMgr.getCrsMsg();
    unsigned count = 0;
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    for (vector<CrsMsg>::iterator iter = Vec.begin(); iter != Vec.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        if (value == crsmsg.number)
        {
            m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
            m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
            m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
            m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
            m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
            count++;
        }
    }
}

void QRSCMMWindow::SearchName()
{
    QString value = QInputDialog::getText(this, tr("Search"),tr("Please enter course's name"));
    vector<CrsMsg>Vec = MsgMgr.getCrsMsg();
    unsigned count = 0;
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    for (vector<CrsMsg>::iterator iter = Vec.begin(); iter != Vec.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        if (value == crsmsg.name)
        {
            m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
            m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
            m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
            m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
            m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
            count++;
        }
    }
}

void QRSCMMWindow::SearchPeriod()
{
    double value = QInputDialog::getDouble(this, tr("Search"),tr("Please enter course's period"));
    vector<CrsMsg>Vec = MsgMgr.getCrsMsg();
    unsigned count = 0;
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    for (vector<CrsMsg>::iterator iter = Vec.begin(); iter != Vec.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        if (value > crsmsg.period - EPS && value < crsmsg.period + EPS)
        {
            m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
            m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
            m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
            m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
            m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
            count++;
        }
    }
}

void QRSCMMWindow::SearchOBPcrs()
{
    QString value = QInputDialog::getText(this, tr("Search"),tr("Please enter course's OBP course"));
    vector<CrsMsg>Vec = MsgMgr.getCrsMsg();
    unsigned count = 0;
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    for (vector<CrsMsg>::iterator iter = Vec.begin(); iter != Vec.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        if (value == crsmsg.OBPcrs)
        {
            m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
            m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
            m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
            m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
            m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
            count++;
        }
    }
}

void QRSCMMWindow::SearchKP()
{
    QString value = QInputDialog::getText(this, tr("Search"),tr("Please enter course's Knowledge Point"));
    vector<CrsMsg>Vec1 = MsgMgr.getCrsMsg();
    vector<KPMsg>Vec2 = MsgMgr.getKPMsg();
    unsigned count = 0;
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    for (vector<KPMsg>::iterator iter1 = Vec2.begin(); iter1 != Vec2.end(); iter1++)
    {
        KPMsg kpmsg = *iter1;
        if (value == kpmsg.text)
        {
            for (vector<CrsMsg>::iterator iter2 = Vec1.begin(); iter2 != Vec1.end(); iter2++)
            {
                CrsMsg crsmsg = *iter2;
                if (kpmsg.crsnumber == crsmsg.number)
                {
                    m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
                    m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
                    m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
                    m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
                    m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
                    count++;
                }
            }
        }
    }
}

void QRSCMMWindow::Search()
{
    int value1 = QInputDialog::getInt(this, tr("Search"),tr("Please enter course number"));
    QString value2 = QInputDialog::getText(this, tr("Search"),tr("Please enter course's name"));
    double value3 = QInputDialog::getDouble(this, tr("Search"),tr("Please enter course's period"));
    QString value4 = QInputDialog::getText(this, tr("Search"),tr("Please enter course's OBP course"));
    vector<CrsMsg>Vec = MsgMgr.getCrsMsg();
    unsigned count = 0;
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    for (vector<CrsMsg>::iterator iter = Vec.begin(); iter != Vec.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        if ((value1 == crsmsg.number) && (value2 == crsmsg.name) &&
            (value3 > crsmsg.period - EPS && value3 < crsmsg.period + EPS) && (value4 == crsmsg.OBPcrs))
        {
            m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
            m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
            m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
            m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
            m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
            count++;
        }
    }
}

void QRSCMMWindow::PacketSearch()
{
    int value = QInputDialog::getInt(this, tr("Packet Search"),tr("Please enter course number"));
    vector< vector<CrsMsg> > vecpacket = MsgMgr.getPacket();
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << "OBPCourse" << "KnowledgePoints";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    for (vector< vector<CrsMsg> >::iterator iter1 = vecpacket.begin(); iter1 != vecpacket.end(); iter1++)
    {
        bool isFind = false;
        vector<CrsMsg> Vec = *iter1;
        for (vector<CrsMsg>::iterator iter2 = Vec.begin(); iter2 != Vec.end() && !isFind; iter2++)
        {
            CrsMsg crsmsg = *iter2;
            if (crsmsg.number == value)
            {
                isFind = true;
            }
        }
        if (isFind)
        {
            unsigned count = 0;
            for (vector<CrsMsg>::iterator iter2 = Vec.begin(); iter2 != Vec.end(); iter2++)
            {
                CrsMsg crsmsg = *iter2;
                m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(crsmsg.number)));
                m_View_Table->setItem(count, 1, new QTableWidgetItem(crsmsg.name));
                m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(crsmsg.period)));
                m_View_Table->setItem(count, 3, new QTableWidgetItem(crsmsg.OBPcrs));
                m_View_Table->setItem(count, 4, new QTableWidgetItem(QString::number(crsmsg.klgnum)));
                count++;
            }
        }
    }
}

void QRSCMMWindow::Sort()
{
    int value = QInputDialog::getInt(this, tr("Packet Search"),tr("Please enter course number"));
    vector<CrsMsg> veccrsmsg;
    vector< vector<CrsMsg> > vecpacket = MsgMgr.getPacket();
    for (vector< vector<CrsMsg> >::iterator iter1 = vecpacket.begin(); iter1 != vecpacket.end(); iter1++)
    {
        bool isFind = false;
        vector<CrsMsg> Vec = *iter1;
        for (vector<CrsMsg>::iterator iter2 = Vec.begin(); iter2 != Vec.end() && !isFind; iter2++)
        {
            CrsMsg crsmsg = *iter2;
            if (crsmsg.number == value)
            {
                isFind = true;
            }
        }
        if (isFind)
        {
            for (vector<CrsMsg>::iterator iter2 = Vec.begin(); iter2 != Vec.end(); iter2++)
            {
                CrsMsg crsmsg = *iter2;
                veccrsmsg.push_back(crsmsg);
            }
        }
    }
    vector<KPMsg> veckpmsg = MsgMgr.getKPMsg();
    vector<KPMsg> veckpmsg1;
    for (vector<CrsMsg>::iterator iter1 = veccrsmsg.begin(); iter1 != veccrsmsg.end(); iter1++)
    {
        CrsMsg crsmsg = *iter1;
        for (vector<KPMsg>::iterator iter2 = veckpmsg.begin(); iter2 != veckpmsg.end(); iter2++)
        {
            KPMsg kpmsg = *iter2;
            if (crsmsg.number == kpmsg.crsnumber)
            {
                veckpmsg1.push_back(kpmsg);
            }
        }
    }
    vector<KPMsg> veckpmsg2;
    for (vector<KPMsg>::iterator iter1 = veckpmsg1.begin(); iter1 != veckpmsg1.end(); iter1++)
    {
        bool isOnce = true;
        KPMsg kpmsg = *iter1;
        for (vector<KPMsg>::iterator iter2 = iter1 + 1; iter2 != veckpmsg1.end(); iter2++)
        {
            KPMsg index = *iter2;
            if (kpmsg.text == index.text)
            {
                kpmsg.period += index.period;
                isOnce = false;
            }
        }
        bool isFind = false;
        for (vector<KPMsg>::iterator iter2 = veckpmsg2.begin(); iter2 != veckpmsg2.end() && !isFind; iter2++)
        {
            KPMsg index = *iter2;
            if (kpmsg.text == index.text)
            {
                isFind = true;
            }
        }
        if (!isFind && !isOnce)
        {
            veckpmsg2.push_back(kpmsg);
        }
    }
    std::sort(veckpmsg2.begin(), veckpmsg2.end(), sortmy);
    m_View_Table->clear();
    QStringList ColLabs;
    ColLabs << "Number" << "Name" << "Period" << " " << " ";
    m_View_Table->setHorizontalHeaderLabels(ColLabs);
    unsigned count = 0;
    for (vector<KPMsg>::iterator iter = veckpmsg2.begin(); iter != veckpmsg2.end(); iter++)
    {
        KPMsg kpmsg = *iter;
        m_View_Table->setItem(count, 0, new QTableWidgetItem(QString::number(kpmsg.crsnumber)));
        m_View_Table->setItem(count, 1, new QTableWidgetItem(kpmsg.text));
        m_View_Table->setItem(count, 2, new QTableWidgetItem(QString::number(kpmsg.period)));
        count++;
    }
}

void QRSCMMWindow::Fix()
{
    MsgMgr.Change();
    MsgMgr.Packet();
    PrintAll();
}

void QRSCMMWindow::Help()
{
    QMessageBox::about(this, tr("About Remote Sensing Courses Message Manager"),
            tr("<p><b>Remote Sensing Courses Message Manager Program</b></p>"
               "<p>by Lizhi</p>"
               "<p>Date 2019.1.10</p>"
               "<p>遥感课程信息管理程序，其功能菜单如下：</p>"
               "<p>00   Exit          退出程序</p>"
               "<p>01   Open File     打开课程信息</p>"
               "<p>02   Close         关闭现有文件</p>"
               "<p>03   Print All     显示所有课程信息</p>"
               "<p>04   Search Number 按课程代码搜索</p>"
               "<p>05   Search Name   按课程名称搜索</p>"
               "<p>06   Search Period 按课程学时搜索</p>"
               "<p>07   Search OBPcrs 按先导课程名称搜索</p>"
               "<p>08   Search KP     按知识点名称搜索</p>"
               "<p>09   Search        综合搜索</p>"
               "<p>10   Packet Search 搜索分组</p>"
               "<p>11   Sort          组别重复知识点排序</p>"
               "<p>12   Fix           修正课程信息</p>"
               "<p>13   Help          帮助界面</p>"));
}

void QRSCMMWindow::ActionControl(bool isOpen)
{
    m_Clear_Action->setEnabled(isOpen);
    m_PacketSearch_Action->setEnabled(isOpen);
    m_SearchNumber_Action->setEnabled(isOpen);
    m_SearchName_Action->setEnabled(isOpen);
    m_SearchPeriod_Action->setEnabled(isOpen);
    m_SearchOBPcrs_Action->setEnabled(isOpen);
    m_SearchKP_Action->setEnabled(isOpen);
    m_Sort_Action->setEnabled(isOpen);
    m_Fix_Action->setEnabled(isOpen);
}


