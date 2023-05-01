#pragma once
#include "Header.h"

//class KeyValues
//{
//public:
//	char _pad[0x20];//csgo, for css its a diff size
//
//
//	KeyValues* FindKey(const char* keyName, bool bCreate = false);
//	void set_string(const char* keyName, const char* value);
//
//};

class KeyValues
{
public:
	KeyValues* FindKey(const char* keyName, bool bCreate = false);
	void set_string(const char* keyName, const char* value);

private:
  std::uint32_t m_key_name : 24;
  std::uint32_t m_key_name_case_sensitive1 : 8;
  char* m_sz_value{};
  wchar_t* m_wsz_value{};

  union {
    int m_i_value{};
    float m_f_value;
    void* m_value;
    unsigned char m_color[4];
  };

  std::int8_t m_data_type{};
  bool m_has_escape_sequences{};
  std::uint16_t m_key_name_case_sensitive2{};
  void* m_unk{};
  bool m_has_case_insensitive_key_symbol{};
  KeyValues* m_peer{};
  KeyValues* m_sub{};
  KeyValues* m_chain{};
  int m_expression_get_symbol_proc{};
};
