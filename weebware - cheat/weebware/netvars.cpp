#include "Header.h"
#include "netvars.h"
#include "shared.h"
#include <fstream>

//------------------------------------------------------------
// Classes to help the parsing of the netvars
//------------------------------------------------------------
#pragma region Helper classes

typedef std::unordered_map<std::string, std::unique_ptr<netvar_table>> TableMap;
typedef std::unordered_map<std::string, uint32_t> PropMap;

typedef std::unordered_map<std::string, std::unique_ptr<netvar_table>>::iterator Iter;

struct netvar_table
{
	TableMap m_ChildTables;
	PropMap m_ChildProps;
	uint32_t m_uOffset = 0;

	bool is_empty()
	{
		return m_ChildTables.size() == 0 && m_ChildProps.size() == 0;
	}

	void insert_table(std::string&& name, std::unique_ptr<netvar_table>&& pTable)
	{
		m_ChildTables.emplace(name, std::move(pTable));
	}

	void insert_prop(std::string&& name, uint32_t offset)
	{
		m_ChildProps.emplace(name, offset);
	}
};

class NetvarDatabase
{
public:
	TableMap m_Tables;

	void insert(std::string&& name, std::unique_ptr<netvar_table>&& pTable)
	{
		m_Tables.emplace(name, std::move(pTable));
	}

	Iter find(const std::string& key)
	{
		return m_Tables.find(key);
	}

	Iter begin()
	{
		return m_Tables.begin();
	}

	Iter end()
	{
		return m_Tables.end();
	}
};

#pragma endregion

//------------------------------------------------------------
// Netvar Manager
//------------------------------------------------------------
netvar_manager* netvar_manager::instance = nullptr;

netvar_manager::netvar_manager()
{
}

netvar_manager::~netvar_manager()
{
}

void netvar_manager::create_database()
{
	m_database = std::make_unique<NetvarDatabase>();
	auto pClient = g_weebware.g_client;

	if (pClient)
	{
		for (auto pClass = pClient->get_all_classes(); pClass; pClass = pClass->m_pNext)
		{
			if (pClass->m_precvtable)
			{
				//Insert new entry on the database
				m_database->insert(
					pClass->m_precvtable->m_pNetTableName,
					internal_load_table(pClass->m_precvtable, 0));
				m_tableCount++;
			}
		}
	}
}

void netvar_manager::dump(std::ostream& output)
{
	for (auto& entry : *m_database)
	{
		auto& pTable = entry.second;
		if (pTable->is_empty())
			continue;
		output << entry.first << std::endl;
		dump(output, *pTable, 1);
		output << "==============================================================" << std::endl;
	}
}
void netvar_manager::dump2(std::ostream& output)
{
	for (auto& entry : *m_database)
	{
		auto& pTable = entry.second;
		if (pTable->is_empty())
			continue;
		output << entry.first << "=0 ";
		dump2(output, *pTable, 1, entry.first);
		output << "";
	}
}
std::vector<std::string> offsets;
std::vector<int> values;
int netvar_manager::get_offset(std::string offset)
{
	const char *offsetchar = offset.c_str();

	for (unsigned i = 0; i < offsets.size(); i++)
	{
		if (!strcmp(offsetchar, offsets[i].c_str()))
		{
			return values[i];
		}
	}

	return 0;
}
void netvar_manager::phaser(std::string chain)
{
	std::string tovector = chain;
	std::stringstream ss(tovector);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> settingarray(begin, end);
	//Gets how many settings to set
	int size = settingarray.size();
	for (int i = 0; i < size - 1; i++)
	{
		// Gets the setting we are dealing with
		std::string meme = settingarray[i];
		//Converst the string to char array
		for (unsigned i = 0; i < meme.length(); i++)
		{
			if (meme[i] == '=')
				meme[i] = ' ';
		}
		std::stringstream temp(meme);
		std::istream_iterator<std::string> tempBegin(temp);
		std::istream_iterator<std::string> Tempend;
		std::vector<std::string> Tempsettingarray(tempBegin, Tempend);
		offsets[i] = Tempsettingarray[0];
		values[i] = atoi(Tempsettingarray[1].c_str());
	}
}
void netvar_manager::dump(const std::string& file)
{
	std::ofstream output(file);
	dump(output);
}
void netvar_manager::dump2(const std::string& file)
{
	std::ofstream output(file);
	dump2(output);
}

