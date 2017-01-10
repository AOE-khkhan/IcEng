//
//  ctl.h
//  CtLib
//
//  Created by Sherman Chen on 2/15/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//
#pragma once

#ifndef __CtLib__
#define __CtLib__

#include <stdio.h>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <sstream>
#include <assert.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <thread>
#include <mutex>

namespace ctl {
	//-----------------------------
	//	String/value convertion
	//-----------------------------
	template <typename T>
	inline std::string v2s(T d)
	{
		std::ostringstream os ;
		os << d ;
		return os.str() ;
	};
	template <typename T>
	inline bool s2v(const std::string& s, T& d)
	{
		return !((std::stringstream(s)>>d).fail());
	};
    //-----------------------------
	//	string utils
	//-----------------------------
    std::vector<std::string> s2tokens(const std::string& s,
							const std::string& sDelimeter);


	//---- String util for file path
	extern std::string sFileExt(const std::string& sFile);
	extern std::string sFilePath(const std::string& sFile);
	extern std::string sFileBase(const std::string& sFile);
	extern std::string sFileNoPath(const std::string& sFile);
	
	//---- Case
	extern std::string s2lower(const std::string& s);
	extern std::string s2upper(const std::string& s);
	extern std::string sFromFile(const std::string& sFile, bool& isReadOK);
	extern bool s2file(const std::string& sFile, const std::string& s);
	extern bool sReplace(std::string& s,
                         const std::string& sKey,
                         const std::string& sRepl);
    //-----------------------------
    //	Aliase for std::shared_ptr
    //-----------------------------
    //---- aliase shared_ptr<T> to Sp<T>
    template<class T> using Sp = std::shared_ptr<T>;
    
    //---- aliase make_shared<T> to makeSp<T>
    template<class T, class ..._Args>
    inline static std::shared_ptr<T> makeSp(_Args&& ...__args)
    { return std::make_shared<T>(__args...); };
    
	//-----------------------------
	//	LogHandler
	//-----------------------------
	class LogHandler
	{
	public:
		enum TE_logLevel
		{
			L_DBG=0,
			L_INFO,
			L_WARN,
			L_ERR,
		};
		virtual void logMsg(TE_logLevel lvl, const std::string& sMsg);
        inline virtual void logMsg(const std::string& sMsg){ std::cout << sMsg; };
        static ctl::Sp<LogHandler> getCurHandler(){ return m_pLogHandler; };
        static void setCurHandler(ctl::Sp<LogHandler> p){ m_pLogHandler = p; };
	protected:
        static ctl::Sp<LogHandler> m_pLogHandler;
		//---- Log Level Default
#if DEBUG
		TE_logLevel m_curLogLvl=L_DBG;
#else
		TE_logLevel m_curLogLvl=L_INFO;
#endif
	};

