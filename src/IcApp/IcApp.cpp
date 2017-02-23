//
//  IcApp.cpp
//  IcEngLib
//
//  Created by Sherman Chen on 2/14/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//


#include "Ic3d.h"

namespace Ic3d
{
    using namespace ctl;
    using namespace std;
    
    static IcApp* l_pIcAppInstance = nullptr;
    //----------------------------
    //  IcApp
    //----------------------------
    IcApp::IcApp()
    {
      //  setInstance(this);
    };
    //----------------------------
    //  singleton
    //----------------------------
    void IcApp::setInstance(IcApp* pApp)
    {
        //---- Set as default IcApp
        if(l_pIcAppInstance!=nullptr)
            logWarn("IcApp::IcApp() : In most case there should be only one IcApp instanciation");
       l_pIcAppInstance = pApp;
    }
    IcApp* IcApp::getInstance()
    {
        return l_pIcAppInstance;
    }

    //----------------------------
    //  addWindow
    //----------------------------
    void IcApp::addWindow(ctl::Sp<IcWindow> pWin)
    {
        auto pMng = IcWinMng::getInstance();
        pMng->addWindow(pWin);
    }
    //----------------------------
    //  onScreenSize
    //----------------------------
    ctl::Sp<IcWindow> IcApp::getWindow(int idx)
    {
        auto pMng = IcWinMng::getInstance();
        return pMng->getWindow(idx);
    }

    //----------------------------
    //  onScreenSize
    //----------------------------
    void IcApp::onScreenSize(const ctl::TSize& sz)
    {
        auto pMng = IcWinMng::getInstance();
        pMng->onScreenSize(sz);
    }
    //----------------------------
    //  initWithScn
    //----------------------------
    void IcApp::initWithScn(ctl::Sp<IcScene> pScn)
    {
        auto pWin = makeSp<IcWindow>();
        pWin->addScene(pScn);
        addWindow(pWin);
    }
    //----------------------------------
    //  windows
    //----------------------------------
    void IcApp::drawUpdateWindows(float deltaT)
    {
        // TODO: IcWinMng into IcApp
        auto pMng = Ic3d::IcWinMng::getInstance();
        pMng->drawUpdate(deltaT);
       
    }
    void IcApp::initWindows()
    {
        auto pMng = Ic3d::IcWinMng::getInstance();
        pMng->initWindows();
      
    }
  
    

}
