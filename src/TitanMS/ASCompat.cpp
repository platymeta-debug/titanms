#include "ASCompat.h"
#include <cassert>
#include <cstring>

static std::unordered_map<std::string, ASCompatModule> g_modules;

static ASCompatModule& getOrCreateMod(const char* moduleName) {
    return g_modules[std::string(moduleName ? moduleName : "")];
}

bool AS_COMPAT_AddScriptSection(asIScriptEngine* eng,
                                const char* moduleName,
                                const char* sectName,
                                const char* code,
                                int codeLen,
                                int lineOffset)
{
    (void)eng;
    auto& m = getOrCreateMod(moduleName);
    if (!m.builder) m.builder = std::make_unique<CScriptBuilder>();

    ASSectionMem s;
    s.name = sectName ? sectName : "";
    s.code.assign(code, code + (codeLen >= 0 ? codeLen : (int)std::strlen(code)));
    s.lineOffset = lineOffset;
    m.sections.emplace_back(std::move(s));
    return true;
}

int AS_COMPAT_Build(asIScriptEngine* eng, const char* moduleName)
{
    auto it = g_modules.find(std::string(moduleName ? moduleName : ""));
    if (it == g_modules.end()) return -1;

    auto& m = it->second;
    auto* b = m.builder.get();
    if (!b) return -2;

    if (b->StartNewModule(eng, moduleName) < 0) {
        g_modules.erase(it);
        return -3;
    }

    // 메모리 섹션으로 모두 추가
    for (auto& s : m.sections) {
        int r = b->AddSectionFromMemory(s.name.c_str(), s.code.c_str(), (unsigned)s.code.size(), s.lineOffset);
        if (r < 0) return r;
    }
    int r = b->BuildModule();
    g_modules.erase(it);
    return r;
}

int AS_COMPAT_SaveByteCode(asIScriptEngine* eng, const char* moduleName, asIBinaryStream* stream)
{
    auto* mod = eng->GetModule(moduleName, asGM_ONLY_IF_EXISTS);
    if (!mod) return -10;
    return mod->SaveByteCode(stream);
}

int AS_COMPAT_LoadByteCode(asIScriptEngine* eng, const char* moduleName, asIBinaryStream* stream)
{
    auto* mod = eng->GetModule(moduleName, asGM_ALWAYS_CREATE);
    if (!mod) return -11;
    return mod->LoadByteCode(stream);
}

asIScriptFunction* AS_COMPAT_GetFunctionByName(asIScriptEngine* eng, const char* moduleName, const char* funcName)
{
    auto* mod = eng->GetModule(moduleName, asGM_ONLY_IF_EXISTS);
    return mod ? mod->GetFunctionByName(funcName) : nullptr;
}
