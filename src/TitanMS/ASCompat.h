#pragma once
#include <angelscript.h>
#include <scriptbuilder.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

// 전역 엔진 포인터(없으면 선언만)
extern asIScriptEngine* g_ScriptEngine;

struct ASSectionMem {
    std::string name;
    std::string code;
    int lineOffset = 0;
};

struct ASCompatModule {
    std::unique_ptr<CScriptBuilder> builder;
    std::vector<ASSectionMem> sections;
};

bool AS_COMPAT_AddScriptSection(asIScriptEngine* eng,
                                const char* moduleName,
                                const char* sectName,
                                const char* code,
                                int codeLen,
                                int lineOffset);

int  AS_COMPAT_Build(asIScriptEngine* eng, const char* moduleName);

int  AS_COMPAT_SaveByteCode(asIScriptEngine* eng,
                            const char* moduleName,
                            asIBinaryStream* stream);

int  AS_COMPAT_LoadByteCode(asIScriptEngine* eng,
                            const char* moduleName,
                            asIBinaryStream* stream);

asIScriptFunction* AS_COMPAT_GetFunctionByName(asIScriptEngine* eng,
                                               const char* moduleName,
                                               const char* funcName);
