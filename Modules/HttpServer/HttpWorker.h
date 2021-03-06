#pragma once
#include "LiveClient.h"

namespace HttpWsServer
{
    struct pss_http_ws_live;
    enum MediaType;

    class CHttpWorker : public LiveClient::ILiveHandle
    {
    public:
        CHttpWorker(string strCode, HandleType t, int nChannel);
        ~CHttpWorker();

        /** 客户端连接 */
        bool AddConnect(pss_http_ws_live* pss);
        bool DelConnect(pss_http_ws_live* pss);

        /** 请求端获取视频数据 */
        AV_BUFF GetHeader();
        AV_BUFF GetVideo(uint32_t *tail);
        void NextWork(pss_http_ws_live* pss);

        virtual void push_video_stream(AV_BUFF buff);
        virtual void stop();
        virtual string get_clients_info();
    private:
        void cull_lagging_clients();

    private:
        string                m_strCode;     // 播放媒体编号

        /**
         * lws_ring无锁环形缓冲区，只能一个线程写入，一个线程读取
         * m_pRing由liveworker中的uv_loop线程写入，http服务所在的uv_loop线程读取
         */
        AV_BUFF               m_stHead;
        struct lws_ring       *m_pRing;
        pss_http_ws_live      *m_pPssList;

        int                   m_nType;          //< 0:live直播；1:record历史视频
        LiveClient::ILiveWorker *m_pLive;       //< 直播数据接收和解包装包
        HandleType            m_type;           //< 表明是哪一种类型
        int                   m_nChannel;       //< 通道 0:原始码流  1:小码流
    };

    /** 直播 */
    CHttpWorker* CreatHttpWorker(string strCode, HandleType t, int nChannel);
    CHttpWorker* GetHttpWorker(string strCode, HandleType t, int nChannel);
    bool DelHttpWorker(string strCode, HandleType t, int nChannel);

    /** 点播 */
};