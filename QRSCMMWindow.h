//QRSCMMWindow.h

#ifndef QRSCMMWINDOW_H
#define QRSCMMWINDOW_H

#include "CMsgMgr.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QResizeEvent>
#include <QAction>

namespace Ui
{
    class QRSCMMWindow;
}

class QRSCMMWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QRSCMMWindow(QWidget* parent = 0);
    ~QRSCMMWindow();
protected:
    void resizeEvent(QResizeEvent* event);
private slots:
    void OpenFile();
    void Clear();
    void Exit();
    void PrintAll();
    void SearchNumber();
    void SearchName();
    void SearchPeriod();
    void SearchOBPcrs();
    void SearchKP();
    void Search();
    void PacketSearch();
    void Sort();
    void Fix();
    void Help();
private:
    Ui::QRSCMMWindow* ui;
    void ActionControl(bool Flag);
    static bool sortmy(const KPMsg &n1, const KPMsg &n2)
     {
         return n1.period > n2.period;
     }
    CMsgMgr MsgMgr;
    QTableWidget* m_View_Table;
    QAction* m_OpenFile_Action;
    QAction* m_Clear_Action;
    QAction* m_Exit_Action;
    QAction* m_PrintAll_Action;
    QAction* m_SearchNumber_Action;
    QAction* m_SearchName_Action;
    QAction* m_SearchPeriod_Action;
    QAction* m_SearchOBPcrs_Action;
    QAction* m_SearchKP_Action;
    QAction* m_Search_Action;
    QAction* m_PacketSearch_Action;
    QAction* m_Fix_Action;
    QAction* m_Sort_Action;
    QAction* m_Help_Action;
};

#endif // QRSCMMWINDOW_H