	inline void logMsg(LogHandler::TE_logLevel lvl, const std::string& sMsg)
		{ LogHandler::getCurHandler()->logMsg(lvl,sMsg); };
	inline void logDbg (const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_DBG,sMsg);};
	inline void logInfo(const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_INFO,sMsg);};
	inline void logWarn(const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_WARN,sMsg);};
	inline void logErr (const std::string& sMsg)
        {logMsg(LogHandler::TE_logLevel::L_ERR,sMsg);};
	inline void logFileErr (const std::string& sFile)
		{logErr("Fail to open file ["+sFile+"]");};
	inline bool chkLogErr(bool c, const std::string& sErr)
		{ if(!c) ctl::logErr(sErr); return c;}
	inline bool chkLogAssert(bool c, const std::string& sErr)
	{
#if DEBUG
		assert(c);
#endif
		return chkLogErr(c, sErr);
	}

    //-----------------------------
    //	UI Coordinates
    //-----------------------------
    struct TPos
    {
        TPos(){};
        virtual ~TPos(){};
        TPos(float xi, float yi){ x=xi; y=yi; };
        float x=0, y=0;
    };
    struct TSize
    {
        TSize(){};
        virtual ~TSize(){};
        TSize(float wi, float hi){ w=wi; h=hi; };
        float w=0, h=0;
    };
    struct TRect
    {
        TRect(){};
        TRect(float x, float y, float w, float h):
        TRect(TPos(x,y), TSize(w,h)){};
        TRect(const TPos& pos, const TSize& sz)
        :pos0(pos), pos1(pos0.x+sz.w , pos0.y+sz.h){};
        virtual ~TRect(){};
        TPos pos0; TPos pos1;
        TSize getSize() const;
    };
    extern bool s2v2d(const std::string& s, TSize& sz);
    extern bool s2v2d(const std::string& s, TPos& pos);
    extern bool s2v2d(const std::string& s, TRect& r);
    
    extern std::string v2s2d(const TSize& sz);
    extern std::string v2s2d(const TPos& sz);
    extern std::string v2s2d(const TRect& sz);
    
    //-----------------------------
    //	BinBuf
    //-----------------------------
    typedef unsigned char TByte;
    class BinBuf
    {
    public:
        BinBuf(){};
        BinBuf(size_t N){ setSize(N); };
        BinBuf(const TByte* pBuf,int N)
        { setSize(N); fill(pBuf,0, N);};
        BinBuf(const BinBuf& bbuf){ copy(bbuf); };
        virtual ~BinBuf(){};
        
        bool copy(const BinBuf& bbuf);
        bool setSize(size_t N);
        bool loadFile(const std::string& sFile);
        bool saveFile(const std::string& sFile) const;
        size_t fill(const TByte* pBuf,
                    size_t iStart, size_t N);
        bool append(const BinBuf& d);
        bool setAt(size_t i, TByte d);
        TByte  operator [](size_t i) const;
        size_t size()const { return m_size; };
        TByte* getBuf() const{ return m_pBuf.get(); };
        void clear(){ m_pBuf = nullptr; m_size=0; };
    protected:
        size_t	m_size = 0;
        std::unique_ptr<TByte> m_pBuf = nullptr;
        
    };
    
   
    //-----------------------------
    //	IcImg
    //-----------------------------
    class IcImg
    {
    public:
        IcImg(){};
        IcImg(const TSize& size) { setSize(size); };
        virtual ~IcImg(){};
        
        struct TPixel{
            TPixel(){};
            TPixel(TByte ri, TByte gi, TByte bi, TByte ai):
            r(ri), g(gi), b(bi), a(ai){};
            TByte r=0,g=0,b=0,a=0; };
        bool setPx(const TPos& pos, const TPixel& c);
        bool getPx(const TPos& pos, TPixel& c) const;
        bool loadFile(const std::string& sFile);
        bool saveFile(const std::string& sFile) const;
        bool setSize(const TSize& size);
        void scaleTo(const TSize& size);
        TSize size()const{ return m_size; };
        BinBuf& getBuf(){ return m_buf; };
        const BinBuf& getBuf()const{ return m_buf; };
        void fillColor(const TPixel& c);
        
        //---- Format Extension Adapter, to support other format externally
        struct CFormatAdp
        {
            virtual bool loadFile(IcImg& img, const std::string& sFile)
            { return false; };
            virtual bool saveFile(const IcImg& img, const std::string& sFile) const
            { return false; };
            
        private:
        };
        static void addFormatAdp(const std::string& sExtList,
                                 ctl::Sp<CFormatAdp> pAdp);
        
    private:
        ctl::BinBuf	m_buf;
        TSize		m_size;
        static void checkSysInit();
        static void checkSysInitPlatform();
        
    };
    
	//-----------------------------
	//	SafeAry
	//-----------------------------
	template<class T>
	class SafeAry
	{
	public:
		SafeAry(){};
		virtual ~SafeAry() {};
		inline bool getAt(size_t i, T& d)const
		{ if(i>=m_ary.size()) return false;
			d = m_ary[i]; return true; };
		void add(const T& d){ m_ary.push_back(d); };
		void clear(){ m_ary.clear(); };
		size_t size()const { return m_ary.size(); };
		
		//---- Lambda func
		typedef std::function<void(size_t idx, T& d)> TFunc;
		void runAll(TFunc f) const
		{	size_t i=0;
			for(auto& d : m_ary) f(i++, d); };
		std::vector<T>& getAry(){ return m_ary; };
		const std::vector<T>& getAry() const{ return m_ary; };
		
	protected:
		std::vector<T>	m_ary;
	
	};

	//-----------------------------
	//	SpAry
	//-----------------------------
	template<class T>
	class SpAry
	{
	public:
		SpAry(){};
		virtual ~SpAry(){};
		void add(Sp<T> p) { m_ary.push_back(p); };
		size_t size() const{ return m_ary.size(); };
		void clear(){ m_ary.clear(); };
		typedef std::function<void(size_t idx, Sp<T>)> TFunc;
		
		//---- Normal access
		Sp<T> operator [](size_t idx) const
			{	if(idx<0 || idx>=m_ary.size()) return nullptr;
				return m_ary[idx];
			};
		const std::vector<Sp<T>>& all() const { return m_ary; };
		
		//---- Access use Lambda
		void runAll(TFunc f) const
			{	size_t i=0;
				for(auto p : m_ary) f(i++, p); };
		bool run(size_t idx, TFunc f) const
			{	if(idx<0 || idx>=m_ary.size()) return false;
				f(idx, m_ary[idx]); return true; };
		
		//---- Acess use Call back class
		struct CallBK
		{	virtual void run(size_t idx, Sp<T> p)=0; };
		void runAll(CallBK& cbk) const
			{	size_t i=0; for(auto p : m_ary) cbk.run(i++, p); };
		bool run(size_t idx, CallBK& cbk) const
			{	if(idx<0 || idx>=m_ary.size()) return false;
				cbk.run(idx, m_ary[idx]); return true; };
		std::vector<Sp<T>>& getAry(){ return m_ary; };

    protected:
		std::vector<Sp<T>> m_ary;
	};
	//-----------------------------
	//	SpMap
	//-----------------------------
	template<class T>
	class SpMap
	{
	public:
		SpMap(){};
		virtual ~SpMap(){};
		typedef std::function<void(const std::string& sKey, Sp<T>)> TFunc;

		void store(const std::string& sKey, Sp<T> p) { m_map[sKey] = p; };
		size_t size() const{ return m_map.size(); };
		void clear(){ m_map.clear(); };
		Sp<T> lookup(const std::string& sKey) const
			{	auto it = m_map.find(sKey);
				if(it==m_map.end()) return nullptr;
				return (*it).second; };
		void getKeys(std::vector<std::string> sKeys)const
			{	sKeys.clear();
				for(auto it = m_map.begin(); it!=m_map.end(); ++it)
					sKeys.push_back((*it).first);	};
		
	
		std::map<std::string, Sp<T>>& all() const { return m_map; };

		//---- Access via Lambda
		void runAll(TFunc f) const
			{	for(auto it = m_map.begin(); it!=m_map.end(); ++it)
				f((*it).first, (*it).second); };
		bool run(const std::string& sKey, TFunc f) const
			{	auto it = m_map.find(sKey);
				if(it==m_map.end()) return false;
				f((*it).first, (*it).second); return true; };
		
		//---- Acess use Call back class
		struct CallBK
		{	virtual void run(const std::string& sKey, Sp<T> p)=0; };
		void runAll(CallBK& cbk) const
			{	for(auto it = m_map.begin(); it!=m_map.end(); ++it)
				cbk.run((*it).first, (*it).second); };
		bool run(const std::string& sKey, CallBK& cbk) const
			{	auto it = m_map.find(sKey);
				if(it==m_map.end()) return false;
				cbk.run((*it).first, (*it).second); return true; };

    protected:
		std::map<std::string, Sp<T>> m_map;
	};
    //-----------------------------
    //	UnitTest
    //-----------------------------
    class UnitTest
    {
    public:
        bool runUnitTest();
        
    protected:
        bool runTestImg();
        bool runTestSpMap();
        bool runTestSpAry();
        
    };
	
}

#endif /* defined(__CtLib__) */