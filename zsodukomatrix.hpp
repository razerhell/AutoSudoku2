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
	void PositionMod3(uint16_t x, uint16_t y, int& grid_unit_index, int& data_index);

private:
	std::vector< std::vector<uint16_t> > m_stage;
	std::vector< StageUnit > m_row_units;
	std::vector< StageUnit > m_col_units;
	std::vector< StageUnit > m_grid_units;
};

#endif