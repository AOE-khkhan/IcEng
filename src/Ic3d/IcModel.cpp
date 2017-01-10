/*
 *  icube_model.cpp
 *  B_O_Britain
 *
 *  Created by Sherman Chen on 2/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "Ic3d.h"

namespace Ic3d
{
	using namespace std;
	using namespace ctl;
	//-----------------------------------------------------------
	bool IcModel::loadFile(const string& sFile)
	{
		bool isOK = true;
		IcModelLoader loader;
		loader.loadFile(sFile);
		for(auto pMdl : loader.m_models.getAry())
			m_childModels.add(pMdl);
		m_sName = ctl::sFileNoPath(sFile);
		return isOK;
	}
	
	//-----------------------------------------------------------
	void IcModel::draw() const
	{
		//---- Draw itself
		if(m_pMat!=nullptr) m_pMat->draw();
		if(m_pTex!=nullptr) m_pTex->draw();
		if(m_pMesh!=nullptr) m_pMesh->draw();
		
		//---- Draw Sub-Model
		for(auto pMdl : m_childModels.all())
			pMdl->draw();
	}

    //--------------------------------
    //  createCube
    //--------------------------------
    ctl::Sp<const IcModel> IcModel::createCube(const TVec3& sz,
                                               const TVec3& ofst,
                                               const TColor& color)
    {
        IcMeshData mshd; mshd.createCube(sz, ofst);
        auto pMat = makeSp<IcMaterial>(color);
        auto pModel = makeSp<IcModel>(mshd);
        pModel->setMaterial(pMat);
        return pModel;
    }

} // namespace Ic3d