//------------------------------------------------------------
// Internal methods below. This is where the real work is done
//------------------------------------------------------------
std::unique_ptr<netvar_table> netvar_manager::internal_load_table(recvtable* precvtable, uint32_t offset)
{
	auto pTable = std::make_unique<netvar_table>();
	pTable->m_uOffset = offset;

	for (auto i = 0; i < precvtable->m_nProps; ++i)
	{
		auto pProp = &precvtable->m_pProps[i];

		//Skip trash array items
		if (!pProp || isdigit(pProp->m_pVarName[0])) continue;
		//We dont care about the base class
		if (strcmp(pProp->m_pVarName, "baseclass") == 0) continue;


		//If this prop is a table
		if (pProp->m_RecvType == SendPropType::DPT_DataTable && //If it is a table AND
			pProp->m_pDataTable != NULL && //The DataTable isnt null AND
			pProp->m_pDataTable->m_pNetTableName[0] == 'D')
		{ //The Table name starts with D (this is because there are some shitty nested 
		  //tables that we want to skip, and those dont start with D)

		  //Load the table pointed by pProp->m_pDataTable and insert it
			pTable->insert_table(pProp->m_pVarName, internal_load_table(pProp->m_pDataTable, pProp->m_Offset));
		}
		else
		{
			pTable->insert_prop(pProp->m_pVarName, pProp->m_Offset);
		}
		m_netvarCount++;
	}
	return pTable;
}
void netvar_manager::dump2(std::ostream& output, netvar_table& table, int level, std::string first)
{
	char line[512] = "";
	char fmt[512] = "";

	sprintf(fmt, "%%-%ds=0x%%08X ", 1);

	for (auto& prop : table.m_ChildProps)
	{
		sprintf(line, fmt, prop.first.c_str(), prop.second + table.m_uOffset);
		output << first << "_" << line;
	}
	for (auto& child : table.m_ChildTables)
	{
		for (int i = 0; i < level; i++)
		{
			if (i != level - 1)
			{
				//output << first << "_" << line2;
			}
			else
			{
			}
		}
		sprintf(line, fmt, child.first.c_str(), child.second->m_uOffset);
		output << first << "_" << line;
		first = first.append("_").append(child.first.c_str());
		dump2(output, *child.second, level + 1, first);
	}
}

void netvar_manager::dump(std::ostream& output, netvar_table& table, int level)
{
	char line[512] = "";
	char fmt[512] = "";

	sprintf(fmt, "%%-%ds: 0x%%08X\n", 50 - level * 4);

	for (auto& prop : table.m_ChildProps)
	{
		for (int i = 0; i < level; i++)
		{
			if (i != level - 1)
			{
				output << "    ";
			}
			else
			{
				output << "---> ";
			}
		}
		sprintf(line, fmt, prop.first.c_str(), prop.second + table.m_uOffset);
		output << line;
	}
	for (auto& child : table.m_ChildTables)
	{
		for (int i = 0; i < level; i++)
		{
			if (i != level - 1)
			{
				output << "    ";
			}
			else
			{
				output << "---> ";
			}
		}
		sprintf(line, fmt, child.first.c_str(), child.second->m_uOffset);
		output << line;
		dump(output, *child.second, level + 1);
	}
}

uint32_t netvar_manager::get_offset(const std::string& szTableName, const std::initializer_list<std::string>& props)
{
	auto table = m_database->find(szTableName);
	if (table == m_database->end()) return -1;

	int tableOffset = table->second->m_uOffset;
	if (props.size() == 0) return tableOffset;

	int totalOffset = tableOffset;

	netvar_table* curTable = table->second.get();

	for (unsigned i = 0; i < props.size(); i++)
	{
		std::string propName = *(props.begin() + i);

		if (i + 1 < props.size())
		{//This index is not the last one
			auto childTable = curTable->m_ChildTables.find(propName);
			if (childTable == curTable->m_ChildTables.end())
			{
				throw std::runtime_error("Prop not found");
			}
			totalOffset += childTable->second->m_uOffset;

			curTable = childTable->second.get();
		}
		else
		{ //Last index, retrieve prop instead of table
			auto childProp = curTable->m_ChildProps.find(propName);
			if (childProp == curTable->m_ChildProps.end())
			{
				throw std::runtime_error("Prop not found");
			}

			totalOffset += childProp->second;
		}
	}

	return totalOffset;
}
