#pragma once
#include <windows.h>
#include <string>
#include <unordered_set>

enum class SuspendResult {
    Success,
    Blacklisted,
    UserBlacklisted,
    CriticalSvchost,
    AccessDenied,
    NtCallFailed,
    ProcessGone,
    IsSelf,
    IsSystemProcess,
    ProtectedProcess,
    GuardNotInitialized
};

enum class ResumeResult {
    Success,
    AccessDenied,
    NtCallFailed,
    ProcessGone,
    PidReused,
    GuardNotInitialized
};

class ProcessGuard {
public:
    ProcessGuard();

    bool IsBlacklisted(DWORD pid, const std::wstring& processName) const;
    bool IsCurrentProcess(DWORD pid) const;
    bool IsCriticalSvchost(DWORD pid) const;

private:
    std::unordered_set<std::wstring> userBlacklist_;
};
