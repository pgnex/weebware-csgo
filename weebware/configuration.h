#pragma once
#include <string>
#include <unordered_map>

struct variable_t
{
public:
	variable_t(std::string string, float def);
	~variable_t();

	float get() const;
	void set(const float flnew);

private:
	float m_value;
	std::string m_varname;

};

struct command_t
{
public:
	typedef void(__stdcall *commf_t)(std::string&);
	command_t(std::string string, commf_t function);
	~command_t();

	void call(std::string&);

private:
	std::string m_commname;
	commf_t m_Funciton;

};

class c_config
{
public:
	c_config();
	~c_config();

	variable_t* find_var(std::string variable);
	command_t* find_com(std::string variable);
	
	void Process();

public: // fuck this, ima just make it public for easyer codens
	std::unordered_map<std::string, variable_t*> m_variables;
	std::unordered_map<std::string, command_t*> m_commands;
	friend struct variable_t;
	friend struct command_t;
};


extern c_config g_config;