#include "core/ProcessGuard.h"
#include <algorithm>
#include <cwctype>

namespace {

const std::unordered_set<std::wstring> kHardcodedBlacklist = {
    L"csrss.exe",
    L"smss.exe",
    L"wininit.exe",
    L"winlogon.exe",
    L"lsass.exe",
    L"services.exe",
    L"dwm.exe",
    L"explorer.exe",
    L"msmpeng.exe",
    L"memory compression",
    L"winfreeze.exe",
    L"system",
    L"registry",
    L"secure system",
};

std::wstring NormalizeName(const std::wstring& name) {
    size_t pos = name.find_last_of(L"/\\");
    std::wstring filename = (pos != std::wstring::npos) ? name.substr(pos + 1) : name;

    std::wstring lower = filename;
    std::transform(lower.begin(), lower.end(), lower.begin(),
        [](wchar_t c) { return static_cast<wchar_t>(std::towlower(c)); });

    return lower;
}

} // namespace

ProcessGuard::ProcessGuard() = default;

bool ProcessGuard::IsBlacklisted(DWORD pid, const std::wstring& processName) const {
    if (pid == 0 || pid == 4)
        return true;

    if (IsCurrentProcess(pid))
        return true;

    std::wstring normalized = NormalizeName(processName);
    return kHardcodedBlacklist.count(normalized) > 0 ||
           userBlacklist_.count(normalized) > 0;
}

bool ProcessGuard::IsCurrentProcess(DWORD pid) const {
    return pid == GetCurrentProcessId();
}

bool ProcessGuard::IsCriticalSvchost(DWORD /*pid*/) const {
    // Implemented in Phase 4 — SCM enumeration
    return false;
}
