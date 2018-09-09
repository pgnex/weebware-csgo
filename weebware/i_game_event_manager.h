#pragma once

#include <cstdint>
#include "key_values.h"

#define EVENT_DEBUG_ID_INIT 42 
#define EVENT_DEBUG_ID_SHUTDOWN 13  

class bf_write;
class bf_read;

class i_game_event
{
public:
	virtual ~i_game_event() = 0;
	virtual const char*     GetName() const = 0;

	virtual bool            IsReliable() const = 0;
	virtual bool            IsLocal() const = 0;
	virtual bool            IsEmpty(const char *keyName = nullptr) = 0;

	virtual bool            GetBool(const char *keyName = nullptr, bool defaultValue = false) = 0;
	virtual int             GetInt(const char *keyName = nullptr, int defaultValue = 0) = 0;
	virtual uint64_t        GetUint64(const char *keyName = nullptr, unsigned long defaultValue = 0) = 0;
	virtual float           GetFloat(const char *keyName = nullptr, float defaultValue = 0.0f) = 0;
	virtual const char*     GetString(const char *keyName = nullptr, const char *defaultValue = "") = 0;
	virtual const wchar_t*  GetWString(const char *keyName, const wchar_t *defaultValue = L"") = 0;

	virtual void            SetBool(const char *keyName, bool value) = 0;
	virtual void            SetInt(const char *keyName, int value) = 0;
	virtual void            SetUint64(const char *keyName, unsigned long value) = 0;
	virtual void            SetFloat(const char *keyName, float value) = 0;
	virtual void            SetString(const char *keyName, const char *value) = 0;
	virtual void            SetWString(const char *keyName, const wchar_t *value) = 0;
};

class i_game_event_listener {
public:
	int m_nDebugID;

	virtual ~i_game_event_listener(void) {};
	virtual void FireGameEvent(i_game_event* event) = 0;

	virtual int GetEventDebugID(void) {
		return m_nDebugID;
	}
};

class i_game_event_manager {
public:
	virtual ~i_game_event_manager(void) {};
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(i_game_event_listener* listener, const char* name, bool serverside) = 0;
	virtual bool FindListener(i_game_event_listener* listener, const char* name) = 0;
	virtual void RemoveListener(i_game_event_listener* listener) = 0;
	virtual void AddListenerGlobal(i_game_event_listener* listener, bool serverside) = 0;
	virtual i_game_event* CreateEvent(const char* name, bool force = false, int* cookie = nullptr) = 0;
	virtual bool FireEvent(i_game_event* event, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(i_game_event* event) = 0;
	virtual i_game_event* DuplicateEvent(i_game_event* event) = 0;
	virtual void FreeEvent(i_game_event* event) = 0;
	virtual bool SerializeEvent(i_game_event* event, bf_write* buffer) = 0;
	virtual i_game_event* UnserializeEvent(bf_read* buffer) = 0;
	virtual KeyValues* GetEventDataTypes(i_game_event* event) = 0;
};