#include "stageunit.hpp"

StageUnit::StageUnit(StageUnitType type) : m_type(type)
{
	m_data.resize(10, NULL);
}

StageUnit::~StageUnit()
{

}

void StageUnit::GetSpacePositionAndDataFlag(uint16_t& space_position, uint16_t& existing_flag)
{
	extern std::vector<uint16_t> z_num_to_flag;

	space_position = 0;
	existing_flag = 0;

	for (size_t i = 0; i < m_data.size(); ++i)
	{
		if (m_data[i] == NULL)
			continue;

		uint16_t data = *(m_data[i]);

		if (data == 0)
			space_position |= z_num_to_flag[i];
		else
			existing_flag |= z_num_to_flag[data];
	}
 }

void StageUnit::SetData(int index, uint16_t* data)
{
	m_data[index] = data;
}
