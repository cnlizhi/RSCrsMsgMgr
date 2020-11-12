//CrsMsg.h

#ifndef _CRSMSG_H
#define _CRSMSG_H

#include <QString>
#include <vector>
using namespace std;

struct KPMsg
{
    KPMsg();
    KPMsg(QString str, float value, unsigned no);
    QString text;
    float period;
    unsigned crsnumber;
};

struct CrsMsg
{
    CrsMsg();
    CrsMsg(const CrsMsg &r);
    ~CrsMsg();
    void Clear();
    unsigned number;
    QString name;
    float period;
    QString OBPcrs;
    unsigned klgnum;
    vector<KPMsg*>vec_klg;
};

#endif
