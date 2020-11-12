//CrsMsg.cpp

#include "CrsMsg.h"

KPMsg::KPMsg()
{
    text.clear();
    period = 0.f;
    crsnumber = 0;
}

KPMsg::KPMsg(QString str, float value, unsigned no)
{
    text = str;
    period = value;
    crsnumber = no;
}

CrsMsg::CrsMsg()
{
    Clear();
}

CrsMsg::CrsMsg(const CrsMsg &r)
{
    number = r.number;
    name = r.name;
    period = r.period;
    OBPcrs = r.OBPcrs;
    klgnum = r.klgnum;
    vec_klg = r.vec_klg;
}

CrsMsg::~CrsMsg()
{
    Clear();
}

void CrsMsg::Clear()
{
    number = 0;
    name.clear();
    period = 0.f;
    OBPcrs.clear();
    klgnum = 0;
    vec_klg.clear();
}
