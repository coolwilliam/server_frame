#ifndef network_export_h__
#define network_export_h__

#ifdef SERVER_FRAME_NETWORK_EXPORTS
#ifdef __cplusplus
#define SERVER_FRAME_NETWORK_API
#else
#define SERVER_FRAME_NETWORK_API (extern "C")
#endif //__cplusplus
#else
#ifdef __cplusplus
#define SERVER_FRAME_NETWORK_API
#else
#define SERVER_FRAME_NETWORK_API (extern "C")
#endif //__cplusplus
#endif //SERVER_FRAME_NETWORK_EXPORTS

#endif // network_export_h__
