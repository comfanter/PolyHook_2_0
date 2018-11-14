//https://github.com/odzhan/shellcode/blob/master/os/win/getapi/dynamic/getapi.c
//https://modexp.wordpress.com/2017/01/15/shellcode-resolving-api-addresses/
#include <string>
#include <iostream>
#include <algorithm>

#include "headers/ErrorLog.hpp"
#include "headers/IHook.hpp"
#include "headers/MemProtector.hpp"
#include "headers/Misc.hpp"
#include "headers/PE/PEB.hpp"

#define RVA2VA(type, base, rva) (type)((ULONG_PTR) base + rva)

namespace PLH {
class EatHook : public IHook {
public:
	EatHook(const std::string& apiName, const std::wstring& moduleName, const char* fnCallback, uint64_t* userOrigVar);
	EatHook(const std::string& apiName, const std::wstring& moduleName, const uint64_t fnCallback, uint64_t* userOrigVar);
	virtual ~EatHook();
	virtual bool hook() override;
	virtual bool unHook() override;
	
	virtual HookType getType() const override {
		return HookType::EAT;
	}
private:
	uint32_t* FindEatFunction(const std::string& apiName, const std::wstring& moduleName = L"");
	uint32_t* FindEatFunctionInModule(const std::string& apiName);

	std::wstring m_moduleName;
	std::string m_apiName;

	uint64_t m_fnCallback;
	uint64_t m_origFunc;
	uint64_t* m_userOrigVar;

	// only used if EAT offset points >= 2GB
	uint8_t* m_trampoline;

	bool m_hooked;
	uint64_t m_moduleBase;
};

inline void* Allocate_2GB_IMPL(uint8_t* pStart, size_t Size, int_fast64_t Delta);
inline void* AllocateWithin2GB(uint8_t* pStart, size_t Size, size_t& AllocationDelta);
}