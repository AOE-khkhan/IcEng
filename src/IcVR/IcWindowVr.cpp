//
//  IcWindowVr.cpp
//  DevEng
//
//  Created by Sherman Chen on 12/23/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//

#include "Ic3d.h"
#include "IcSceneVr.h"
inline namespace Ic3d {
    using namespace ctl;
    using namespace std;

    const static TEuler K_mouseCoef{0.02,0.02,0.02};
    //---------------------------------------
    //  IcWindowVr::IcWindowVr
    //---------------------------------------
    IcWindowVr::IcWindowVr()
    {
        auto pScn0 = makeSp<IcSceneVr>(true);
        auto pScn1 = makeSp<IcSceneVr>(false);
        addScene(pScn0);
        addScene(pScn1);
        m_vrScn[0] = pScn0;
        m_vrScn[1] = pScn1;
    }
    //---------------------------------------
    //  IcWindowVr::setRootObj
    //---------------------------------------
    void IcWindowVr::setRootObj(Sp<IcObject> pObj)
    {
        m_pRootObj = pObj;
        if(m_scnAry.size()<2) return;   // TODO: assert
        for(int i=0;i<2;i++)
        {
            auto pScn = m_scnAry[i];
            if(pScn==nullptr) return;
            pScn->clearObjs();
            pScn->addObj(pObj);
            
        }
    }
    //---------------------------------------
    //  IcWindowVr::updateVrFogPara
    //---------------------------------------
    void IcWindowVr::updateVrFogPara()
    {
        m_vrScn[0]->m_cfg.m_fogPara = m_vrCfg.m_fogPara;
        m_vrScn[1]->m_cfg.m_fogPara = m_vrCfg.m_fogPara;
        
    }

    //---------------------------------------
    //  IcWindowVr::onInit
    //---------------------------------------
    void IcWindowVr::onInit()
    {
        IcWindow::onInit();
         
        setRootObj(m_pRootObj);
        //---- Copy Cfg

        m_vrScn[0]->m_cfg.m_camCfg = m_vrCfg.m_camCfg;
        m_vrScn[1]->m_cfg.m_camCfg = m_vrCfg.m_camCfg;
    }
    //---------------------------------------
    //  updateVrCamPos/updateVrCamRot
    //---------------------------------------
    void IcWindowVr::updateVrCamPos(const TVec3& pos)
    {
        auto& stts = m_vrStts;
        stts.m_camPos = pos;
        m_vrScn[0]->updateCam(stts.m_camPos, stts.m_camRot);
        m_vrScn[1]->updateCam(stts.m_camPos, stts.m_camRot);
       
    }
    void IcWindowVr::updateVrCamRot(const TQuat& rot)
    {
        auto& stts = m_vrStts;
        stts.m_camRot = rot;
        m_vrScn[0]->updateCam(stts.m_camPos, stts.m_camRot);
        m_vrScn[1]->updateCam(stts.m_camPos, stts.m_camRot);
        
    }

    //---------------------------------------
    //  onCamAttitude
    //---------------------------------------
    /*
    void IcWindowVr::onCamAttitude(float pitch, float roll, float yaw)
    {
        const Ic3d::TQuat K_rotPre(0,0,0,1);
        Ic3d::TVec3 e1 = { roll, -pitch, yaw  };
        Ic3d::TQuat qInv(TVec3(deg2rad(-90),0,0));
        Ic3d::TQuat q(e1);
        q = qInv * q;
        setCamRot(q);
    }
     */
    //---------------------------------------
    //  IcWindowVr::onMouseMove
    //---------------------------------------
    void IcWindowVr::onMouseMove(int x, int y)
    {
        IcWindow::onMouseMove(x, y);
        //---- Use mouse simulate camera attitude
        auto q = m_mouseHelper.onMouseMove(x, y);
        updateVrCamRot(q);
    }
   
    
    //---------------------------------------
    //  IcWindowVr::onWindowSize
    //---------------------------------------
    void IcWindowVr::onWindowSize(const ctl::TSize& winSize)
    {
        auto sz = winSize;
        sz.w = sz.w/2;
        TRect rect[2];
        rect[0] = TRect(0,0, sz.w, sz.h);
        rect[1] = TRect(sz.w,0, sz.w, sz.h);
        for(int i=0;i<2;i++)
        {
            auto pScn = m_scnAry[i];
            pScn->onViewRect(rect[i]);
        }
    }
    //---------------------------------------
    //  IcWindowVr::CMouseHelper
    //---------------------------------------
    TQuat IcWindowVr::CMouseHelper::onMouseMove(int x, int y)
    {
     //   ctl::logDbg(toStr(TVec2(x,y)));
        TVec2 pos(x,y);
        if(m_isFirst) m_mousePrevPos = pos;
        m_isFirst = false;
        auto dPos = pos - m_mousePrevPos;
        m_camAtt.p += dPos.y * K_mouseCoef.p;
        m_camAtt.y += dPos.x * K_mouseCoef.y;
        //---- Pitch in +/-90 degree
        dClamp<decltype(m_camAtt.p)>(m_camAtt.p, -M_PI/2.0, M_PI/2.0);
        m_mousePrevPos = pos;
        return m_camAtt.toQuat();
    }
    
}
