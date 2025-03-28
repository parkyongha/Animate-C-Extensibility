#pragma once

/*****************************************************************************
 * Public data types
 ****************************************************************************/

typedef struct JSContext JSContext;
typedef struct JSObject JSObject;
#if _WIN64 || __x86_64__
typedef long long jsval;
#else
typedef long jsval;
#endif
#ifndef JSBool
#   if _WIN64 || __x86_64__
typedef long long JSBool;
#   else
typedef long JSBool;
#   endif
#endif

typedef JSBool(*JSNative)(JSContext* cx, JSObject* obj, unsigned int argc,
	jsval* argv, jsval* rval);

/* Possible values for JSBool */
#define JS_TRUE 1
#define JS_FALSE 0



/*****************************************************************************
 * Public functions
 ****************************************************************************/

 /* JSBool JS_DefineFunction(unsigned short *name, JSNative call, unsigned int nargs) */
#define JS_DefineFunction(n, c, a) \
    (mmEnv.defineFunction ? (*(mmEnv.defineFunction))(mmEnv.libObj, n, c, a) \
                          : JS_FALSE)

/* unsigned short *JS_ValueToString(JSContext *cx, jsval v, unsigned short *pLength) */
#define JS_ValueToString(c, v, l) \
    (mmEnv.valueToString  ? (*(mmEnv.valueToString))(c, v, l) : (unsigned short *)0)

/* unsigned char *JS_ValueToBytes(JSContext *cx, jsval v, unsigned int *pLength) */
#define JS_ValueToBytes(c, v, l) \
    (mmEnv.valueToBytes  ? (*(mmEnv.valueToBytes))(c, v, l) : (unsigned char *)0)

/* JSBool JS_ValueToInteger(JSContext *cx, jsval v, long *lp); */
#define JS_ValueToInteger(c, v, l) \
    (mmEnv.valueToInteger ? (*(mmEnv.valueToInteger))(c, v, l) : JS_FALSE)

/* JSBool JS_ValueToDouble(JSContext *cx, jsval v, double *dp); */
#define JS_ValueToDouble(c, v, d) \
    (mmEnv.valueToDouble  ? (*(mmEnv.valueToDouble))(c, v, d) : JS_FALSE)

/* JSBool JS_ValueToBoolean(JSContext *cx, jsval v, JSBool *bp); */
#define JS_ValueToBoolean(c, v, b) \
    (mmEnv.valueToBoolean ? (*(mmEnv.valueToBoolean))(c, v, b) : JS_FALSE)

/* JSBool JS_ValueToObject(JSContext *cx, jsval v, JSObject **op); */
#define JS_ValueToObject(c, v, o) \
    (mmEnv.valueToObject  ? (*(mmEnv.valueToObject))(c, v, o) : JS_FALSE)

/* JSBool JS_StringToValue(JSContext *cx, unsigned short *bytes, uint sz, jsval *vp); */
#define JS_StringToValue(c, b, s, v) \
    (mmEnv.stringToValue  ? (*(mmEnv.stringToValue))(c, b, s, v) : JS_FALSE)

/* JSBool JS_BytesToValue(JSContext *cx, unsigned char *bytes, uint sz, jsval *vp); */
#define JS_BytesToValue(c, b, s, v) \
    (mmEnv.bytesToValue  ? (*(mmEnv.bytesToValue))(c, b, s, v) : JS_FALSE)

/* JSBool JS_DoubleToValue(JSContext *cx, double dv, jsval *vp); */
#define JS_DoubleToValue(c, d, v) \
    (mmEnv.doubleToValue  ? (*(mmEnv.doubleToValue))(c, d, v) : JS_FALSE)

/* jsval JS_IntegerToValue(long lv); */
#define JS_IntegerToValue(lv)       (((jsval)(lv) << 1) | 0x1)

/* jsval JS_BooleanToValue(JSBool bv); */
#define JS_BooleanToValue(bv)       (((jsval)(bv) << 3) | 0x6)

/* jsval JS_ObjectToValue(JSObject *obj); */
#define JS_ObjectToValue(ov)        ((jsval)(ov))

/* unsigned short *JS_ObjectType(JSObject *obj); */
#define JS_ObjectType(o) \
    (mmEnv.objectType     ? (*(mmEnv.objectType))(o) : (unsigned short *)0)

/* JSObject *JS_NewArrayObject(JSContext *cx, unsigned int length, jsval *v) */
#define JS_NewArrayObject(c, l, v) \
    (mmEnv.newArrayObject ? (*(mmEnv.newArrayObject))(c, l, v) : (JSObject *)0)

/* long JS_GetArrayLength(JSContext *cx, JSObject *obj) */
#define JS_GetArrayLength(c, o) \
    (mmEnv.getArrayLength ? (*(mmEnv.getArrayLength))(c, o) : -1)

