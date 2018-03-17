#ifndef common_macro_h__
#define common_macro_h__
#ifndef DISABLE_COPY
#define DISABLE_COPY(class_type) \
const class_type& operator=(const class_type & obj_copy); \
class_type(const class_type& copy_obj);
#endif // !DISABLE_COPY

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif


#endif // common_macro_h__
