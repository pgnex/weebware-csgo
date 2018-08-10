#include "Header.h"
#include <shlobj.h>

c_config g_config;

variable_t::variable_t(std::string string, float def) :
	m_value(def),
	m_varname(string)
{
	g_config.m_variables[string] = this;
}

variable_t::~variable_t()
{
	if (g_config.m_variables.find(m_varname)->second)
	{
		g_config.m_variables.erase(m_varname);
	}
}

float variable_t::get() const
{
	return this ? m_value : 0.0f;
}

void variable_t::set(const float flnew)
{
	this ? m_value = flnew : false;
}

c_config::c_config()
{

}

c_config::~c_config()
{

}

variable_t * c_config::find_var(std::string variable)
{
	if (m_variables.find(variable)->second)
	{
		return m_variables[variable];
	}

	return nullptr;
}

command_t * c_config::find_com(std::string variable)
{
	if (m_commands.find(variable)->second)
	{
		return m_commands[variable];
	}

	return nullptr;
}

void c_config::Process()
{
	std::string variable, operation;

	std::cin >> variable >> operation;

	if (variable.length() != 0)
	{
		if (operation.length() != 0)
		{
			if (auto Variable = find_var(variable))
			{
				Variable->set(float( atof(operation.c_str()) ));
			}
			else if (auto command = find_com(variable))
			{
				command->call(operation);
			}
			else
			{
				std::cout << "\nError, Variable/Command \"" << variable << "\" Not Found!\n";
			}
		}
	}
}

command_t::command_t(std::string string, commf_t function) :
	m_Funciton(function),
	m_commname(string)
{
	g_config.m_commands[string] = this;
}

command_t::~command_t()
{
	if (g_config.m_commands.find(m_commname)->second)
	{
		g_config.m_commands.erase(m_commname);
	}
}

void command_t::call(std::string& oper)
{
	if (m_Funciton)
		m_Funciton(oper);
}

void __stdcall loadConfig(std::string& File)
{
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	std::string F = std::string(my_documents) + "/weebware/" + File + ".conf";
	auto configFile = std::ifstream(F);
	
	if (!configFile)
	{
		printf("CONFIG NOT FOUND!\n");
	}
	else
	{
		std::string Var, Val;
		while (configFile >> Var >> Val)
		{
			if (auto Variable = g_config.find_var(Var))
			{
				Variable->set(float(atof(Val.c_str())));
			}
		}

		configFile.close();
		printf("CONFIG ");
		printf(File.c_str());
		printf(" LOADED!\n");
	}
}

void __stdcall saveConfig(std::string& File)
{
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

	std::string F = std::string(my_documents) + "/weebware/" + File + ".conf";
	auto configFile = std::ofstream(F, std::ios_base::out | std::ios_base::trunc);

	if (!configFile)
	{
		printf("CONFIG UNABLE TO BE CREATED!\n");
	}
	else
	{
		for (auto Var : g_config.m_variables)
		{
			configFile << Var.first << " " << Var.second->get() << std::endl;
		}

		printf("CONFIG SAVED!\n");
		configFile.close();
	}
}

c_vars g_vars;

command_t Load = command_t("load", loadConfig);
command_t Save = command_t("save", saveConfig);


