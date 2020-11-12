//CMsgMgr.cpp

#include "CMsgMgr.h"
#include "BST.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QTextCodec>
#include <cstring>
#include <QDebug>

CMsgMgr::CMsgMgr()
{
    Clear();
}

CMsgMgr::~CMsgMgr()
{
    Clear();
}

bool CMsgMgr::Insert(QString path)
{
    if (path == NULL)
    {
        return false;
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    QTextStream textstream(&file);
    QTextCodec* codec = QTextCodec::codecForName("GBK");
    textstream.setCodec(codec);
    CrsMsg newCrsMsg;
    newCrsMsg.number = textstream.readLine().toUInt();
    newCrsMsg.name = textstream.readLine();
    newCrsMsg.period = textstream.readLine().toFloat();
    newCrsMsg.OBPcrs = textstream.readLine();
    newCrsMsg.klgnum = textstream.readLine().toUInt();
    float klgperiod = newCrsMsg.period / newCrsMsg.klgnum;
    while (!textstream.atEnd())
    {
        KPMsg newKPMsg(textstream.readLine(), klgperiod, newCrsMsg.number);
        KPMsg* pointer = &newKPMsg;
        newCrsMsg.vec_klg.push_back(pointer);
        m_Vec_KPMsg.push_back(newKPMsg);
    }
    m_Vec_CrsMsg.push_back(newCrsMsg);
    file.close();
    return true;
}

void CMsgMgr::Clear()
{
    m_Vec_CrsMsg.clear();
    m_Vec_KPMsg.clear();
}

vector< vector<CrsMsg> > CMsgMgr::getPacket() const
{
    return m_VecVec_Packet;
}

vector<CrsMsg> CMsgMgr::getCrsMsg() const
{
    return m_Vec_CrsMsg;
}

vector<KPMsg> CMsgMgr::getKPMsg() const
{
    return m_Vec_KPMsg;
}

void CMsgMgr::Packet()
{
    m_VecVec_Packet.clear();
    vector<CrsMsg>vecroot;
    for (vector<CrsMsg>::iterator iter = m_Vec_CrsMsg.begin(); iter != m_Vec_CrsMsg.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        if (crsmsg.OBPcrs.size() == 0)
        {
            vecroot.push_back(crsmsg);
        }
    }
    for (vector<CrsMsg>::iterator iter = vecroot.begin(); iter != vecroot.end(); iter++)
    {
        vector<CrsMsg>Vec;
        CrsMsg crsmsg = *iter;
        Vec.push_back(crsmsg);
        SetPacket(crsmsg.name, Vec);
        m_VecVec_Packet.push_back(Vec);
    }
}

void CMsgMgr::Change()
{
    vector<CrsMsg> newveccrsmsg;
    vector<KPMsg> newveckpmsg;
    for (vector<KPMsg>::iterator iter1 = m_Vec_KPMsg.begin(); iter1 != m_Vec_KPMsg.end(); iter1++)
    {
        KPMsg kpmsg = *iter1;
        bool isFind = false;
        for (vector<KPMsg>::iterator iter2 = newveckpmsg.begin(); iter2 != newveckpmsg.end() && !isFind; iter2++)
        {
            KPMsg index = *iter2;
            if (kpmsg.text == index.text)
            {
                isFind = true;
            }
        }
        if (!isFind)
        {
            newveckpmsg.push_back(kpmsg);
        }
    }
    for (vector<CrsMsg>::iterator iter1 = m_Vec_CrsMsg.begin(); iter1 != m_Vec_CrsMsg.end(); iter1++)
    {
        CrsMsg crsmsg = *iter1;
        float newperiod = 0;
        int sum = 0;
        for (vector<KPMsg>::iterator iter2 = newveckpmsg.begin(); iter2 != newveckpmsg.end(); iter2++)
        {
            KPMsg kpmsg = *iter2;
            if (kpmsg.crsnumber == crsmsg.number)
            {
                newperiod += kpmsg.period;
                sum++;
            }
        }
        crsmsg.period = newperiod;
        crsmsg.klgnum = sum;
        newveccrsmsg.push_back(crsmsg);
    }
    m_Vec_CrsMsg = newveccrsmsg;
    m_Vec_KPMsg = newveckpmsg;
}

void CMsgMgr::SetPacket(QString name, vector<CrsMsg>& r)
{
    vector<CrsMsg>Vec;
    for (vector<CrsMsg>::iterator iter = m_Vec_CrsMsg.begin(); iter != m_Vec_CrsMsg.end(); iter++)
    {
        CrsMsg crsmsg = *iter;
        if (name == crsmsg.OBPcrs)
        {
            r.push_back(*iter);
            Vec.push_back(*iter);
        }
    }
    if (!Vec.empty())
    {
        for (vector<CrsMsg>::iterator iter = Vec.begin(); iter != Vec.end(); iter++)
        {
            CrsMsg crsmsg = *iter;
            SetPacket(crsmsg.name, r);
        }
    }
}
