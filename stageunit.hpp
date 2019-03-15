#ifndef AUTOSODUKO2_STAGE_UNIT_H_
#define AUTOSODUKO2_STAGE_UNIT_H_

#include <vector>
#include <unordered_map>
#include <cstdint>


enum StageUnitType
{
	NONE,
	ROW,
	COL,
	GRID
};


class StageUnit
{
public:
	StageUnit(StageUnitType type = NONE);
	virtual ~StageUnit();

	void GetSpacePositionAndDataFlag(uint16_t& space_position, uint16_t& data_flag);

	void SetData(int index, uint16_t* data);

private:
	std::vector<uint16_t*>  m_data;
	StageUnitType			m_type;
};

#endif