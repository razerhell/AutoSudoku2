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

	// ��������Ϊ��ǰ Stage ��������
	void SetData(std::vector<uint16_t> data);

	void SolveSoduko();

	void PrintToConsole();

protected:
	// ����ǰ Stage ��ָ���ŵ����� unit �У�Ӧ������������֮�󱻵���
	void InitUnits();

	// ͳ�Ƶ�ǰ Stage �е��������
	void CountSpace(std::queue< StagePosition > & space_queue);

	// ������䵥���հ�
	uint16_t TryToFillSingleSpace(uint16_t x, uint16_t y);

	// ���㵱ǰԪ���ڶ�Ӧ������Ԫ�±꼰���ڶ�Ӧ����Ԫ�ڵ��±�
	void PositionMod3(uint16_t x, uint16_t y, int& grid_unit_index, int& data_index);

private:
	std::vector< std::vector<uint16_t> > m_stage;
	std::vector< StageUnit > m_row_units;
	std::vector< StageUnit > m_col_units;
	std::vector< StageUnit > m_grid_units;
};

#endif