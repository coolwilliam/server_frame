#ifndef tool_exports_h__
#define tool_exports_h__

#ifdef SERVER_FRAME_TOOLS_EXPORTS
#ifdef __cplusplus
#define SERVER_FRAME_TOOL_API
#else
#define SERVER_FRAME_TOOL_API (extern "C")
#endif  // __cplusplus
#else
#ifdef __cplusplus
#define SERVER_FRAME_TOOL_API
#else
#define SERVER_FRAME_TOOL_API (extern "C")
#endif  // __cplusplus
#endif  // SERVER_FRAME_TOOLS_EXPORTS

#endif  // tool_exports_h__
