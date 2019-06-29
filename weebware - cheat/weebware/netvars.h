#pragma once
#include "c_rec.h"

struct netvar_table;

class NetvarDatabase;

class netvar_manager
{
private:
	static netvar_manager* instance;

	netvar_manager();
	~netvar_manager();

	netvar_manager(const netvar_manager&) = delete;

public:
	static netvar_manager* _instance()
	{
		if (!instance) instance = new netvar_manager;
		return instance;
	}

	void create_database();

	void dump(std::ostream& stream);
	void dump(const std::string& file);
	void phaser(std::string chain);
	int get_offset(std::string offset);

	void dump2(std::ostream& stream);
	void dump2(const std::string& file);

	int get_netvar_count()
	{
		return m_netvarCount;
	}

	int get_table_count()

	{
		return m_tableCount;
	}

	template <typename ...Args>
	uint32_t iget_offset(const std::string& tablename, Args&&... args)
	{
		return get_offset(tablename, { std::forward<Args>(args)... });
	}

private:
	std::unique_ptr<netvar_table> internal_load_table(recv_table* precvtable, uint32_t offset);
	void dump(std::ostream& output, netvar_table& table, int level);
	void dump2(std::ostream& output, netvar_table& table, int level, std::string first);

	uint32_t get_offset(const std::string& szTableName, const std::initializer_list<std::string>& props);

private:
	std::unique_ptr<NetvarDatabase> m_database = nullptr;
	uint32_t m_tableCount = 0;
	uint32_t m_netvarCount = 0;
};

#define return_netvar(table, ...) netvar_manager::_instance()->iget_offset(table, __VA_ARGS__)