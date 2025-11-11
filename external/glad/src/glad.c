/*
 * GLAD Implementation
 * Loader simplificado para OpenGL
 */

#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
    #include <windows.h>
    static HMODULE libgl;
    typedef PROC (WINAPI* GLADloadproc)(LPCSTR lpProcName);
#elif defined(__APPLE__)
    #include <dlfcn.h>
    static void* libgl;
#else
    #include <dlfcn.h>
    static void* libgl;
#endif

/* Function pointers */
PFNGLCLEARPROC glad_glClear = NULL;
PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
PFNGLVIEWPORTPROC glad_glViewport = NULL;
PFNGLENABLEPROC glad_glEnable = NULL;
PFNGLDISABLEPROC glad_glDisable = NULL;
PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLCULLFACEPROC glad_glCullFace = NULL;
PFNGLDRAWELEMENTSPROC glad_glDrawElements = NULL;
PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
PFNGLGETSTRINGPROC glad_glGetString = NULL;

/* Platform-specific loader */
static void* get_proc(const char *namez) {
    void* result = NULL;
#if defined(_WIN32)
    if (libgl != NULL) {
        result = (void*)GetProcAddress((HMODULE)libgl, namez);
    }
#elif defined(__APPLE__)
    if (libgl != NULL) {
        result = dlsym(libgl, namez);
    }
#else
    if (libgl != NULL) {
        result = dlsym(libgl, namez);
    }
#endif
    return result;
}

/* Open OpenGL library */
static int open_gl(void) {
#if defined(_WIN32)
    libgl = LoadLibraryA("opengl32.dll");
    return libgl != NULL;
#elif defined(__APPLE__)
    libgl = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY | RTLD_LOCAL);
    return libgl != NULL;
#else
    libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_LOCAL);
    if (!libgl) {
        libgl = dlopen("libGL.so", RTLD_LAZY | RTLD_LOCAL);
    }
    return libgl != NULL;
#endif
}

/* Close OpenGL library */
static void close_gl(void) {
#if defined(_WIN32)
    if (libgl != NULL) {
        FreeLibrary((HMODULE)libgl);
        libgl = NULL;
    }
#elif defined(__APPLE__) || defined(__linux__)
    if (libgl != NULL) {
        dlclose(libgl);
        libgl = NULL;
    }
#endif
}

/* Main loader function */
int gladLoadGLLoader(void* (*load)(const char *name)) {
    if (!open_gl()) {
        return 0;
    }

    /* Load core functions */
    glad_glClear = (PFNGLCLEARPROC)load("glClear");
    glad_glClearColor = (PFNGLCLEARCOLORPROC)load("glClearColor");
    glad_glViewport = (PFNGLVIEWPORTPROC)load("glViewport");
    glad_glEnable = (PFNGLENABLEPROC)load("glEnable");
    glad_glDisable = (PFNGLDISABLEPROC)load("glDisable");
    glad_glBlendFunc = (PFNGLBLENDFUNCPROC)load("glBlendFunc");
    glad_glCullFace = (PFNGLCULLFACEPROC)load("glCullFace");
    glad_glDrawElements = (PFNGLDRAWELEMENTSPROC)load("glDrawElements");
    glad_glDrawArrays = (PFNGLDRAWARRAYSPROC)load("glDrawArrays");
    glad_glGetString = (PFNGLGETSTRINGPROC)load("glGetString");

    /* Verificar que al menos las funciones básicas se cargaron */
    if (!glad_glClear || !glad_glClearColor || !glad_glViewport || !glad_glGetString) {
        return 0;
    }

    return 1;
}