/* JSBool JS_GetElement(JSContext *cx, JSObject *obj, jsint idx, jsval *vp) */
#define JS_GetElement(c, o, i, v) \
    (mmEnv.getElement     ? (*(mmEnv.getElement))(c, o, i, v) : JS_FALSE)

/* JSBool JS_SetElement(JSContext *cx, JSObject *obj, jsint idx, jsval *vp) */
#define JS_SetElement(c, o, i, v) \
    (mmEnv.setElement     ? (*(mmEnv.setElement))(c, o, i, v) : JS_FALSE)

/* JSBool JS_ExecuteScript(JSContext *cx, JSObject *obj, unsigned short *script,
 *     unsigned int sz, jsval *rval) */
#define JS_ExecuteScript(c, o, s, z, r) \
    (mmEnv.executeScript  ? (*(mmEnv.executeScript))(c, o, s, z, _T(__FILE__), \
        __LINE__, r) : JS_FALSE)

 /* JSBool JS_ReportError(JSContext *cx, unsigned short *error, unsigned int sz) */
#define JS_ReportError(c, e, s) \
    (mmEnv.reportError    ? (*(mmEnv.reportError))(c, e, s) : JS_FALSE)



/*****************************************************************************
 * Private data types, macros, and globals
 ****************************************************************************/

typedef struct {
	JSObject* libObj;
	JSBool(*defineFunction)(JSObject* libObj, unsigned short* name, JSNative call,
		unsigned int nargs);
	unsigned short* (*valueToString)(JSContext* cx, jsval v, unsigned int* pLength);
	unsigned char* (*valueToBytes)(JSContext* cx, jsval v, unsigned int* pLength);
	JSBool(*valueToInteger)(JSContext* cx, jsval v, long* lp);
	JSBool(*valueToDouble)(JSContext* cx, jsval v, double* dp);
	JSBool(*valueToBoolean)(JSContext* cx, jsval v, JSBool* bp);
	JSBool(*valueToObject)(JSContext* cx, jsval v, JSObject** op);
	JSBool(*stringToValue)(JSContext* cx, unsigned short* b, unsigned int sz, jsval* vp);
	JSBool(*bytesToValue)(JSContext* cx, unsigned char* b, unsigned int sz, jsval* vp);
	JSBool(*doubleToValue)(JSContext* cx, double dv, jsval* vp);
	unsigned short* (*objectType)(JSObject* obj);
	JSObject* (*newArrayObject)(JSContext* cx, unsigned int length, jsval* vp);
	long (*getArrayLength)(JSContext* cx, JSObject* obj);
	JSBool(*getElement)(JSContext* cx, JSObject* obj, unsigned int idx,
		jsval* vp);
	JSBool(*setElement)(JSContext* cx, JSObject* obj, unsigned int idx,
		jsval* vp);
	JSBool(*executeScript)(JSContext* cx, JSObject* obj, unsigned short* script,
		unsigned int sz, unsigned short* file, unsigned int lineNum, jsval* rval);
	JSBool(*reportError)(JSContext* cx, unsigned short* error, unsigned int sz);
} MM_Environment;

extern MM_Environment mmEnv;

// Declare the external entry point and linkage
#ifdef _WIN32
#   ifndef _MAC  
	// Windows (32 & 64 bit)
extern "C" {
__declspec(dllexport) void MM_InitWrapper(MM_Environment* env, unsigned int envSize);
}
#   else
	// Mac with MSVC++ Win32 portability lib
extern void MM_InitWrapper(MM_Environment* env, unsigned int envSize);
#   endif
#else
	// Codewarrior
#   pragma export on    MM_Environment
extern void MM_InitWrapper(*env, unsigned int envSize);
#   pragma export off
#endif



#define MM_STATE                                                                \
    /* Definitions of global variables */                                       \
    MM_Environment mmEnv;                                                       \
                                                                                \
    void                                                                        \
    MM_InitWrapper(MM_Environment *env, unsigned int envSize)                   \
    {                                                                           \
        extern void MM_Init();                                                  \
                                                                                \
        char **envPtr = (char **)env;                                           \
        char **mmPtr =  (char **)(&mmEnv);                                      \
        char **envEnd = (char **)((char *)envPtr + envSize);                    \
        char **mmEnd =  (char **)((char *)mmPtr  + sizeof(MM_Environment));     \
                                                                                \
        /* Copy fields from env to mmEnv, one pointer at a time */              \
        while (mmPtr < mmEnd && envPtr < envEnd)                                \
            *mmPtr++ = *envPtr++;                                               \
                                                                                \
      /* If env doesn't define all of mmEnv's fields, set extras to NULL */     \
        while (mmPtr < mmEnd)                                                   \
            *mmPtr++ = (char *)0;                                               \
                                                                                \
      /* Call user's MM_Init function */                                        \
      MM_Init();                                                                \
    }                                                                           \

