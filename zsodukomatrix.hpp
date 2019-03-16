#ifndef AUTOSODUKO2_ZSODUKOMATRIX_H_
#define AUTOSODUKO2_ZSODUKOMATRIX_H_

#include <vector>
#include <cstdint>
#include <queue>

#include "stageunit.hpp"

struct StagePosition
{
	uint16_t m_x;
	uint16_t m_y;

	StagePosition(uint16_t x = 0, uint16_t y = 0) : 
		m_x(x), 
		m_y(y)
	{
		
	}
};

class ZSodukoMatrix
{
public:	
	ZSodukoMatrix();
	virtual ~ZSodukoMatrix();

	// 由外界调用为当前 Stage 设置数字
	void SetData(std::vector<uint16_t> data);

	void SolveSoduko();

	void PrintToConsole();

protected:
	// 将当前 Stage 的指针存放到各个 unit 中，应在设置完数字之后被调用
	void InitUnits();

	// 统计当前 Stage 中的留白情况
	void CountSpace(std::queue< StagePosition > & space_queue);

	// 尝试填充单个空白
	uint16_t TryToFillSingleSpace(uint16_t x, uint16_t y);

	// 计算当前元素在对应的网格单元下标及其在对应网格单元内的下标
	void PositionMod3(uint16_t x, uint16_t y, uint16_t& grid_unit_index, uint16_t& grid_unit_data_index);
	void PositionMod3Reverse(uint16_t grid_unit_index, uint16_t grid_unit_data_index, uint16_t& x, uint16_t& y);

	// 获取单个网格相关行/列/网格单元的空白标记和不可用数据标记
	void GetSpaceFlagAndExistingFlag(
		uint16_t x, uint16_t y,
		uint16_t& row_space_flag, uint16_t& row_existing_flag,
		uint16_t& col_space_flag, uint16_t& col_existing_flag,
		uint16_t& grid_space_flag, uint16_t& grid_existing_flag);

	// 根据flag得出单个数字（对于非2整数幂的数字返回值均为0）
	uint16_t ConverFlagToNum(uint16_t flag);

	// 根据统计的flag得出对应的数字队列（将flag中可对应的数字全部存放到vector中）
	void ConvertFlagToVector(uint16_t flag, std::vector<uint16_t>& nums);

private:
	std::vector< std::vector<uint16_t> > m_stage;
	std::vector< StageUnit > m_row_units;
	std::vector< StageUnit > m_col_units;
	std::vector< StageUnit > m_grid_units;
};

#endif
