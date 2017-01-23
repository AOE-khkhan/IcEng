//
//  IcMeshData.cpp
//  DevEng
//
//  Created by Sherman Chen on 10/24/16.
//  Copyright © 2016 Sherman Chen. All rights reserved.
//


#include "Ic3d.h"

namespace Ic3d{
    using namespace std;
    using namespace ctl;
    
    //-----------------------------------------------------
    //	IcMeshData
    //-----------------------------------------------------
    void IcMeshData::createCube(const TVec3& sz, const TVec3& ofst)
    {
        if(sz.x < 0 || sz.y < 0 || sz.z <0) return;
        //---- We have 8 points for a cube,
        // 0/1/2/3 for top side, 4/5/6/7 for bottom side
        const vector<TVec3>  verts = {
            { -1,  1, -1}, { 1,  1, -1},  {-1,  1, 1}, { 1,  1, 1},
            { -1, -1, -1}, { 1, -1, -1},  {-1, -1, 1}, { 1, -1, 1}};
        for(const auto vi : verts)
        {
            auto v = vi;
            v.x *= sz.x; v.y *= sz.y; v.z *= sz.z;
            v.x += ofst.x; v.y += ofst.y; v.z += ofst.z;
            addVert(v);
        }
        //---- No texture cords provided, add one, as index 0
        addTexCo(TVec2(0,0));
        
        //---- 6 faces of cube
        // 0-1 4-5
        // 2-3 6-7
        struct TQuad{ size_t i0,i1,i2,i3; TVec3 n; };
        const vector<TQuad> quads ={
            // top/bottom quad
            {0,1,2,3, TVec3( 0, 1, 0)}, {6,7,4,5, TVec3( 0,-1, 0)},
            // 4 walls
            {0,2,4,6, TVec3(-1, 0, 0)}, {2,3,6,7, TVec3( 0, 0, 1)},
            {3,1,7,5, TVec3( 1, 0, 0)}, {1,0,5,4, TVec3( 0, 0,-1)}};
        
        size_t qi = 0;
        for(auto q : quads)
        {
            TFaceIdx f;
            auto& pnts = f.m_verts;
            addNorm(q.n);
            size_t idx[4] = { q.i0, q.i1, q.i2, q.i3 };
            for(int i=0;i<4;i++)
            {
                pnts[i].m_vi = idx[i];
                pnts[i].m_ni = qi;
                pnts[i].m_ti = 0;
            }
            addQuad(f, m_cfg.m_isWindingCCR);
            qi ++;
        }
    }
    //-----------------------------------------------------
    //	createPlaneXZ
    //-----------------------------------------------------
    //
    //  p0 -----     0---1
    //   |     |     | / |
    //   ----- p1    2---3
    //
    void IcMeshData::createPlaneXZ(const ctl::TRect& rect,
                                   const ctl::TRect& texRect)
    {
        auto p0 = rect.pos0;
        auto p1 = rect.pos1;
        auto tp0 = texRect.pos0;
        auto tp1 = texRect.pos1;
        struct TPnt{TVec3 v; TVec2 t;};
        
        //---- Add Vert and Tex cordinator
        const vector<TPnt> pnts = {
            {TVec3(p0.x, 0, p0.y),TVec2(tp0.x, 1-tp0.y)},
            {TVec3(p1.x, 0, p0.y),TVec2(tp1.x, 1-tp0.y)},
            {TVec3(p0.x, 0, p1.y),TVec2(tp0.x, 1-tp1.y)},
            {TVec3(p1.x, 0, p1.y),TVec2(tp1.x, 1-tp1.y)}
        };
        for(auto& pnt : pnts) {
            addVert(pnt.v);
            addTexCo(pnt.t);
        }
        
        //---- Add 1 Normal, face up
        addNorm(TVec3(0,1,0));
        TFaceIdx f; int i=0;
        for(auto& v : f.m_verts)
        {
            v.m_vi = v.m_ti = i;
            i++;
        }
        addQuad(f, m_cfg.m_isWindingCCR);
    }
    //-----------------------------------------------------
    //	createGridXZ
    //-----------------------------------------------------
    void IcMeshData::createGridXZ(const ctl::TRect& rect,
                                  const ctl::TRect& texRect,
                                  int N_x, int N_y)
    {
        auto vsz = rect.getSize();
        auto tsz = texRect.getSize();
        TSize vgsz(vsz.w/N_x, vsz.h/N_y);   // GridSize
        TSize tgsz(tsz.w/N_x, tsz.h/N_y);   // Texture GridSize
        auto vp0 = vecConv(rect.pos0);
        auto tp0 = vecConv(texRect.pos0);
        //---- Add Vert/Normal/TexCo
        for(int i=0;i<N_y+1; i++)     // y
        {
            for(int j=0;j<N_x+1;j++)  // x
            {
                //--- Verti
                float x = vgsz.w*j - vgsz.w/2 + vp0.x;
                float z = vgsz.h*j - vgsz.h/2 + vp0.y;
                float y = 0;
                
                //--- Texture
                float u = tgsz.w*j - tgsz.w/2+ tp0.x;
                float v = tgsz.h*j - tgsz.h/2+ tp0.y;
                v = 1 - v;    // TODO: to be verified
                
                //---- Vert, normal and TexCo
                addVert({x,y,z});
                addNorm(glm::normalize(TVec3(0,1,0)));
                addTexCo({u, v});
            }
        }
        //---- Add Quad
        for(int i=0;i<N_y; i++)      // Latitude
            for(int j=0;j<N_x;j++)  // longtitude
            {
                size_t i0 = (N_x+1)*(i+1) + j; size_t i1 = i0 +1;
                size_t i2 = (N_x+1)*i + j;     size_t i3 = i2 +1;
                TFaceIdx f; auto& vs = f.m_verts;
                vs[0]={i0, i0, i0}; vs[1]={i1, i1, i1};
                vs[2]={i2, i2, i2}; vs[3]={i3, i3, i3};
                addQuad(f, m_cfg.m_isWindingCCR);
            }
    }
    //-----------------------------------------------------
    //	createSphere
    //-----------------------------------------------------
    void IcMeshData::createSphere(float R, int N_stack, int N_slice)
    {
        //---- Add Vert/Normal/TexCo
        for(int i=0;i<N_stack+1; i++)     // Latitude
        {
            float latt = deg2rad((180.0/N_stack) * i - 90.0);
            float r = R *  cos(latt);
            float z = R *  sin(latt);
            for(int j=0;j<N_slice+1;j++)  // longtitude
            {
                float longtt = deg2rad(j * (360.0/N_slice));
                float x = cos(longtt) * r;
                float y = sin(longtt) * r;
                
                //---- Vert, normal and TexCo
                addVert(TVec3(x,y,z));
                addNorm(glm::normalize(TVec3(x,y,z)));
                addTexCo(TVec2((float)j/N_slice, 1.0-(float)i/N_stack));
             }
        }
        //---- Add Quad
        for(int i=0;i<N_stack; i++)      // Latitude
             for(int j=0;j<N_slice;j++)  // longtitude
             {
                 size_t i0 = (N_slice+1)*(i+1) + j;
                 size_t i1 = i0 +1;
                 size_t i2 = (N_slice+1)*i + j;
                 size_t i3 = i2 +1;
                 TFaceIdx f; auto& vs = f.m_verts;
                 vs[0]={i0, i0, i0}; vs[1]={i1, i1, i1};
                 vs[2]={i2, i2, i2}; vs[3]={i3, i3, i3};
                 addQuad(f, m_cfg.m_isWindingCCR);
             }
        
    }
}
