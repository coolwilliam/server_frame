#ifndef business_exports_h__
#define business_exports_h__

#ifdef SERVER_FRAME_BUSINESS_EXPORTS
#ifdef __cplusplus
#define SERVER_FRAME_BUSINESS_API
#else
#define SERVER_FRAME_BUSINESS_API (extern "C")
#endif  // __cplusplus
#else
#ifdef __cplusplus
#define SERVER_FRAME_BUSINESS_API
#else
#define SERVER_FRAME_BUSINESS_API (extern "C")
#endif  // __cplusplus
#endif  // SERVER_FRAME_BUSINESS_EXPORTS

#endif  // business_exports_h__
