#ifndef MYLIB_DRIVER_MANAGER_H_
#define MYLIB_DRIVER_MANAGER_H_


#include "Driver.h"
<<<<<<< HEAD
#include "MemoryDriver.h"
#include "StatusDriver.h"


#include "../core/MyaiService.h"


MYAI_BEGIN
struct DriverConfig {
};

=======

MYAI_BEGIN
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)

class DriverManager {
public:
	using ptr = std::shared_ptr<DriverManager>;
	// using Motive = void(const Edge &);
<<<<<<< HEAD
<<<<<<< HEAD
	static constexpr nodeid_t MAX_CONTROL_NODE_ID = 0x1000'0000;

	DriverManager(MyaiService::ptr ser) : m_service(ser) {}

	void init() {

		m_status = std::make_shared<StatusDriver>(m_service->applyId(1000), 1000);
		m_memory = std::make_shared<MemoryDriver>(m_service->applyId(100), 100);
		m_drivers.push_back(m_status);
		m_drivers.push_back(m_memory);

		for (auto &driver: m_drivers) {
			driver->init();
		}
	}
	MyaiDriver::ptr addDriver(MyaiDriver::ptr driver);

	void collect(EdgeList::ptr out);

	/// @brief 控制
	/// @param output 对外输出数据
	void control(const Edge &output);

	// 返回m_positive的值
	auto positive() const { return m_status->m_positive; }
	auto negative() const { return m_status->m_negative; }
	auto filter() const { return m_status->m_filter; }
	auto driver_weight(MyaiDriver::Type source, MyaiDriver::Type target) const {
		return m_status->m_driver_weight[source][target];
	}
	void activate_node(const Edge &edge) {
		m_service->activatedNode(m_memory->getCollects(), edge);
	}

private:
	MyaiService::ptr m_service;
	StatusDriver::ptr m_status;
	MemoryDriver::ptr m_memory;
=======

	bool loadConfig();
	bool saveConfig();
=======
	static constexpr nodeid_t MAX_CONTROL_NODE_ID = 0x1000'0000;
>>>>>>> cf44269 (2025年2月23日 16:12:23)

	MyaiDriver::ptr addDriver(MyaiDriver::ptr driver) {
		m_drivers.push_back(driver);
		return driver;
	}

	void collect(EdgeList::ptr out) {
		for (auto &var: m_drivers) {
			auto temp = var->collect();
			out->insert(temp->begin(), temp->end());
		}
	}

	/// @brief 控制
	/// @param output 对外输出数据
	void control(const Edge &output) {
		MyaiDriver::S_CONNECTIONS.at(output.id)(output.weight);
	}

private:
>>>>>>> 574ffc2 (2025年2月23日 12:27:49)
	std::vector<MyaiDriver::ptr> m_drivers;
};

MYAI_END

#endif// !MYLIB_DRIVER_MANAGER_H_
