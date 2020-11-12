//CMsgMgr.h

#ifndef _CMSGMGR_H
#define _CMSGMGR_H

#include "CrsMsg.h"
#include <QString>
#include <vector>
#include <string>
using namespace std;

class CMsgMgr
{
public:
    CMsgMgr();
    ~CMsgMgr();
    bool Insert(QString path);
    void Clear();
    vector< vector<CrsMsg> > getPacket() const;
    vector<CrsMsg> getCrsMsg() const;
    vector<KPMsg> getKPMsg() const;
    void Packet();
    void Change();
protected:
    void SetPacket(QString name, vector<CrsMsg>& r);
private:
    vector< vector<CrsMsg> >m_VecVec_Packet;
    vector<CrsMsg>m_Vec_CrsMsg;
    vector<KPMsg>m_Vec_KPMsg;
};

#endif
